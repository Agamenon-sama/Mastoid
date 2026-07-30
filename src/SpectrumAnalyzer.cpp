#include "SpectrumAnalyzer.h"

#include <numeric>

#include <QTimer>

SpectrumAnalyzer::SpectrumAnalyzer(QObject *parent)
    : QObject{parent}, _leftBuffer(kFftSize * 4), _rightBuffer(kFftSize * 4)
{
    _bufferOutput = new QAudioBufferOutput(this);
    connect(_bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &SpectrumAnalyzer::processBuffer);

    _fftInLeft = fftwf_alloc_real(kFftSize); Q_CHECK_PTR(_fftInLeft);
    _fftOutLeft = fftwf_alloc_complex(kSpectrumBins); Q_CHECK_PTR(_fftOutLeft);

    _fftInRight = fftwf_alloc_real(kFftSize); Q_CHECK_PTR(_fftInRight);
    _fftOutRight = fftwf_alloc_complex(kSpectrumBins); Q_CHECK_PTR(_fftOutRight);

    _fftPlan = fftwf_plan_dft_r2c_1d(static_cast<int>(kFftSize), _fftInLeft, _fftOutLeft, FFTW_MEASURE);
    Q_CHECK_PTR(_fftPlan);

    _hannWindow.resize(kFftSize);
    for (size_t i = 0; i < kFftSize; i++) {
        _hannWindow[i] = 0.5f * (1.f - std::cos(2.f * std::numbers::pi_v<float> * i / (kFftSize - 1)));
    }


    auto *timer = new QTimer(this);
    timer->setInterval(static_cast<int>(1000.f / _fftRate));
    connect(timer, &QTimer::timeout, this, &SpectrumAnalyzer::computeSpectrum);
    timer->start();
}

SpectrumAnalyzer::~SpectrumAnalyzer() {
    if (_fftPlan) fftwf_destroy_plan(_fftPlan);

    if (_fftOutRight) fftwf_free(_fftOutRight);
    if (_fftInRight) fftwf_free(_fftInRight);

    if (_fftOutLeft) fftwf_free(_fftOutLeft);
    if (_fftInLeft) fftwf_free(_fftInLeft);
}

template<typename T>
inline float normalizeSample(T value);

template<>
inline float normalizeSample<float>(float value) { return value; }

template<>
inline float normalizeSample<qint16>(qint16 value) { return value / 32768.f; }

template<>
inline float normalizeSample<qint32>(qint32 value) { return value / 2147483648.f; }

template<>
inline float normalizeSample<quint8>(quint8 value) {
    // UInt8 PCM is unsigned, centered at 128
    return (static_cast<int>(value) - 128) / 128.f;
}

template<typename T>
void extractChannels(const QAudioBuffer &buffer, std::vector<float> &left, std::vector<float> &right) {
    const T *data = buffer.constData<T>();
    const int channels = buffer.format().channelCount();
    const int frames = buffer.frameCount();

    left.resize(frames);
    right.resize(frames);

    if (channels == 1) {
        // mono -> duplicate into both channels
        for (int i = 0; i < frames; i++) {
            const float s = normalizeSample<T>(data[i]);
            left[i] = s;
            right[i] = s;
        }
    } else if (channels == 2) {
        for (int i = 0; i < frames; i++) {
            left[i]  = normalizeSample<T>(data[i * 2 + 0]);
            right[i] = normalizeSample<T>(data[i * 2 + 1]);
        }
    } else {
        // >2 channels -> Treat it as mono
        for (int i = 0; i < frames; i++) {
            float sum = 0.f;
            for (int c = 0; c < channels; c++)
                sum += normalizeSample<T>(data[i * channels + c]);
            const float avg = sum / channels;
            left[i]  = avg;
            right[i] = avg;
        }
    }
}

void SpectrumAnalyzer::processBuffer(const QAudioBuffer &buffer) {
    if (!buffer.isValid() || buffer.frameCount() == 0)
        return;

    if (buffer.format().sampleRate() != _sampleRate) {
        _sampleRate = buffer.format().sampleRate();
        _rebuildBandEdges();
    }
    // qDebug() << buffer.format();

    std::vector<float> left, right;

    switch (buffer.format().sampleFormat()) {
    case QAudioFormat::Float:
        extractChannels<float>(buffer, left, right);
        break;
    case QAudioFormat::Int32:
        extractChannels<qint32>(buffer, left, right);
        break;
    case QAudioFormat::Int16:
        extractChannels<qint16>(buffer, left, right);
        break;
    case QAudioFormat::UInt8:
        extractChannels<quint8>(buffer, left, right);
        break;
    default:
        qWarning() << "Spectrum Analyzer: Unknown sample format" << buffer.format().sampleFormat();
        break;
    }

    if (left.empty()) return;

    _leftBuffer.write(left.data(), left.size());
    _rightBuffer.write(right.data(), right.size());
}

