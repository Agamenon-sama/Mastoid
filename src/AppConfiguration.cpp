#include "AppConfiguration.h"

#include <QDir>
#include <QDebug>
#include <QStandardPaths>

AppConfiguration::AppConfiguration(const QCommandLineParser &parser)
    : QObject{nullptr}, _startupFileName("")
    , _width(1080), _height(600), _runInTray(true) {

    _configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Mastoid/mastoid.cfg";
    _baseDirectory = "file:" + QStandardPaths::writableLocation(QStandardPaths::MusicLocation);

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

int AppConfiguration::width() const {
    return _width;
}

int AppConfiguration::height() const {
    return _height;
}

QUrl AppConfiguration::baseDirectory() const {
    return _baseDirectory;
}

bool AppConfiguration::runInTray() const {
    return _runInTray;
}

void AppConfiguration::setWidth(int width) {
    if (_width == width) {
        return;
    }

    _width = width;
    emit widthChanged();
}

void AppConfiguration::setHeight(int height) {
    if (_height == height) {
        return;
    }

    _height = height;
    emit heightChanged();
}

void AppConfiguration::setBaseDirectory(QUrl baseDirectory) {
    if (_baseDirectory == baseDirectory) {
        return;
    }

    _baseDirectory = baseDirectory;
    emit baseDirectoryChanged();
}

void AppConfiguration::setRunInTray(bool run) {
    if (_runInTray == run) {
        return;
    }

    _runInTray = run;
    emit runInTrayChanged();
}

void AppConfiguration::_parseConfig() {
    QFile file(_configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "failed to open config file: " << _configFileName << ":\n" << file.errorString();
        return;
    }

    QTextStream fileIn(&file);
    while (!fileIn.atEnd()) {
        auto line = fileIn.readLine().trimmed();

        // remove anything after any # sign
        line = line.left(line.indexOf('#')).trimmed();
        if (line == "") continue; // if there is nothing left, go to next line

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
        else if (key == "base_directory") {
            if (value.left(2) == "~/") {
                value = QDir::homePath() + "/" + value.remove(0, 2);
            }
            _baseDirectory = "file:" + value;
            continue;
        }
        else if (key == "run_in_system_tray") {
            if (value.toLower() == "false") {
                _runInTray = false;
            }
            continue;
        }
    }
}
