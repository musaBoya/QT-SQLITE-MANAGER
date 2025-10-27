#ifndef SQL_WINDOW_H
#define SQL_WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QGridLayout>
#include "database.h"

constexpr const char* SAVE_BUTTON   = "Save";
constexpr const char* LIST_BUTTON   = "List";
constexpr const char* DELETE_BUTTON = "Delete";

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private:
    QLineEdit *IDBox;
    QLineEdit *nameBox;
    QLineEdit *surnameBox;
    QLineEdit *ageBox;
    QLineEdit *statusDisplay;
    QTableWidget *tableWidget;
    QGridLayout* initializeInputs();
    void onSaveClicked();
    bool checkIsSaveValid(user input);
    void onListClicked();
    void onDeleteClicked();
    QTableWidget* initializeTable();
};

#endif // SQL_WINDOW_H
