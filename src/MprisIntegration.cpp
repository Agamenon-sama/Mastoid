#include "MprisIntegration.h"

#include <QtDBus>
#include <QStandardPaths>
#include <QImage>
#include <QDir>
#include <QCryptographicHash>

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

    auto folderName = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/mpris/";

    QDir folder(folderName);
    folder.removeRecursively();
}

void MprisIntegration::updateMetadata(const QString &trackId, const QString &title,
                                      const QStringList &artists, const QString &album,
                                      qlonglong lengthUs) {
    QVariantMap meta;
    meta["mpris:trackid"] = QVariant::fromValue(QDBusObjectPath("/org/mpris/MediaPlayer2/Track/" + trackId));
    meta["mpris:length"]  = lengthUs;
    meta["xesam:title"]   = title;
    meta["xesam:artist"]  = artists;
    meta["xesam:album"]   = album;

    meta["mpris:artUrl"] = "file://" + _coverArtPath; // file:// URL

    _playerAdaptor->setMetadata(meta);
}

void MprisIntegration::updatePlaybackStatus(const QString &status) {
    _playerAdaptor->setPlaybackStatus(status);
}

void MprisIntegration::updatePosition(qlonglong positionUs) {
    _currentPositionUs = positionUs;
}

void MprisIntegration::updateCoverArt(QString uri) {
    QUrl url(uri);
    QImage coverArt;

    if (url.scheme() == "qrc") {
        QString resourcePath = ":" + url.path();
        if (!coverArt.load(resourcePath)) {
            qWarning() << "updateCoverArt: Failed to load qrc resource" << resourcePath;
            return;
        }
    }
    else if (url.isLocalFile()) {
        if (!coverArt.load(url.path())) {
            qWarning() << "Failed to load the cover file" << url.path();
            return;
        }
    }
    else {
        qDebug() << "Unsupported url scheme for cover art" << url;
    }

    auto folderName = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/mpris/";

    QDir folder(folderName);
    if (!folder.exists()) {
        folder.mkpath(".");
    }

    QString hash = QCryptographicHash::hash(uri.toUtf8(), QCryptographicHash::Md5).toHex();
    QString fileName = "cover" + hash + ".jpg";

    if (folder.exists(fileName)) {
        _coverArtPath = folderName + fileName;
        return;
    }

    // resize the image to 128p while keeping aspect ratio
    float ratio = static_cast<float>(coverArt.width()) / coverArt.height();
    coverArt = coverArt.scaled(QSize(ratio * 128u, 128u));


    if (!coverArt.save(folderName + fileName, "JPG", 90)) {
        qWarning() << "Failed to write the file" << folderName + fileName << "for mpris support";
        return;
    }

    _coverArtPath = folderName + fileName;
}

void MprisIntegration::updateVolume(double volume) {
    _playerAdaptor->setVolumeToDBus(volume);
}

void MprisIntegration::notifySeeked(qlonglong positionUs) {
    emit _playerAdaptor->Seeked(positionUs);
}

