#include "sql_window.h"
#include "database.h"
#include <QVBoxLayout>
#include <QFont>
#include <QLabel>
#include <QTableWidget>
#include <QMessageBox>
#include <QSqlRecord>

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
    refreshDBList();
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

    const bool ok = Database::instance().insertUser(newUser.companyID, newUser.name, newUser.surname, newUser.age);

    if (ok) {
        QMessageBox::information(this, "Success", "User saved!");
        refreshDBList();
    } else {
        QMessageBox::critical(this, "Error", "User is not saved!\n" + Database::instance().lastError());
        return;
    }
    companyIDBox->clear();nameBox->clear();surnameBox->clear();ageBox->clear();
}

void MainWindow::onListClicked() {
    if (tableWidget->rowCount() == 0){
        statusDisplay->setText("Db is empty.");
    }
    refreshDBList();
}

void MainWindow::onDeleteClicked() {
    const auto ranges = tableWidget->selectedRanges();
    if (ranges.empty()) {
        statusDisplay->setText("Select an item to delete.");
        return;
    }

    if (ranges.first().bottomRow() - ranges.first().topRow() > 0) {
        statusDisplay->setText("multiple selection will add");
        return;
    }

    const int selectedRow = ranges.first().topRow();
    QTableWidgetItem* item = tableWidget->takeItem(selectedRow, 0);
    const QString itemCompanyId = item->text();
    delete item;

    QSqlQuery getuser = Database::instance().getUserByCompanyID(itemCompanyId);
    QString userId;
    if (getuser.next()) {
        userId = getuser.value(0).toString();
    }

    const bool ok = Database::instance().deleteUser(userId.toInt());
    if (ok) {
        QMessageBox::information(this, "Success", "User deleted!");
        const QSqlRecord rec = getuser.record();
        statusDisplay->setText("[Deleted] -|" + getuser.value(rec.indexOf("companyID")).toString() + "-"
                               + getuser.value(rec.indexOf("name")).toString() + "-"
                               + getuser.value(rec.indexOf("surname")).toString() + "-"
                               + getuser.value(rec.indexOf("age")).toString() + "|-");
        refreshDBList();
    } else {
        QMessageBox::critical(this, "Error", "User is not deleted!\n" + Database::instance().lastError());
    }
}

void MainWindow::refreshDBList() {
    QSqlQuery userbuf = Database::instance().getAllUsers();
    tableWidget->clearContents();
    for (int sizerow = tableWidget->rowCount(); sizerow > 0; --sizerow) {
        tableWidget->removeRow(sizerow - 1);
    }
    while (userbuf.next()) {
        const QSqlRecord rec = userbuf.record();
        user newUser{
            userbuf.value(rec.indexOf("companyID")).toString(),
            userbuf.value(rec.indexOf("name")).toString(),
            userbuf.value(rec.indexOf("surname")).toString(),
            userbuf.value(rec.indexOf("age")).toString()
        };

        const int newRow = tableWidget->rowCount();
        tableWidget->insertRow(newRow);
        tableWidget->setItem(newRow, 0, new QTableWidgetItem(newUser.companyID));
        tableWidget->setItem(newRow, 1, new QTableWidgetItem(newUser.name));
        tableWidget->setItem(newRow, 2, new QTableWidgetItem(newUser.surname));
        tableWidget->setItem(newRow, 3, new QTableWidgetItem(newUser.age));
    }
}