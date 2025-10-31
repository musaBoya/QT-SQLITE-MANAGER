#include "sql_window.h"
#include "database.h"
#include <iostream>
#include <QVBoxLayout>
#include <QFont>
#include <QLabel>
#include <QTableWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {

    setWindowTitle("SQL program");
    setGeometry(100, 100, 420, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGridLayout* initInputs = initializeInputs();
    mainLayout->addLayout(initInputs);

    // Grid buttons
    mainLayout->addLayout(initializeButtons());

    statusDisplay = new QLineEdit();
    mainLayout->addWidget(statusDisplay);
    mainLayout->addWidget(initializeTable());

    setLayout(mainLayout);
    refreshList();
}

QGridLayout* MainWindow::initializeButtons(){
    QGridLayout* gridLayout = new QGridLayout();

    // Buton definitions: (text, row, column)
    struct Button {
        QString text;
        int row, col;
    };

    Button buttons[] = {
        {SAVE_BUTTON, 0, 0}, {LIST_BUTTON, 0, 1}, {DELETE_BUTTON, 0, 2},
    };

    for (const auto& btn : buttons) {
        QPushButton* button = new QPushButton(btn.text);
        button->setMinimumHeight(60);
        QFont btnFont("Arial", 18, QFont::Bold);
        button->setFont(btnFont);
        if (btn.text == SAVE_BUTTON){
            connect(button, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
        } else if (btn.text == LIST_BUTTON){
            connect(button, &QPushButton::clicked, this, &MainWindow::onListClicked);
        } else if (btn.text == DELETE_BUTTON){
            connect(button, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
        }
        gridLayout->addWidget(button, btn.row, btn.col);
    }
    return gridLayout;
}

QTableWidget* MainWindow::initializeTable(){
    tableWidget = new QTableWidget(0, 4);
    QStringList headers;
    headers << "CompanyID" << "Name" << "Surname" << "Age";
    tableWidget->setHorizontalHeaderLabels(headers);
    return tableWidget;
}

QGridLayout* MainWindow::initializeInputs(){
    QGridLayout *inputGridLayout = new QGridLayout();

    // CompanyID row
    QLabel *IDLabel = new QLabel("CompanyID:");
    companyIDBox = new QLineEdit();
    companyIDBox->setMinimumHeight(35);

    inputGridLayout->addWidget(IDLabel, 0, 0); // row 0, column 0
    inputGridLayout->addWidget(companyIDBox, 0, 1);   // row 0, column 1

    // Name row
    QLabel *nameLabel = new QLabel("Name:");
    nameBox = new QLineEdit();
    nameBox->setMinimumHeight(35); // smaller height

    inputGridLayout->addWidget(nameLabel, 1, 0); // row 1, column 0
    inputGridLayout->addWidget(nameBox, 1, 1);   // row 1, column 1

    // Surname row
    QLabel *surnameLabel = new QLabel("Surname:");
    surnameBox = new QLineEdit();
    surnameBox->setMinimumHeight(35);

    inputGridLayout->addWidget(surnameLabel, 2, 0); // row 2, column 0
    inputGridLayout->addWidget(surnameBox, 2, 1);   // row 2, column 1


    QLabel *ageLabel = new QLabel("Age:");
    ageBox = new QLineEdit();
    ageBox->setMinimumHeight(35);

    inputGridLayout->addWidget(ageLabel, 3, 0); // row 3, column 0
    inputGridLayout->addWidget(ageBox, 3, 1);   // row 3, column 1
    return inputGridLayout;
}

struct MainWindow::ValidationRule {
    QString value;
    QString fieldName;
    int maxSize;
};

bool MainWindow::checkIsSaveValid(user input){

    std::vector<ValidationRule> rules = {
        {input.companyID, "companyID", 10},
        {input.name, "name", 15},
        {input.surname, "surname", 15},
        {input.age, "age", 3}
    };
    
    for (const auto& rule : rules) {
        if (rule.value.isEmpty()) {
            statusDisplay->setText(QString("Missing input %1!").arg(rule.fieldName));
            return false;
        }
        if (rule.value.size() > rule.maxSize) {
            statusDisplay->setText(QString("%1 size is too big! (max: %2)")
                                  .arg(rule.fieldName).arg(rule.maxSize));
            return false;
        }
    }
    return true;
}

void MainWindow::onSaveClicked() {
    user newUser{companyIDBox->text(), nameBox->text(), surnameBox->text(), ageBox->text()};
    
    if(!checkIsSaveValid(newUser)){
        return;
    }

    const QString retDB =Database::instance().insertUser(newUser.companyID, newUser.name, newUser.surname, newUser.age); 
    
    if (retDB == "DB_OK") {
        QMessageBox::information(this, "Success", "User saved!");
        refreshList();
    } else {
        QMessageBox::critical(this, "Error", "User is not saved!\nRetcode: " + retDB);
        return;
    }
    companyIDBox->clear();nameBox->clear();surnameBox->clear();ageBox->clear();
}

void MainWindow::onListClicked() {
    if (tableWidget->rowCount() == 0){
        statusDisplay->setText("Db is empty.");
    }
    refreshList();
}

void MainWindow::onDeleteClicked() {
    if (tableWidget->selectedRanges().empty()){
        statusDisplay->setText("Select an item to delete.");
        return;
    }

    auto slctedTopRow = tableWidget->selectedRanges().takeAt(0).topRow();
    auto bottomRow = tableWidget->selectedRanges().takeAt(0).bottomRow();
    auto itemCompanyId = tableWidget->takeItem(slctedTopRow,0)->text();

    QSqlQuery getuser = Database::instance().getUserByCompanyID(itemCompanyId);
    QString usersdata;
    if (getuser.next()) {
        usersdata = getuser.value(0).toString();
    }

    const QString retDB = Database::instance().deleteUser(usersdata.toInt());
    if (retDB == "DB_OK") {
        QMessageBox::information(this, "Success", "User deleted!");
        refreshList();
    } else {
        QMessageBox::critical(this, "Error", "User is not deleted!\nRetcode: " + retDB);
        return;
    }
    statusDisplay->setText("[Deleted] -|"+ getuser.value(1).toString() + "-"+ getuser.value(2).toString() +
                           "-"+ getuser.value(3).toString() + "-"+ getuser.value(4).toString() + "|-");
}

bool MainWindow::refreshList() {
    QSqlQuery userbuf = Database::instance().getAllUsers();
    tableWidget->clearContents();
    auto sizerow = tableWidget->rowCount();
    for(;sizerow > 0; --sizerow){
        tableWidget->removeRow(sizerow - 1);
    }
    while (userbuf.next()) {
        QString country = userbuf.value(2).toString();
        user newUser{userbuf.value(1).toString(), userbuf.value(2).toString(), userbuf.value(3).toString(), userbuf.value(4).toString()};

        int newRow = tableWidget->rowCount();
        tableWidget->insertRow(newRow);
        tableWidget->setItem(newRow, 0, new QTableWidgetItem(newUser.companyID));
        tableWidget->setItem(newRow, 1, new QTableWidgetItem(newUser.name));
        tableWidget->setItem(newRow, 2, new QTableWidgetItem(newUser.surname));
        tableWidget->setItem(newRow, 3, new QTableWidgetItem(newUser.age));
    }
}