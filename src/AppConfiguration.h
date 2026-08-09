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
    Q_PROPERTY(bool spectrumEnabled MEMBER _spectrumEnabled READ spectrumEnabled WRITE setSpectrumEnabled NOTIFY spectrumEnabledChanged FINAL)
    Q_PROPERTY(bool shaderEnabled MEMBER _shaderEnabled READ shaderEnabled WRITE setShaderEnabled NOTIFY shaderEnabledChanged FINAL)

public:
    explicit AppConfiguration(const QCommandLineParser &parser);

    Q_INVOKABLE QUrl getStartupFile() const;
    Q_INVOKABLE void save() const;

    int width() const;
    int height() const;
    QUrl baseDirectory() const;
    bool runInTray() const;
    bool spectrumEnabled() const;
    bool shaderEnabled() const;

    void setWidth(int width);
    void setHeight(int height);
    void setBaseDirectory(QUrl baseDirectory);
    void setRunInTray(bool run);
    void setSpectrumEnabled(bool enabled);
    void setShaderEnabled(bool enabled);

signals:
    void widthChanged();
    void heightChanged();
    void baseDirectoryChanged();
    void runInTrayChanged();
    void spectrumEnabledChanged();
    void shaderEnabledChanged();

private:
    QString _startupFileName;
    QString _configFileName;

    // configs
    int _width;
    int _height;
    QUrl _baseDirectory;
    bool _runInTray;
    bool _spectrumEnabled;
    bool _shaderEnabled;

    void _parseConfig();
};

#endif // APPCONFIGURATION_H
