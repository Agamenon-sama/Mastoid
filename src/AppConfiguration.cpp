#include "AppConfiguration.h"

#include <QDir>
#include <QDebug>
#include <QStandardPaths>

AppConfiguration::AppConfiguration(const QCommandLineParser &parser)
    : QObject{nullptr}, _startupFileName("")
    , _width(1080), _height(600) {

    _configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mastoid/config";

    if (parser.value("f") != "") {
        _startupFileName = parser.value("f");

        // todo: might want to make this posix specific
        // if the path start with "~/" replace it with the home directory
        if (_startupFileName.left(2) == "~/") {
            _startupFileName = QDir::homePath() + "/" + _startupFileName.remove(0, 2);
        }
    }

    _parseConfig();
}

QUrl AppConfiguration::getStartupFile() const {
    return QUrl("file:" + _startupFileName);
}

void AppConfiguration::_parseConfig() {
    QFile file(_configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // todo: properly log this
        qDebug() << "failed to open config file: " << _configFileName;
        return;
    }

    QTextStream fileIn(&file);
    while (!fileIn.atEnd()) {
        auto line = fileIn.readLine().trimmed();
        auto key = line.section('=', 0, 0).trimmed();
        auto value = line.section('=', 1, 1).trimmed();

        if (key == "window_width") {
            bool ok;
            _width = value.toInt(&ok);
            continue;
        }
        else if (key == "window_height") {
            bool ok;
            _height = value.toInt(&ok);
            continue;
        }
    }
}
