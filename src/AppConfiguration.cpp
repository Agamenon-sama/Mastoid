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

// This function can hide many bugs :(
void AppConfiguration::save() const {
    QFile file(_configFileName);
    if (!file.exists()) {
        QFile newFile(_configFileName);
        if (!newFile.open(QIODevice::NewOnly)) {
            qWarning() << "Config file " << _configFileName << "doesn't exist and failed to create a new one:\n" << file.errorString();
            return;
        }
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { // this might be redundant
        qWarning() << "failed to read config file " << _configFileName << ":\n" << file.errorString();
        return;
    }

    QString tmpBuffer = "";
    QHash<QString, bool> savedConfigOptions = {
        {"window_width", false},
        {"window_height", false},
        {"base_directory", false},
        {"run_in_system_tray", false},
    };

    qDebug() << "Started reading config file...";
    QTextStream fileIn(&file);
    while (!fileIn.atEnd()) {
        auto line = fileIn.readLine();

        if (!line.contains("=")) {
            tmpBuffer += line + "\n";
            continue;
        }

        auto key = line.section('=', 0, 0).trimmed();
        auto rest = line.section('=', 1, 1);
        rest = rest.contains("#") ? " " + rest.right(rest.length() - rest.indexOf('#')) : "";

        // replace old values
        if (key == "window_width") {
            tmpBuffer += key + "=" + QString::number(_width) + rest + "\n";
            savedConfigOptions["window_width"] = true;
            continue;
        }
        else if (key == "window_height") {
            tmpBuffer += key + "=" + QString::number(_height) + rest + "\n";
            savedConfigOptions["window_height"] = true;
            continue;
        }
        else if (key == "base_directory") {
            tmpBuffer += key + "=" + _baseDirectory.toLocalFile() + rest + "\n";
            savedConfigOptions["base_directory"] = true;
            continue;
        }
        else if (key == "run_in_system_tray") {
            tmpBuffer += key + "=" + (_runInTray ? "true" : "false") + rest + "\n";
            savedConfigOptions["run_in_system_tray"] = true;
            continue;
        }
        else {
            tmpBuffer += line + "\n";
            continue;
        }
    }

    // if an option was set but the key doesn't exist in the original file
    for (auto it = savedConfigOptions.cbegin(); it != savedConfigOptions.cend(); it++) {
        if (!it.value()) {
            if (it.key() == "window_width") {
                tmpBuffer += it.key() + "=" + QString::number(_width) + "\n";
                savedConfigOptions["window_width"] = true;
                continue;
            }
            else if (it.key() == "window_height") {
                tmpBuffer += it.key() + "=" + QString::number(_height) + "\n";
                savedConfigOptions["window_height"] = true;
                continue;
            }
            else if (it.key() == "base_directory") {
                tmpBuffer += it.key() + "=" + _baseDirectory.toLocalFile() + "\n";
                savedConfigOptions["base_directory"] = true;
                continue;
            }
            else if (it.key() == "run_in_system_tray") {
                tmpBuffer += it.key() + "=" + (_runInTray ? "true" : "false") + "\n";
                savedConfigOptions["run_in_system_tray"] = true;
                continue;
            }
        }
    }

    file.close();

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        file.write(tmpBuffer.toUtf8());
    }
    else {
        qWarning() << "failed to update config file " << _configFileName << ":\n" << file.errorString();
        return;
    }

    qInfo() << "Updated configuration file successfully";
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
