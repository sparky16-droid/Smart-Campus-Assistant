#include <QApplication>
#include <QDebug>
#include <QDir>
#include "database/DatabaseManager.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SmartCampusAssistant");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("PBL College Project");

    qDebug() << "==================================================";
    qDebug() << "   Smart Campus Assistant - PBL C++ Application   ";
    qDebug() << "==================================================";

    // Initialize Database
    QString dbPath = "smart_campus.db";
    if (!DatabaseManager::instance().initialize(dbPath)) {
        qCritical() << "Error: Could not initialize SQLite database at" << dbPath;
        return 1;
    }
    qDebug() << "Database initialized and verified successfully.";

    MainWindow window;
    window.show();

    return app.exec();
}
