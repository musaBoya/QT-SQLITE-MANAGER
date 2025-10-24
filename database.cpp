#include "database.h"
#include <QSqlRecord>
#include <QFile>

Database::Database() : connectionName("main_connection") {
}

Database::~Database() {
    disconnect();
}

Database& Database::instance() {
    static Database instance;
    return instance;
}

bool Database::connect(const QString& dbPath) {
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(dbPath);
    }

    if (!db.open()) {
        qDebug() << "Veritabanı açılamadı:" << db.lastError().text();
        return false;
    }

    qDebug() << "Veritabanı bağlantısı başarılı:" << dbPath;
    return createTables();
}

void Database::disconnect() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Veritabanı bağlantısı kapatıldı";
    }
}

bool Database::isConnected() const {
    return db.isOpen();
}

bool Database::createTables() {
    QSqlQuery query(db);

    // Users tablosu
    QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            IDx TEXT NOT NULL,
            name TEXT NOT NULL UNIQUE,
            surname TEXT NOT NULL,
            age TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createUsersTable)) {
        qDebug() << "Users tablosu oluşturulamadı:" << query.lastError().text();
        return false;
    }

    // Diğer tablolar buraya eklenebilir
    // örneğin: products, orders, vb.

    qDebug() << "Tablolar başarıyla oluşturuldu";
    return true;
}

QString Database::insertUser(const QString& IDx, const QString& name, const QString& surname, const QString& age)  {
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (IDx, name, surname, age) VALUES (:IDx, :name, :surname, :age)");
    query.bindValue(":IDx", IDx);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":age", age);

    if (!query.exec()) {
        qDebug() << "Kullanıcı eklenemedi:" << query.lastError().text();
        return query.lastError().text();
    }

    qDebug() << "Kullanıcı eklendi:" << name;
    return "DB_OK";
}

bool Database::updateUser(int id, const QString& name, const QString& age) {
    QSqlQuery query(db);
    query.prepare("UPDATE users SET username = :username, email = :email WHERE id = :id");
    query.bindValue(":username", name);
    query.bindValue(":age", age);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Kullanıcı güncellenemedi:" << query.lastError().text();
        return false;
    }

    qDebug() << "Kullanıcı güncellendi: ID" << id;
    return true;
}

bool Database::deleteUser(int id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Kullanıcı silinemedi:" << query.lastError().text();
        return false;
    }

    qDebug() << "Kullanıcı silindi: ID" << id;
    return true;
}

QSqlQuery Database::getAllUsers() {
    QSqlQuery query(db);
    query.exec("SELECT * FROM users ORDER BY id DESC");
    return query;
}

QSqlQuery Database::getUserById(int id) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    return query;
}

QSqlQuery Database::getUserByUsername(const QString& username) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();
    return query;
}

QSqlQuery Database::executeQuery(const QString& query) {
    QSqlQuery sqlQuery(db);
    sqlQuery.exec(query);
    return sqlQuery;
}

bool Database::executeNonQuery(const QString& query) {
    QSqlQuery sqlQuery(db);
    if (!sqlQuery.exec(query)) {
        qDebug() << "Sorgu çalıştırılamadı:" << sqlQuery.lastError().text();
        return false;
    }
    return true;
}

bool Database::beginTransaction() {
    return db.transaction();
}

bool Database::commit() {
    return db.commit();
}

bool Database::rollback() {
    return db.rollback();
}

QString Database::lastError() const {
    return db.lastError().text();
}
