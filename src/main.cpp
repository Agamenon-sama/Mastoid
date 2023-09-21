#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "FileSystemHelper.h"
#include "AppConfiguration.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("Mastoid");

    QCommandLineParser args;
    args.addHelpOption();

    QList<QCommandLineOption> options;
    options.append({{"f", "file"}, "Absolute path of the file to play on application start.", "filename"});
    args.addOptions(options);
    args.process(app);

    FileSystemHelper helper;
    AppConfiguration config(args);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/ui/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("FileSystemHelper", &helper);
    engine.rootContext()->setContextProperty("AppConfiguration", &config);

    engine.load(url);


    return app.exec();
}