void MprisIntegration::setPosition(qlonglong positionUs) {
    _currentPositionUs = positionUs;
    emit setPositionRequest(positionUs);
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

void MprisIntegration::PlayPause() {
    emit playPauseRequest();
}

void MprisIntegration::Stop() {
    emit stopRequest();
}

void MprisIntegration::Next() {
    emit nextRequest();
}

void MprisIntegration::Previous() {
    emit previousRequest();
}

void MprisIntegration::Seek(qlonglong x) {
    emit seekRequest(x);
}

void MprisIntegration::OpenUri(QString uri) {
    emit openUriRequest(uri);
}

void RootAdaptor::Raise() {
    static_cast<MprisIntegration*>(parent())->Raise();
}

void RootAdaptor::Quit() {
    static_cast<MprisIntegration*>(parent())->Quit();
}

void PlayerAdaptor::emitPropertiesChanged(const QVariantMap &changed, const QStringList &invalidated) {
    QDBusMessage msg = QDBusMessage::createSignal(
        "/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged");

    msg << "org.mpris.MediaPlayer2.Player" << changed << invalidated;

    QDBusConnection::sessionBus().send(msg);
}

void PlayerAdaptor::setMetadata(const QVariantMap &metadata) {
    _metadata = metadata;
    emitPropertiesChanged({{"Metadata", _metadata}});
}

void PlayerAdaptor::setPlaybackStatus(const QString &status) {
    if (_playbackStatus == status) return;
    _playbackStatus = status;
    emitPropertiesChanged({{"PlaybackStatus", _playbackStatus}});
}

void PlayerAdaptor::setLoopStatus(const QString &status) {
    if (_loopStatus == status) return;
    _loopStatus = status;
    emitPropertiesChanged({{"LoopStatus", _loopStatus}});
    static_cast<MprisIntegration*>(parent())->loopStatusChangeRequest(status);
}

void PlayerAdaptor::setVolume(double volume) {
    if (qFuzzyCompare(_volume, volume)) return;

    _volume = std::clamp(volume, 0.0, 1.0);
    emitPropertiesChanged({{"Volume", _volume}});
    static_cast<MprisIntegration*>(parent())->volumeChangeRequest(_volume);
}

void PlayerAdaptor::setVolumeToDBus(double volume) {
    _volume = volume;
    emitPropertiesChanged({{"Volume", _volume}});
}

void PlayerAdaptor::setCanGoNext(bool v) {
    if (_canGoNext == v) return;

    _canGoNext = v;
    emitPropertiesChanged({{"CanGoNext", _canGoNext}});
}

void PlayerAdaptor::setCanGoPrevious(bool v) {
    if (_canGoPrevious == v) return;

    _canGoPrevious = v;
    emitPropertiesChanged({{"CanGoPrevious", _canGoPrevious}});
}

void PlayerAdaptor::setCanPlay(bool v) {
    if (_canPlay == v) return;

    _canPlay = v;
    emitPropertiesChanged({{"CanPlay", _canPlay}});
}

void PlayerAdaptor::setCanPause(bool v) {
    if (_canPause == v) return;

    _canPause = v;
    emitPropertiesChanged({{"CanPause", _canPause}});
}

void PlayerAdaptor::setCanSeek(bool v) {
    if (_canSeek == v) return;

    _canSeek = v;
    emitPropertiesChanged({{"CanSeek", _canSeek}});
}

qlonglong PlayerAdaptor::position() const {
    return static_cast<MprisIntegration*>(parent())->currentPositionUs();
}

void PlayerAdaptor::Play() {
    static_cast<MprisIntegration*>(parent())->Play();
}

void PlayerAdaptor::Pause() {
    static_cast<MprisIntegration*>(parent())->Pause();
}

void PlayerAdaptor::PlayPause() {
    static_cast<MprisIntegration*>(parent())->PlayPause();
}

void PlayerAdaptor::Stop() {
    static_cast<MprisIntegration*>(parent())->Stop();
}

void PlayerAdaptor::Next() {
    static_cast<MprisIntegration*>(parent())->Next();
}

void PlayerAdaptor::Previous() {
    static_cast<MprisIntegration*>(parent())->Previous();
}

void PlayerAdaptor::Seek(const qlonglong x) {
    static_cast<MprisIntegration*>(parent())->Seek(x);
}

void PlayerAdaptor::OpenUri(QString uri) {
    static_cast<MprisIntegration*>(parent())->OpenUri(uri);
}


void PlayerAdaptor::SetPosition(const QDBusObjectPath &trackId, qlonglong position) {
    if (!_canSeek) return;

    if (position < 0) return;
    auto duration = _metadata.value("mpris:length").value<qlonglong>();
    if (position > duration) return;

    // NOTE: The spec requires the trackId to match current ID.
    // In my current test, the client always sends an empty id.
    // I'll disable the track id check for now but this needs to be reviewed.

    // auto current = _metadata.value("mpris:trackid").value<QDBusObjectPath>();
    // qDebug() << "Recieved SetPosition" << trackId << position;
    // qDebug() << "current trackid" << current;

    // if (current.path() != trackId.path())
    //     return;

    static_cast<MprisIntegration*>(parent())->setPosition(position);
}
