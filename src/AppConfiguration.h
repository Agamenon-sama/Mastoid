#ifndef APPCONFIGURATION_H
#define APPCONFIGURATION_H

#include <QObject>
#include <QCommandLineParser>
#include <QUrl>

class AppConfiguration : public QObject {
    Q_OBJECT
    Q_PROPERTY(int width MEMBER _width NOTIFY widthChanged)
    Q_PROPERTY(int height MEMBER _height NOTIFY heightChanged)
    Q_PROPERTY(QUrl baseDirectory MEMBER _baseDirectory NOTIFY baseDirectoryChanged)
public:
    explicit AppConfiguration(const QCommandLineParser &parser);

    Q_INVOKABLE QUrl getStartupFile() const;

signals:
    // I'm only defining these signals to remove the warning from the QML engine
    // at no point in the app they should be emitted
    // not sure if I need to implement them but I'm not getting any errors for now
    void widthChanged();
    void heightChanged();
    void baseDirectoryChanged();

private:
    QString _startupFileName;
    QString _configFileName;

    // configs
    int _width;
    int _height;
    QUrl _baseDirectory;

    void _parseConfig();
};

#endif // APPCONFIGURATION_H
