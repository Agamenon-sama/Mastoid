#include "SpectrumAnalyzer.h"

#include <random>

#include <QTimer>

SpectrumAnalyzer::SpectrumAnalyzer(QObject *parent)
    : QObject{parent}, _leftBuffer(8192), _rightBuffer(8192)
{
    _bufferOutput = new QAudioBufferOutput(this);
    connect(_bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &SpectrumAnalyzer::processBuffer);

    auto *timer = new QTimer(this);
    timer->setInterval(static_cast<int>(1000.f / _fftRate));
    connect(timer, &QTimer::timeout, this, &SpectrumAnalyzer::computeSpectrum);
    timer->start();
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
            float sum = 0.0f;
            for (int c = 0; c < channels; ++c)
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

    // ... convert to mono float, accumulate into _ringBuffer,
    // run FFT once you have enough samples, compute magnitudes ...

    _magnitudes.clear();

    // std::random_device rd;
    // std::minstd_rand rng(rd());
    // std::uniform_real_distribution dist(0.f, 1.f);

    std::vector<float> mags(256);
    _leftBuffer.read(mags.data(), 256);

    for (int i = 0; i < 256; i++) {
        // _magnitudes.append(dist(rng));
        _magnitudes.append(mags[i]);
    }

    emit magnitudesChanged();
}
