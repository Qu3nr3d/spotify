#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>

mainwindow::mainwindow(const LoggedUser& user, DatabaseManager *database, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::mainwindow),
      loggedUser(user),
      database(database)
{
    ui->setupUi(this);

    ui->songsTableWidget->setColumnCount(5);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Tytuł",
        "Artysta",
        "Album",
        "Czas",
        "Gatunek"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    this->setStyleSheet(R"(
    QWidget {
        background-color: #121212;
        color: #ffffff;
        font-family: Segoe UI;
        font-size: 14px;
    }

    QFrame {
        background-color: #181818;
        border: 1px solid #2a2a2a;
        border-radius: 10px;
    }

    #topBarFrame {
        background-color: #181818;
        border: 1px solid #2a2a2a;
        border-radius: 10px;
    }

    #contentFrame {
        background-color: #121212;
        border: none;
    }

    #sidebarFrame {
        background-color: #181818;
        border: 1px solid #2a2a2a;
        border-radius: 12px;
    }

    #mainContentFrame {
        background-color: #181818;
        border: 1px solid #2a2a2a;
        border-radius: 12px;
    }

    #playerBarFrame {
        background-color: #181818;
        border: 1px solid #2a2a2a;
        border-radius: 12px;
    }

    QLabel {
        background-color: transparent;
        border: none;
        color: #ffffff;
    }

    #welcomeLabel {
        color: #00ff66;
        font-size: 16px;
        font-weight: bold;
    }

    #accounttypeLabel {
    color: #00ff66;
    font-size: 15px;
    font-weight: bold;
    }

    #sectionTitleLabel {
        color: #00ff66;
        font-size: 22px;
        font-weight: bold;
        padding: 8px;
    }

    #currentSongLabel {
        color: #b3b3b3;
        font-size: 13px;
    }

    QPushButton {
        background-color: #242424;
        color: #ffffff;
        border: 1px solid #333333;
        border-radius: 8px;
        padding: 8px 14px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #00cc55;
        color: #000000;
        border: 1px solid #00ff66;
    }

    QPushButton:pressed {
        background-color: #009944;
        color: #000000;
    }

    #logoutButton {
        background-color: #00ff66;
        color: #000000;
        border-radius: 8px;
        font-weight: bold;
    }

    #logoutButton:hover {
        background-color: #1aff7a;
    }

    #libraryButton,
    #playlistsButton,
    #favouritesButton,
    #historyButton,
    #premiumButton {
        text-align: left;
        padding-left: 18px;
        background-color: #202020;
    }

    #libraryButton:hover,
    #playlistsButton:hover,
    #favouritesButton:hover,
    #historyButton:hover,
    #premiumButton:hover {
        background-color: #00ff66;
        color: #000000;
    }

    #playButton {
        background-color: #00ff66;
        color: #000000;
        border-radius: 16px;
        min-width: 80px;
    }

    #playButton:hover {
        background-color: #1aff7a;
    }

    QTableWidget {
        background-color: #121212;
        color: #ffffff;
        gridline-color: #2a2a2a;
        border: none;
        border-radius: 8px;
        selection-background-color: #00ff66;
        selection-color: #000000;
    }

    QHeaderView::section {
        background-color: #202020;
        color: #00ff66;
        border: none;
        padding: 8px;
        font-weight: bold;
    }

    QTableWidget::item {
        padding: 6px;
        border: none;
    }

    QTableWidget::item:hover {
        background-color: #1f1f1f;
    }

    QScrollBar:vertical {
        background-color: #121212;
        width: 10px;
        margin: 0px;
    }

    QScrollBar::handle:vertical {
        background-color: #00ff66;
        border-radius: 5px;
        min-height: 20px;
    }

    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0px;
    }
)");

    ui->verticalLayout->setContentsMargins(12, 12, 12, 12);
    ui->verticalLayout->setSpacing(10);

    ui->verticalLayout_2->setContentsMargins(12, 12, 12, 12);
    ui->verticalLayout_2->setSpacing(10);

    ui->verticalLayout_3->setContentsMargins(16, 16, 16, 16);
    ui->verticalLayout_3->setSpacing(10);

    ui->horizontalLayout_4->setContentsMargins(16, 10, 16, 10);
    ui->horizontalLayout_4->setSpacing(12);

    ui->songsTableWidget->setColumnCount(5);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Tytuł",
        "Artysta",
        "Album",
        "Czas",
        "Gatunek"
    });

    ui->songsTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadSongsToTable();

    setWindowTitle("Zegrze on a Beat - " + loggedUser.name);

    ui->welcomeLabel->setText("Witaj, " + loggedUser.name + " " + loggedUser.surname);
    ui->accounttypeLabel->setText("Typ konta: " + loggedUser.accountType);

    ui->welcomeLabel->setMinimumWidth(250);
    ui->accounttypeLabel->setMinimumWidth(180);

    ui->accounttypeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    connect(ui->logoutButton, &QPushButton::clicked, this, [this]() {
        emit logoutRequested();
        this->close();
    });
    connect(ui->libraryButton, &QPushButton::clicked, this, [this]() {
    ui->sectionTitleLabel->setText("Biblioteka");
    });
    connect(ui->favouritesButton, &QPushButton::clicked, this, [this]() {
    ui->sectionTitleLabel->setText("Ulubione");
    });
    connect(ui->historyButton, &QPushButton::clicked, this, [this]() {
    ui->sectionTitleLabel->setText("Historia");
    });
}

void mainwindow::loadSongsToTable()
{
    QVector<SongRecord> songs = database->getAllSongs();

    ui->songsTableWidget->setRowCount(0);

    for (const SongRecord& song : songs) {
        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(song.title));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(song.artist));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(song.album));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(song.duration));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(song.genre));
    }
}

mainwindow::~mainwindow()
{
    delete ui;
}