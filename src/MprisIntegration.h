#ifndef MPRISINTEGRATION_H
#define MPRISINTEGRATION_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusContext>
#include <QDBusObjectPath>

class RootAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    Q_CLASSINFO("D-Bus Introspection", "<interface name=\"org.mpris.MediaPlayer2\">"
                                            "<property name=\"CanRaise\" type=\"b\" access=\"\"/>"
                                            "<property name=\"CanQuit\" type=\"b\" access=\"\"/>"
                                            "<property name=\"Fullscreen\" type=\"b\" access=\"\"/>"
                                            "<property name=\"HasTrackList\" type=\"b\" access=\"\"/>"
                                            "<property name=\"Identity\" type=\"s\" access=\"\"/>"
                                            "<property name=\"DesktopEntry\" type=\"s\" access=\"\"/>"
                                            "<method name=\"Raise\"/>"
                                            "<method name=\"Quit\"/>"
                                        "</interface>")

    Q_PROPERTY(bool CanRaise MEMBER _canRaise CONSTANT)
    Q_PROPERTY(bool CanQuit MEMBER _canQuit CONSTANT)
    Q_PROPERTY(bool Fullscreen MEMBER _fullscreen CONSTANT)
    Q_PROPERTY(bool HasTrackList MEMBER _hasTrackList CONSTANT)
    Q_PROPERTY(QString Identity MEMBER _identity CONSTANT)
    Q_PROPERTY(QString DesktopEntry MEMBER _desktopEntry CONSTANT)
public:
    RootAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent) {}

public slots:
    void Raise();
    void Quit();

private:
    const bool _canRaise = false;
    const bool _canQuit = true;
    const bool _fullscreen = false;
    const bool _hasTrackList = false;
    const QString _identity = "Mastoid";
    const QString _desktopEntry = "Mastoid";
};

class PlayerAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")

    Q_PROPERTY(QString PlaybackStatus MEMBER _playbackStatus READ playbackStatus)
    Q_PROPERTY(QString LoopStatus MEMBER _loopStatus READ loopStatus WRITE setLoopStatus)
    Q_PROPERTY(double Rate MEMBER _rate READ rate WRITE setRate)
    Q_PROPERTY(bool Shuffle MEMBER _shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(QVariantMap Metadata MEMBER _metadata READ metadata)
    Q_PROPERTY(double Volume MEMBER _volume READ volume WRITE setVolume)
    Q_PROPERTY(qlonglong Position MEMBER _position READ position)
    Q_PROPERTY(double MinimumRate MEMBER _minimumRate CONSTANT)
    Q_PROPERTY(double MaximumRate MEMBER _maximumRate CONSTANT)
    Q_PROPERTY(bool CanGoNext MEMBER _canGoNext)
    Q_PROPERTY(bool CanGoPrevious MEMBER _canGoPrevious)
    Q_PROPERTY(bool CanPlay MEMBER _canPlay)
    Q_PROPERTY(bool CanPause MEMBER _canPause)
    Q_PROPERTY(bool CanSeek MEMBER _canSeek)
    Q_PROPERTY(bool CanControl MEMBER _canControl CONSTANT)
public:
    PlayerAdaptor(QObject *parent): QDBusAbstractAdaptor(parent) {}

    // get
    QString playbackStatus() const { return _playbackStatus; }
    QString loopStatus() const { return _loopStatus; }
    double rate() const { return _rate; }
    bool shuffle() const { return _shuffle; }
    QVariantMap metadata() const { return _metadata; }
    double volume() const { return _volume; }
    qlonglong position() const;
    double minimumRate() const { return _minimumRate; }
    double maximumRate() const { return _maximumRate; }
    bool canGoNext() const { return _canGoNext; }
    bool canGoPrevious() const { return _canGoPrevious; }
    bool canPlay() const { return _canPlay; }
    bool canPause() const { return _canPause; }
    bool canSeek() const { return _canSeek; }
    bool canControl() const { return _canControl; }

    // set
    void setPlaybackStatus(const QString &status);
    void setLoopStatus(const QString &status);
    void setRate(double rate) {}
    void setShuffle(bool shuffle) {}
    void setMetadata(const QVariantMap &metadata);
    void setVolume(double volume);
    void setVolumeToDBus(double volume);
    void setMinimumRate(double v) { _minimumRate = v; }
    void setMaximumRate(double v) { _maximumRate = v; }
    void setCanGoNext(bool v);
    void setCanGoPrevious(bool v);
    void setCanPlay(bool v);
    void setCanPause(bool v);
    void setCanSeek(bool v);
    void setCanControl(bool v) { _canControl = v; }

signals:
    Q_SCRIPTABLE void Seeked(qlonglong x);

public slots:
    Q_SCRIPTABLE void Next();
    Q_SCRIPTABLE void Previous();
    Q_SCRIPTABLE void Pause();
    Q_SCRIPTABLE void PlayPause();
    Q_SCRIPTABLE void Stop();
    Q_SCRIPTABLE void Play();
    Q_SCRIPTABLE void Seek(const qlonglong x);
    Q_SCRIPTABLE void SetPosition(const QDBusObjectPath &trackId, qlonglong position);
    Q_SCRIPTABLE void OpenUri(QString uri);

private:
    QString _playbackStatus = QStringLiteral("Stopped");
    QString _loopStatus = QStringLiteral("None");
    double _rate = 1.0;
    bool _shuffle = false;
    QVariantMap _metadata;
    double _volume = 1.0;
    qlonglong _position = 0;
    double _maximumRate = 1.0;
    double _minimumRate = 1.0;

    bool _canGoNext = false;
    bool _canGoPrevious = false;
    bool _canPlay = true;
    bool _canPause = true;
    bool _canSeek = true;
    bool _canControl = true;

    void emitPropertiesChanged(const QVariantMap &changed, const QStringList &invalidated = {});
};

