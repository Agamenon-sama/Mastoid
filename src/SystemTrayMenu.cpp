#include "SystemTrayMenu.h"

SystemTrayMenu::SystemTrayMenu(QCoreApplication *app, bool runInTray) : _app(app) {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << "System tray menus are not supported on this system";
        return;
    }

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

    QObject::connect(_playAction, &QAction::triggered, this, [this] { if (_systemTrayMenu) emit play(); });
    QObject::connect(_pauseAction, &QAction::triggered, this, [this] { if (_systemTrayMenu) emit pause(); });
    QObject::connect(_soundAction, &QAction::triggered, this, [this] { if (_systemTrayMenu) emit soundToggle(); });
    QObject::connect(_restoreAction, &QAction::triggered, this, [this] { if (_systemTrayMenu) emit restoreWindow(); });
    QObject::connect(_quitAction, &QAction::triggered, app, &QCoreApplication::quit);

    if (!runInTray) {
        qDebug() << "Application is configured to NOT use sys tray";
        return;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    _systemTrayMenu = new QSystemTrayIcon(_app);
    _systemTrayMenu->setIcon(QIcon(":/icons/Mastoid.png").pixmap(32));
    _systemTrayMenu->setContextMenu(_menu);
    _systemTrayMenu->show();
}

void SystemTrayMenu::enable() {
    if (_systemTrayMenu) return;

    if (!QSystemTrayIcon::isSystemTrayAvailable()) return;

    QApplication::setQuitOnLastWindowClosed(false);

    _systemTrayMenu = new QSystemTrayIcon(_app);
    _systemTrayMenu->setIcon(QIcon(":/icons/Mastoid.png").pixmap(32));
    _systemTrayMenu->setContextMenu(_menu);
    _systemTrayMenu->show();
}

void SystemTrayMenu::disable() {
    if (!_systemTrayMenu) return;

    QApplication::setQuitOnLastWindowClosed(true);

    _systemTrayMenu->hide();
    delete _systemTrayMenu;
    _systemTrayMenu = nullptr;
}
