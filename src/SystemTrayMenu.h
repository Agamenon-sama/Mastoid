#ifndef SYSTEMTRAYMENU_H
#define SYSTEMTRAYMENU_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class SystemTrayMenu : public QObject
{
    Q_OBJECT
public:
    SystemTrayMenu(QCoreApplication *app, bool runInTray);

private:
    QCoreApplication *_app;
    QSystemTrayIcon *_systemTrayMenu = nullptr;
    QMenu *_menu = nullptr;
    QAction *_playAction = nullptr;
    QAction *_pauseAction = nullptr;
    QAction *_soundAction = nullptr;
    QAction *_restoreAction = nullptr;
    QAction *_quitAction = nullptr;

signals:
    void play();
    void pause();
    void soundToggle();
    void restoreWindow();

public slots:
    void enable();
    void disable();
};

#endif // SYSTEMTRAYMENU_H
