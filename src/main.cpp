#include <QApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QStandardPaths>

#include "FileSystemHelper.h"
#include "AppConfiguration.h"
#include "SystemTrayMenu.h"

static QFile *logFile;

static void logMessage(QtMsgType t, const QMessageLogContext &ctx, const QString &msg) {
    QString type = "";
    switch (t) {
    case QtMsgType::QtFatalMsg:
        type = "Fatal  ";
        break;
    case QtMsgType::QtCriticalMsg:
        type = "Error  ";
        break;
    case QtMsgType::QtWarningMsg:
        type = "Warning";
        break;
    case QtMsgType::QtInfoMsg:
        type = "Info   ";
        break;
    case QtMsgType::QtDebugMsg:
        type = "Debug  ";
        break;
    default:
        break;
    }

    // create message
    QString message = QString("[mastoid] -- %1 -- %2 -- %3\n").arg(
        type, QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm::ss"), msg
    );

    // write to file
    if (logFile) {
        logFile->write(message.toStdString().c_str());
        logFile->flush();
    }

    // write to console
    if (t == QtMsgType::QtInfoMsg || t == QtMsgType::QtDebugMsg) {
        static QTextStream stream(stdout);
        stream << message;
        stream.flush();
    }
    else {
        static QTextStream stream(stderr);
        stream << message;
        stream.flush();
    }
}

int main(int argc, char *argv[])
{
    // setup logging system
    logFile = new QFile();
    logFile->setFileName(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/Mastoid/mastoid.log");
    logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    if (logFile->isOpen()) {
        qInstallMessageHandler(logMessage);
        qInfo() << "Currently logging to " << logFile->fileName();
    }
    else {
        auto filename = logFile->fileName();
        delete logFile;
        logFile = nullptr;
        qInstallMessageHandler(logMessage); // use custom logger but without writing to file
        qWarning() << "Can't open log file " << filename;
    }


    QApplication app(argc, argv);
    app.setApplicationName("Mastoid");

    QCommandLineParser args;
    args.addHelpOption();

    QList<QCommandLineOption> options;
    options.append({{"f", "file"}, "Absolute path of the file to play on application start.", "filename"});
    args.addOptions(options);
    args.process(app);

    FileSystemHelper helper;
    AppConfiguration config(args);
    SystemTrayMenu systemTray(&app);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/ui/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("FileSystemHelper", &helper);
    engine.rootContext()->setContextProperty("AppConfiguration", &config);
    engine.rootContext()->setContextProperty("SystemTrayMenu", &systemTray);

    engine.load(url);

    auto ret = app.exec();

    delete logFile;

    return ret;
}