void SpectrumAnalyzer::computeSpectrum() {
    if (!_leftBuffer.hasEnoughSamples(2048)) {
        return;
    }
    // qDebug() << "computing spectrum...";


    std::vector<float> left(kFftSize), right(kFftSize);
    _leftBuffer.read(left.data(), kFftSize);
    _rightBuffer.read(right.data(), kFftSize);

    for (size_t i = 0; i < kFftSize; i++) {
        _fftInLeft[i] = left[i] * _hannWindow[i];
        _fftInRight[i] = right[i] * _hannWindow[i];
    }

    fftwf_execute_dft_r2c(_fftPlan, _fftInLeft, _fftOutLeft);
    fftwf_execute_dft_r2c(_fftPlan, _fftInRight, _fftOutRight);

    QVariantList leftRaw  = _computeBandsForChannel(_fftOutLeft);
    QVariantList rightRaw = _computeBandsForChannel(_fftOutRight);

    float frameMax = 0.f;
    for (const QVariant &v : leftRaw)  frameMax = std::max(frameMax, v.toFloat());
    for (const QVariant &v : rightRaw) frameMax = std::max(frameMax, v.toFloat());

    _runningPeak = std::max(frameMax, _runningPeak * kPeakDecay);

    auto normalize = [this](const QVariantList &raw) {
        QVariantList out;
        out.reserve(raw.size());
        for (const QVariant &v : raw) {
            float n = v.toFloat() / _runningPeak;
            n = std::pow(std::clamp(n, 0.f, 1.f), 0.5f);
            out.append(n);
        }
        return out;
    };

    _leftMagnitudes  = normalize(leftRaw);
    _rightMagnitudes = normalize(rightRaw);

    // QVariantList mags;
    // mags.reserve(_numBars);

    // const float norm = 2.f / kFftSize;

    // for (int b = 0; b < _numBars; ++b) {
    //     const int startBin = _bandEdges[b];
    //     const int endBin = _bandEdges[b + 1]; // exclusive
    //     float maxMag = 0.f;

    //     for (int i = startBin; i < endBin; i++) {
    //         const float re = _fftOut[i][0];
    //         const float im = _fftOut[i][1];
    //         const float mag = std::sqrt(re * re + im * im) * norm;
    //         maxMag = std::max(maxMag, mag);
    //     }

    //     // logarithmic
    //     const float db = 20.f * std::log10(std::max(maxMag, 1e-9f));
    //     // remap [-60, 0] db to [0, 1], clamp and store
    //     mags.append(std::clamp((db + 60.f) / 60.f, 0.f, 1.f));

    //     // linear
    //     // It's not really linear since I'm applying a gamma correction.
    //     // Also, I pulled the 5.f and 0.35f from my ass
    //     // const float val = std::clamp(maxMag * 5.f, 0.f, 1.f);
    //     // mags.append(std::pow(val, 0.45f));
    // }

    // _magnitudes = mags;

    emit magnitudesChanged();
}

void SpectrumAnalyzer::_rebuildBandEdges() {
    const float minFreq = 30.f; // below this is mostly inaudible
    const float maxFreq = _sampleRate / 2.f; // Nyquist-Shannon

    _bandEdges.resize(_numBars + 1);

    // logarithmic interpolation between minFreq and maxFreq
    for (int i = 0; i <= _numBars; i++) {
        const float t = float(i) / _numBars;
        const float freq = minFreq * std::pow(maxFreq / minFreq, t);
        int bin = static_cast<int>(freq * kFftSize / _sampleRate);
        _bandEdges[i] = std::clamp(bin, 0, int(kSpectrumBins) - 1);
    }

    for (int i = 1; i <= _numBars; i++) {
        if (_bandEdges[i] <= _bandEdges[i - 1]) {
            _bandEdges[i] = _bandEdges[i - 1] + 1;
        }
    }
}

QVariantList SpectrumAnalyzer::_computeBandsForChannel(const fftwf_complex *fftOut) {
    const float norm = 2.f / kFftSize;

    QVariantList bars;
    bars.reserve(_numBars);

    for (int b = 0; b < _numBars; ++b) {
        const int startBin = _bandEdges[b];
        const int endBin = _bandEdges[b + 1];
        float maxMag = 0.f;
        for (int i = startBin; i < endBin; i++) {
            const float re = fftOut[i][0];
            const float im = fftOut[i][1];
            maxMag = std::max(maxMag, std::sqrt(re * re + im * im) * norm);
        }
        bars.append(maxMag); // still raw linear here
    }
    return bars;
}
