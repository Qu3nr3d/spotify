#ifndef SPOTIFY_REGISTERDIALOG_H
#define SPOTIFY_REGISTERDIALOG_H

#include <QDialog>

#include "../database/DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class RegisterDialog;
}
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(DatabaseManager *database, QWidget *parent = nullptr);
    ~RegisterDialog() override;

private:
    Ui::RegisterDialog *ui;
    DatabaseManager *database;
};

#endif // SPOTIFY_REGISTERDIALOG_H