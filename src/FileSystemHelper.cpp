#include "FileSystemHelper.h"

#include <QDir>
#include <QDebug>

FileSystemHelper::FileSystemHelper(QObject *parent) : QObject{parent} {}

QUrl FileSystemHelper::findCoverUrl(QString searchFolder) const {
    // transform the string that was sent as a url to a path for QDir
    QDir dir(QUrl(searchFolder).path());

    // QDir creates a list of all the files in the directory
    auto list = dir.entryList();

    // depending on this order of priority, if the file exists,
    //  we return the cover with these extensions
    std::vector<QString> possibleCoverFiles = {
        "cover.webp", "cover.png", "cover.jpg", "cover.jpeg",
        "folder.webp", "folder.png", "folder.jpg", "folder.jpeg"
    };

    for (const auto& coverFile : possibleCoverFiles) {
        if (list.contains(coverFile)) {
            return searchFolder + "/" + coverFile;
        }
    }

    // else we return the default cover
    return QUrl(u"qrc:/icons/Mastoid.png"_qs);
}
