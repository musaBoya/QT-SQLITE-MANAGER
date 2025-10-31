#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDebug>

class Database {
public:
    static Database& instance();

    // Database connection management
    bool connect(const QString& dbPath = "myapp.db");
    void disconnect();
    bool isConnected() const;

    // Table creation
    bool createTables();

    // CRUD operations - User table example
    QString insertUser(const QString& companyID, const QString& name, const QString& surname, const QString& age);
    bool updateUser(int id, const QString& name, const QString& age);
    QString deleteUser(int id);
    QSqlQuery getAllUsers();
    QSqlQuery getUserById(int id);
    QSqlQuery getUserByCompanyID(const QString companyID);
    QSqlQuery getUserByUsername(const QString& name);

    // Run a general query
    QSqlQuery executeQuery(const QString& query);
    bool executeNonQuery(const QString& query);

    // Transaction management
    bool beginTransaction();
    bool commit();
    bool rollback();

    // Error management
    QString lastError() const;

private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase db;
    QString connectionName;
};

struct user{
    QString companyID;
    QString name;
    QString surname;
    QString age;
};

#endif // DATABASE_H
