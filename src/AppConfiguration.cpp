#include "AppConfiguration.h"

#include <QDir>
#include <QDebug>

AppConfiguration::AppConfiguration(const QCommandLineParser &parser)
    : QObject{nullptr}, _startupFileName("") {

    if (parser.value("f") != "") {
        _startupFileName = parser.value("f");

        // todo: might want to make this posix specific
        // todo: make it work with relative paths
        // if the path start with "~/" replace it with the home directory
        if (_startupFileName.left(2) == "~/") {
            _startupFileName = QDir::homePath() + "/" + _startupFileName.remove(0, 2);
        }
    }
}

QUrl AppConfiguration::getStartupFile() const {
    return QUrl("file:" + _startupFileName);
}
