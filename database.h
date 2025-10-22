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

    // Veritabanı bağlantı yönetimi
    bool connect(const QString& dbPath = "myapp.db");
    void disconnect();
    bool isConnected() const;

    // Tablo oluşturma
    bool createTables();

    // CRUD işlemleri - User tablosu örneği
    bool insertUser(const QString& IDx, const QString& name, const QString& surname, const QString& age);
    bool updateUser(int id, const QString& name, const QString& age);
    bool deleteUser(int id);
    QSqlQuery getAllUsers();
    QSqlQuery getUserById(int id);
    QSqlQuery getUserByUsername(const QString& name);

    // Genel sorgu çalıştırma
    QSqlQuery executeQuery(const QString& query);
    bool executeNonQuery(const QString& query);

    // Transaction yönetimi
    bool beginTransaction();
    bool commit();
    bool rollback();

    // Hata yönetimi
    QString lastError() const;

private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase db;
    QString connectionName;
};

#endif // DATABASE_H
