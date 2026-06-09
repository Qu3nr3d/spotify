#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "../database/DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class mainwindow;
}
QT_END_NAMESPACE

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainwindow(const LoggedUser& user,
                    DatabaseManager *database,
                    QWidget *parent = nullptr);
    ~mainwindow() override;

    signals:
        void logoutRequested();

private:
    Ui::mainwindow *ui;
    LoggedUser loggedUser;
    DatabaseManager *database;

    void loadSongsToTable();
};

#endif // MAINWINDOW_H