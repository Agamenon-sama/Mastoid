#ifndef APPCONFIGURATION_H
#define APPCONFIGURATION_H

#include <QObject>
#include <QCommandLineParser>
#include <QUrl>

class AppConfiguration final : public QObject {
    Q_OBJECT
    Q_PROPERTY(int width MEMBER _width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height MEMBER _height READ height WRITE setHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(QUrl baseDirectory MEMBER _baseDirectory READ baseDirectory WRITE setBaseDirectory NOTIFY baseDirectoryChanged FINAL)
    Q_PROPERTY(bool runInTray MEMBER _runInTray READ runInTray WRITE setRunInTray NOTIFY runInTrayChanged FINAL)
public:
    explicit AppConfiguration(const QCommandLineParser &parser);

    Q_INVOKABLE QUrl getStartupFile() const;

    int width() const;
    int height() const;
    QUrl baseDirectory() const;
    bool runInTray() const;

    void setWidth(int width);
    void setHeight(int height);
    void setBaseDirectory(QUrl baseDirectory);
    void setRunInTray(bool run);

signals:
    void widthChanged();
    void heightChanged();
    void baseDirectoryChanged();
    void runInTrayChanged();

private:
    QString _startupFileName;
    QString _configFileName;

    // configs
    int _width;
    int _height;
    QUrl _baseDirectory;
    bool _runInTray;

    void _parseConfig();
};

#endif // APPCONFIGURATION_H
