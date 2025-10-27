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

    setWindowTitle("SQL programi");
    setGeometry(100, 100, 420, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGridLayout* initInputs = initializeInputs();
    mainLayout->addLayout(initInputs);

    // Grid butonları
    QGridLayout* gridLayout = new QGridLayout();

    // Buton tanımları: (metni, satır, sütun)
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
    mainLayout->addLayout(gridLayout);

    statusDisplay = new QLineEdit();
    mainLayout->addWidget(statusDisplay);
    mainLayout->addWidget(initializeTable());

    setLayout(mainLayout);
}

QTableWidget* MainWindow::initializeTable(){
    tableWidget = new QTableWidget(0, 4);
    QStringList headers;
    headers << "ID" << "İsim" << "Soyisim" << "Yaş";
    tableWidget->setHorizontalHeaderLabels(headers);
    return tableWidget;
}

QGridLayout* MainWindow::initializeInputs(){
    QGridLayout *inputGridLayout = new QGridLayout();

    // Soyisim satırı
    QLabel *IDLabel = new QLabel("ID:");
    IDBox = new QLineEdit();
    IDBox->setMinimumHeight(35);

    inputGridLayout->addWidget(IDLabel, 0, 0); // satır 0, sütun 0
    inputGridLayout->addWidget(IDBox, 0, 1);   // satır 0, sütun 1

    // İsim satırı
    QLabel *nameLabel = new QLabel("İsim:");
    nameBox = new QLineEdit();
    nameBox->setMinimumHeight(35); // Daha küçük yükseklik

    inputGridLayout->addWidget(nameLabel, 1, 0); // satır 1, sütun 0
    inputGridLayout->addWidget(nameBox, 1, 1);   // satır 1, sütun 1

    // Soyisim satırı
    QLabel *surnameLabel = new QLabel("Soyisim:");
    surnameBox = new QLineEdit();
    surnameBox->setMinimumHeight(35);

    inputGridLayout->addWidget(surnameLabel, 2, 0); // satır 2, sütun 0
    inputGridLayout->addWidget(surnameBox, 2, 1);   // satır 2, sütun 1


    QLabel *ageLabel = new QLabel("Yaş:");
    ageBox = new QLineEdit();
    ageBox->setMinimumHeight(35);

    inputGridLayout->addWidget(ageLabel, 3, 0); // satır 3, sütun 0
    inputGridLayout->addWidget(ageBox, 3, 1);   // satır 3, sütun 1
    return inputGridLayout;
}

bool MainWindow::checkIsSaveValid(user input){

    if (input.idx.isEmpty()){
        statusDisplay->setText("idx empty!");
        return false;
    }
    if (input.name.isEmpty()){
        statusDisplay->setText("name empty!");
        return false;
    }
    if (input.surname.isEmpty()){
        statusDisplay->setText("surname empty!");
        return false;
    }
    if (input.age.isEmpty()){
        statusDisplay->setText("age empty!");
        return false;
    }
    return true;
}

void MainWindow::onSaveClicked() {
    user newUser{IDBox->text(), nameBox->text(), surnameBox->text(), ageBox->text()};
    if(!checkIsSaveValid(newUser)){
        return;
    }

    if (QString retDB = Database::instance().insertUser(newUser.idx, newUser.name, newUser.surname, newUser.age);
                retDB == "DB_OK") {
            QMessageBox::information(this, "Başarılı", "Kullanıcı kaydedildi!");
            //loadUsers(); // Listeyi güncelle
        } else {
            QMessageBox::critical(this, "Hata", "Kullanıcı kaydedilemedi!\nRetcode: " + retDB);
            return;
        }

    int newRow = tableWidget->rowCount();
    tableWidget->insertRow(newRow);

    tableWidget->setItem(newRow, 0, new QTableWidgetItem(newUser.idx));
    tableWidget->setItem(newRow, 1, new QTableWidgetItem(newUser.name));
    tableWidget->setItem(newRow, 2, new QTableWidgetItem(newUser.surname));
    tableWidget->setItem(newRow, 3, new QTableWidgetItem(newUser.age));

    tableWidget->setAlternatingRowColors(true); // Alternatif satır renkleri
    //tableWidget->resizeColumnsToContents(); // Sütun genişliklerini içeriğe göre ayarla


    QString inputText = newUser.idx + "\n- " + newUser.name + "\n- " +
                        newUser.surname + "\n- " + newUser.age;
    statusDisplay->setText(inputText);
}
void MainWindow::onListClicked() {
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
        tableWidget->setItem(newRow, 0, new QTableWidgetItem(newUser.idx));
        tableWidget->setItem(newRow, 1, new QTableWidgetItem(newUser.name));
        tableWidget->setItem(newRow, 2, new QTableWidgetItem(newUser.surname));
        tableWidget->setItem(newRow, 3, new QTableWidgetItem(newUser.age));
    }
}
void MainWindow::onDeleteClicked() {
    auto inputText = statusDisplay->text().toStdString();
    statusDisplay->setText("Delete button comming soon!");
}
