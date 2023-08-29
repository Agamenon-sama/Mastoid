#include "FileSystemHelper.h"

#include <QDir>
#include <QDebug>

FileSystemHelper::FileSystemHelper(QObject *parent) : QObject{parent} {}

QUrl FileSystemHelper::findCoverUrl(QString searchFolder) {
    // transform the string that was sent as a url to a path for QDir
    QDir dir(QUrl(searchFolder).path());

    // QDir creates a list of all the files in the directory
    auto list = dir.entryList();

    // depending on this order of priority, if the file exists,
    //  we return the cover with these extensions
    if (list.contains("cover.webp")) {
        return searchFolder + "/cover.webp";
    }

    if (list.contains("cover.png")) {
        return searchFolder + "/cover.png";
    }

    if (list.contains("cover.jpg")) {
        return searchFolder + "/cover.jpg";
    }

    if (list.contains("cover.jpeg")) {
        return searchFolder + "/cover.jpeg";
    }

    // else we return the default cover
    return QUrl(u"qrc:/icons/defaultCover.png"_qs);
}
