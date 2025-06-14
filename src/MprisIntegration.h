#ifndef MPRISINTEGRATION_H
#define MPRISINTEGRATION_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusContext>

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
    Q_CLASSINFO("D-Bus Introspection", "<interface name=\"org.mpris.MediaPlayer2.Player\">"
                                           "<property name=\"Position\" type=\"s\" access=\"\"/>"
                                           "<property name=\"MinimumRate\" type=\"d\" access=\"\"/>"
                                           "<property name=\"MaximumRate\" type=\"d\" access=\"\"/>"
                                           "<property name=\"CanGoNext\" type=\"b\" access=\"\"/>"
                                           "<property name=\"CanGoPrevious\" type=\"b\" access=\"\"/>"
                                           "<property name=\"CanPlay\" type=\"b\" access=\"\"/>"
                                           "<property name=\"CanPause\" type=\"b\" access=\"\"/>"
                                           "<property name=\"CanSeek\" type=\"b\" access=\"\"/>"
                                           "<property name=\"CanControl\" type=\"b\" access=\"\"/>"
                                           "<method name=\"Next\"/>"
                                           "<method name=\"Previous\"/>"
                                           "<method name=\"PausePlay\"/>"
                                           "<method name=\"Pause\"/>"
                                           "<method name=\"Play\"/>"
                                       "</interface>")

    Q_PROPERTY(QString Position MEMBER _position CONSTANT)
    Q_PROPERTY(double MinimumRate MEMBER _minimumRate CONSTANT)
    Q_PROPERTY(double MaximumRate MEMBER _maximumRate CONSTANT)
    Q_PROPERTY(bool CanGoNext MEMBER _canGoNext CONSTANT)
    Q_PROPERTY(bool CanGoPrevious MEMBER _canGoPrevious CONSTANT)
    Q_PROPERTY(bool CanPlay MEMBER _canPlay CONSTANT)
    Q_PROPERTY(bool CanPause MEMBER _canPause CONSTANT)
    Q_PROPERTY(bool CanSeek MEMBER _canSeek CONSTANT)
    Q_PROPERTY(bool CanControl MEMBER _canControl CONSTANT)
public:
    PlayerAdaptor(QObject *parent): QDBusAbstractAdaptor(parent) {}

public slots:
    Q_SCRIPTABLE void Next() {}
    Q_SCRIPTABLE void Previous() {}
    Q_SCRIPTABLE void Pause();
    Q_SCRIPTABLE void PausePlay() {}
    Q_SCRIPTABLE void Stop() {}
    Q_SCRIPTABLE void Play();
    Q_SCRIPTABLE void Seek(int x) {}
    Q_SCRIPTABLE void OpenUri(QString uri) {}

    Q_SCRIPTABLE void SetPosition(int trackId, int position) {}
    // void getPosition(int trackId, int position) {}

private:
    QString _position = "";
    double _maximumRate = 0.0;
    double _minimumRate = 0.0;

    const bool _canGoNext = false;
    const bool _canGoPrevious = false;
    const bool _canPlay = true;
    const bool _canPause = true;
    const bool _canSeek = false;
    const bool _canControl = false;
};

class MprisIntegration : public QObject {
    Q_OBJECT
    // Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Mastoid")
public:
    explicit MprisIntegration(QObject *parent = nullptr);
    ~MprisIntegration();

signals:
    void playRequest();
    void pauseRequest();

public Q_SLOTS:
    // root interface
    void Raise();
    void Quit();

    // player interface
    void Play();
    void Pause();

// signals:
// private:
public:
    RootAdaptor *_rootAdaptor;
    PlayerAdaptor *_playerAdaptor;
};

#endif // MPRISINTEGRATION_H
