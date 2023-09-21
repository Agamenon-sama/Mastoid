#ifndef APPCONFIGURATION_H
#define APPCONFIGURATION_H

#include <QObject>
#include <QCommandLineParser>
#include <QUrl>

class AppConfiguration : public QObject {
    Q_OBJECT
public:
    explicit AppConfiguration(const QCommandLineParser &parser);

    Q_INVOKABLE QUrl getStartupFile() const;

signals:

private:
    QString _startupFileName;
};

#endif // APPCONFIGURATION_H
