#include <QApplication>
#include "sql_window.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Start database
    if (!Database::instance().connect("myapp.db")) {
        qCritical() << "Database initialization failed!";
        return -1;
    }

    MainWindow myapp;
    myapp.show();

    return a.exec();
}
