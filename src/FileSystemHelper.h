#ifndef FILESYSTEMHELPER_H
#define FILESYSTEMHELPER_H

#include <QObject>
#include <QUrl>
#include <QString>

class FileSystemHelper : public QObject {
    Q_OBJECT
public:
    explicit FileSystemHelper(QObject *parent = nullptr);

    Q_INVOKABLE QUrl findCoverUrl(QString searchFolder);

signals:

};

#endif // FILESYSTEMHELPER_H
