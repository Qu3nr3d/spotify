#include <QApplication>

#include "gui/login.h"
#include "database/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager database;

    if (!database.openDatabase()) {
        return -1;
    }

    if (!database.createTables()) {
        return -1;
    }

    database.seedSongs();

    Login loginWindow(&database);
    loginWindow.show();

    return app.exec();
}