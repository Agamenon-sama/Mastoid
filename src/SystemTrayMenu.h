#ifndef SYSTEMTRAYMENU_H
#define SYSTEMTRAYMENU_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "AppConfiguration.h"

class SystemTrayMenu : public QObject
{
    Q_OBJECT
public:
    SystemTrayMenu(QCoreApplication *app, const AppConfiguration &config);

private:
    QSystemTrayIcon *_systemTrayMenu;
    QMenu *_menu;
    QAction *_playAction;
    QAction *_pauseAction;
    QAction *_soundAction;
    QAction *_restoreAction;
    QAction *_quitAction;

signals:
    void play();
    void pause();
    void soundToggle();
    void restoreWindow();
};

#endif // SYSTEMTRAYMENU_H
