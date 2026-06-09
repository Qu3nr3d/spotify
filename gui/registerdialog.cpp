#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QMessageBox>
#include <QPushButton>

RegisterDialog::RegisterDialog(DatabaseManager *database, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::RegisterDialog),
      database(database)
{
    ui->setupUi(this);

    connect(ui->registerButton, &QPushButton::clicked, this, [this]() {
        QString name = ui->nameLineEdit->text();
        QString surname = ui->surnameLineEdit->text();
        QString email = ui->emailLineEdit->text();
        QString password = ui->passwordLineEdit_2->text();

        if (name.isEmpty() || surname.isEmpty() || email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Błąd rejestracji", "Uzupełnij imię, nazwisko, email i hasło.");
            return;
        }

        bool success = this->database->registerUser(name, surname, email, password, "free");

        if (!success) {
            QMessageBox::warning(this, "Błąd rejestracji", "Nie udało się utworzyć konta. Możliwe, że taki email już istnieje.");
            return;
        }

        QMessageBox::information(this, "Rejestracja", "Konto zostało utworzone.");
        accept();
    });

    connect(ui->retreatButton, &QPushButton::clicked, this, [this]() {
        reject();
    });
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}