#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "FileSystemHelper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    FileSystemHelper helper;

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/ui/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("FileSystemHelper", &helper);

    engine.load(url);


    return app.exec();
}
