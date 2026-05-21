//
// Created by konra on 19.05.2026.
//

#ifndef SPOTIFY_MAINWINDOW_H
#define SPOTIFY_MAINWINDOW_H

#include <QWidget>
#include "models.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    MusicLibrary library;  // <-- dodaj to
    Player player;
};


#endif //SPOTIFY_MAINWINDOW_H