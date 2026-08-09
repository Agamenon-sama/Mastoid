#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <vector>
#include <deque>

#include <QObject>
#include <QAudioBuffer>
#include <QAudioBufferOutput>
#include <QVariantList>
#include <QElapsedTimer>

#include <fftw3.h>

#include "RingBuffer.h"

class SpectrumAnalyzer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QAudioBufferOutput* bufferOutput READ bufferOutput CONSTANT)
    Q_PROPERTY(bool processingEnabled READ processingEnabled WRITE setProcessingEnabled NOTIFY processingEnabledChanged)
    Q_PROPERTY(QVariantList leftMagnitudes READ leftMagnitudes NOTIFY magnitudesChanged)
    Q_PROPERTY(QVariantList rightMagnitudes READ rightMagnitudes NOTIFY magnitudesChanged)
    Q_PROPERTY(qreal bass READ bass NOTIFY magnitudesChanged)
    Q_PROPERTY(qreal treble READ treble NOTIFY magnitudesChanged)
    Q_PROPERTY(qreal beat READ beat NOTIFY magnitudesChanged)

public:
    explicit SpectrumAnalyzer(QObject *parent = nullptr);
    ~SpectrumAnalyzer();

    QAudioBufferOutput *bufferOutput() const { return _bufferOutput; }
    bool processingEnabled() const { return _processingEnabled; }
    QVariantList leftMagnitudes() const { return _leftMagnitudes; }
    QVariantList rightMagnitudes() const { return _rightMagnitudes; }
    qreal bass() const { return _bassLevel; }
    qreal treble() const { return _trebleLevel; }
    qreal beat() const { return _beatPulse; }

    void setProcessingEnabled(bool enabled);

signals:
    void magnitudesChanged();
    void processingEnabledChanged();

private slots:
    void processBuffer(const QAudioBuffer &buffer);
    void computeSpectrum();

private:
    QAudioBufferOutput *_bufferOutput;
    bool _processingEnabled = true;
    QVariantList _leftMagnitudes;
    QVariantList _rightMagnitudes;
    float _bassLevel = 0.f;
    float _trebleLevel = 0.f;
    float _beatPulse = 0.f;

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
    int _bassBinStart = 0, _bassBinEnd = 0;
    int _trebleBinStart = 0, _trebleBinEnd = 0;

    float _runningPeak = 1e-6f;
    static constexpr float kPeakDecay = 0.97f;
    static constexpr float kBeatDecay = 0.85f;
    static constexpr qint64 kBeatDebounceMs = 150;

    std::deque<float> _bassEnergyHistory;
    QElapsedTimer _beatTimer;  // start() in constructor
    qint64 _lastBeatMs = 0;
    float _bassRunningPeak = 0.f;
    float _trebleRunningPeak = 0.f;

    void _rebuildBandEdges();
    QVariantList _computeBandsForChannel(const fftwf_complex *fftOut);
    void _updateBassTrebleAndBeat();
};

#endif // SPECTRUMANALYZER_H
