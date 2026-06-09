#ifndef SPOTIFY_LOGIN_H
#define SPOTIFY_LOGIN_H

#include <QWidget>

#include "../database/DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Login;
}
QT_END_NAMESPACE

class Login : public QWidget {
    Q_OBJECT

public:
    explicit Login(DatabaseManager *database, QWidget *parent = nullptr);
    ~Login() override;

private:
    Ui::Login *ui;
    DatabaseManager *database;
};

#endif // SPOTIFY_LOGIN_H