#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <vector>

#include <QObject>
#include <QAudioBuffer>
#include <QAudioBufferOutput>
#include <QVariantList>

#include <fftw3.h>

#include "RingBuffer.h"

class SpectrumAnalyzer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QAudioBufferOutput* bufferOutput READ bufferOutput CONSTANT)
    Q_PROPERTY(QVariantList leftMagnitudes READ leftMagnitudes NOTIFY magnitudesChanged)
    Q_PROPERTY(QVariantList rightMagnitudes READ rightMagnitudes NOTIFY magnitudesChanged)

public:
    explicit SpectrumAnalyzer(QObject *parent = nullptr);
    ~SpectrumAnalyzer();

    QAudioBufferOutput *bufferOutput() const { return _bufferOutput; }
    QVariantList leftMagnitudes() const { return _leftMagnitudes; }
    QVariantList rightMagnitudes() const { return _rightMagnitudes; }

signals:
    void magnitudesChanged();

private slots:
    void processBuffer(const QAudioBuffer &buffer);
    void computeSpectrum();

private:
    QAudioBufferOutput *_bufferOutput;
    QVariantList _leftMagnitudes;
    QVariantList _rightMagnitudes;

    RingBuffer _leftBuffer;
    RingBuffer _rightBuffer;
    float _fftRate = 30.f;

    static constexpr size_t kFftSize = 2048;
    static constexpr size_t kSpectrumBins = kFftSize / 2 + 1;

    float *_fftInLeft = nullptr;
    float *_fftInRight = nullptr;
    fftwf_complex *_fftOutLeft = nullptr;
    fftwf_complex *_fftOutRight = nullptr;
    fftwf_plan _fftPlan = nullptr;
    std::vector<float> _hannWindow;

    int _sampleRate = 0; // updated from the first buffer received
    int _numBars = 256;
    std::vector<int> _bandEdges; // precomputed bin index boundaries, size _numBars+1

    float _runningPeak = 1e-6f;
    static constexpr float kPeakDecay = 0.97f;

    void _rebuildBandEdges();
    QVariantList _computeBandsForChannel(const fftwf_complex *fftOut);
};

#endif // SPECTRUMANALYZER_H