class MprisIntegration : public QObject {
    Q_OBJECT
    // Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Mastoid")
public:
    explicit MprisIntegration(QObject *parent = nullptr);
    ~MprisIntegration();

    Q_INVOKABLE void updateMetadata(
        const QString &trackId, const QString &title,
        const QStringList &artists, const QString &album,
        qlonglong lengthUs
    );
    Q_INVOKABLE void updatePlaybackStatus(const QString &status);
    Q_INVOKABLE void updatePosition(qlonglong positionUs);
    Q_INVOKABLE void updateCoverArt(QString uri);
    Q_INVOKABLE void updateVolume(double volume);
    // Q_INVOKABLE void updateCanGoNext(bool v);
    // Q_INVOKABLE void updateCanGoPrevious(bool v);

    Q_INVOKABLE void notifySeeked(qlonglong positionUs);

    qlonglong currentPositionUs() const { return _currentPositionUs; }
    void setPosition(qlonglong positionUs);

signals:
    void playRequest();
    void pauseRequest();
    void playPauseRequest();
    void stopRequest();
    void nextRequest();
    void previousRequest();
    void seekRequest(const int x);
    void setPositionRequest(qlonglong position);
    void openUriRequest(QString uri);
    void loopStatusChangeRequest(const QString &status);
    void rateChangeRequest(double rate);
    void shuffleChangeRequest(bool shuffle);
    void volumeChangeRequest(double volume);

public slots:
    // root interface
    void Raise();
    void Quit();

    // player interface
    void Play();
    void Pause();
    void PlayPause();
    void Stop();
    void Next();
    void Previous();
    void Seek(qlonglong x);
    void OpenUri(QString uri);

private:
// public:
    RootAdaptor *_rootAdaptor;
    PlayerAdaptor *_playerAdaptor;

    QString _coverArtPath;
    qlonglong _currentPositionUs = 0;
};

#endif // MPRISINTEGRATION_H
