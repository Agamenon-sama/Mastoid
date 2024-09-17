#include "SystemTrayMenu.h"

SystemTrayMenu::SystemTrayMenu(QCoreApplication *app) {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << "System tray menus are not supported on this system";
        return;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    _playAction = new QAction("Play"); _playAction->setIcon(QIcon(":/icons/play.svg").pixmap(32));
    _pauseAction = new QAction("Pause"); _pauseAction->setIcon(QIcon(":/icons/pause.svg").pixmap(32));
    _soundAction = new QAction("Toggle Sound On/Off"); _soundAction->setIcon(QIcon(":/icons/speaker.svg").pixmap(32));
    _restoreAction = new QAction("Show Window");
    _quitAction = new QAction("Quit");

    _menu = new QMenu();
    _menu->addAction(_playAction);
    _menu->addAction(_pauseAction);
    _menu->addAction(_soundAction);
    _menu->addSeparator();
    _menu->addAction(_restoreAction);
    _menu->addAction(_quitAction);

    // QIcon icon(dataLocation + "/Mastoid.png");
    _systemTrayMenu = new QSystemTrayIcon(app);
    // _systemTrayMenu->setIcon(QIcon((dataLocation + "/Mastoid.png")).pixmap(32));
    _systemTrayMenu->setIcon(QIcon(":/icons/Mastoid.png").pixmap(32));
    _systemTrayMenu->setContextMenu(_menu);

    QObject::connect(_playAction, &QAction::triggered, this, [this] { emit play(); });
    QObject::connect(_pauseAction, &QAction::triggered, this, [this] { emit pause(); });
    QObject::connect(_soundAction, &QAction::triggered, this, [this] { emit soundToggle(); });
    QObject::connect(_restoreAction, &QAction::triggered, this, [this] { emit restoreWindow(); });
    QObject::connect(_quitAction, &QAction::triggered, app, &QCoreApplication::quit);

    _systemTrayMenu->show();
}
