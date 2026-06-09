#include "login.h"
#include "ui_login.h"

#include "mainwindow.h"
#include "registerdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>

Login::Login(DatabaseManager *database, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Login),
      database(database)
{
    ui->setupUi(this);

    ui->registerLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->registerLabel->setOpenExternalLinks(false);

    connect(ui->registerLabel, &QLabel::linkActivated, this, [this]() {
        RegisterDialog registerDialog(this->database, this);
        registerDialog.exec();
    });

    connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
        QString email = ui->usernameLineEdit->text();
        QString password = ui->passwordLineEdit->text();

        if (email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Błąd logowania", "Podaj login i hasło.");
            return;
        }

        bool success = this->database->loginUser(email, password);

        if (!success) {
            QMessageBox::warning(this, "Błąd logowania", "Nieprawidłowy login lub hasło.");
            return;
        }

        LoggedUser user = this->database->getLoggedUser(email, password);

        if (user.id == -1) {
            QMessageBox::warning(this, "Błąd logowania", "Nie udało się pobrać danych użytkownika.");
            return;
        }

        auto *mainWindow = new mainwindow(user, this->database);
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);

        connect(mainWindow, &mainwindow::logoutRequested, this, [this]() {
            ui->passwordLineEdit->clear();
            this->show();
        });

        mainWindow->show();

        this->hide();
    });
}

Login::~Login()
{
    delete ui;
}