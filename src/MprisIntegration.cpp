#include "MprisIntegration.h"

// #include <QtDBus/QtDBus> // TODO: try <QtDBus
#include <QtDBus>

MprisIntegration::MprisIntegration(QObject *parent)
    : QObject{parent}
{
    _rootAdaptor = new RootAdaptor(this);
    _playerAdaptor = new PlayerAdaptor(this);
    if (!QDBusConnection::sessionBus().registerService("org.mpris.MediaPlayer2.Mastoid")) {
        qCritical() << "Can't register the mpris service on dbus, "
                       "likely because another Service is registered as 'org.mpris.MediaPlayer2.Mastoid'."
                       "Maybe another Mastoid instance ?"
                       "Mastoid will start without a Mpris access";
        return;
    }
    if (!QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this)) {
        qCritical() << "Can't register the player interface";
    }
}

MprisIntegration::~MprisIntegration() {
    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2.Mastoid");
}

void MprisIntegration::Raise() {
    qDebug() << "TODO: implement raise";
}

void MprisIntegration::Quit() {
    qApp->quit();
}

void MprisIntegration::Play() {
    emit playRequest();
}

void MprisIntegration::Pause() {
    emit pauseRequest();
}


void RootAdaptor::Raise() {
    static_cast<MprisIntegration*>(parent())->Raise();
}

void RootAdaptor::Quit() {
    static_cast<MprisIntegration*>(parent())->Quit();
}

void PlayerAdaptor::Play() {
    static_cast<MprisIntegration*>(parent())->Play();
}

void PlayerAdaptor::Pause() {
    static_cast<MprisIntegration*>(parent())->Pause();
}
