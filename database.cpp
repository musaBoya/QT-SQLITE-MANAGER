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
        qDebug() << "Could not open database:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connection successful:" << dbPath;
    return createTables();
}

void Database::disconnect() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed.";
    }
}

bool Database::isConnected() const {
    return db.isOpen();
}

bool Database::createTables() {
    QSqlQuery query(db);

    // Users table
    QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            companyID TEXT NOT NULL,
            name TEXT NOT NULL UNIQUE,
            surname TEXT NOT NULL,
            age TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createUsersTable)) {
        qDebug() << "Could not create Users table:" << query.lastError().text();
        return false;
    }

    // Other tables can be added here
    // for example: products, orders, etc.

    qDebug() << "Tables created successfully";
    return true;
}

QString Database::insertUser(const QString& companyID, const QString& name, const QString& surname, const QString& age)  {
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (companyID, name, surname, age) VALUES (:companyID, :name, :surname, :age)");
    query.bindValue(":companyID", companyID);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":age", age);

    if (!query.exec()) {
        qDebug() << "User cannot added:" << query.lastError().text();
        qDebug() << "User info:" << companyID << "-"<< name << "-"<< surname << "-"<< age;
        return query.lastError().text();
    }

    qDebug() << "User added:" << name;
    return "DB_OK";
}

bool Database::updateUser(int id, const QString& name, const QString& age) {
    QSqlQuery query(db);
    query.prepare("UPDATE users SET username = :username, email = :email WHERE id = :id");
    query.bindValue(":username", name);
    query.bindValue(":age", age);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Could not update user:" << query.lastError().text();
        return false;
    }

    qDebug() << "User updated: ID" << id;
    return true;
}

QString Database::deleteUser(int id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Could not delete user:" << query.lastError().text();
        return query.lastError().text();
    }

    qDebug() << "User deleted: ID" << id;
    return "DB_OK";
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
QSqlQuery Database::getUserByCompanyID(const QString companyID) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE companyID = :companyID");
    query.bindValue(":companyID", companyID);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
    }
    
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
        qDebug() << "The query could not be run:" << sqlQuery.lastError().text();
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
