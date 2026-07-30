#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <QObject>
#include <QAudioBuffer>
#include <QAudioBufferOutput>
#include <QVariantList>

#include "RingBuffer.h"

class SpectrumAnalyzer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QAudioBufferOutput* bufferOutput READ bufferOutput CONSTANT)
    Q_PROPERTY(QVariantList magnitudes READ magnitudes NOTIFY magnitudesChanged)

public:
    explicit SpectrumAnalyzer(QObject *parent = nullptr);

    QAudioBufferOutput *bufferOutput() const { return _bufferOutput; }
    QVariantList magnitudes() const { return _magnitudes; }
signals:
    void magnitudesChanged();

private slots:
    void processBuffer(const QAudioBuffer &buffer);
    void computeSpectrum();

private:
    QAudioBufferOutput *_bufferOutput;
    QVariantList _magnitudes;
    RingBuffer _leftBuffer;
    RingBuffer _rightBuffer;
    float _fftRate = 30.f;
};

#endif // SPECTRUMANALYZER_H
