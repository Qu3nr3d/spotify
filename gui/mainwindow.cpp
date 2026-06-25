#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <memory>
#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidget>
#include <QStringList>

mainwindow::mainwindow(const LoggedUser& user, DatabaseManager *database, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::mainwindow),
      loggedUser(user),
      database(database)
{
    ui->setupUi(this);

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

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    setWindowTitle("Zegrze on a Beat - " + loggedUser.name);

    ui->welcomeLabel->setText("Witaj, " + loggedUser.name + " " + loggedUser.surname);
    ui->accounttypeLabel->setText("Typ konta: " + loggedUser.accountType);

    ui->welcomeLabel->setMinimumWidth(250);
    ui->accounttypeLabel->setMinimumWidth(180);
    ui->accounttypeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ui->currentSongLabel->setText("Nie wybrano utworu");
    ui->playButton->setText("Play");

    playerController.setDatabase(database);
    playerController.setUserId(loggedUser.id);
    playlistController.setDatabase(database);
    playlistController.setUserId(loggedUser.id);
    playlistController.loadFavouritesFromDatabase();

    connect(ui->logoutButton, &QPushButton::clicked, this, [this]() {
        emit logoutRequested();
        this->close();
    });

    connect(ui->libraryButton, &QPushButton::clicked, this, [this]() {
        showLibraryView();
    });

    connect(ui->playlistsButton, &QPushButton::clicked, this, [this]() {
        showPlaylistsView();
    });

    connect(ui->favouritesButton, &QPushButton::clicked, this, [this]() {
        showFavouritesView();
    });

    connect(ui->historyButton, &QPushButton::clicked, this, [this]() {
        showHistoryView();
    });

    connect(ui->premiumButton, &QPushButton::clicked, this, [this]() {
        showPremiumView();
    });

    connect(ui->songsTableWidget, &QTableWidget::cellDoubleClicked,
            this, &mainwindow::handleSongDoubleClick);

    connect(ui->playButton, &QPushButton::clicked,
            this, &mainwindow::handlePlayButton);

    connect(ui->nextButton, &QPushButton::clicked,
            this, &mainwindow::handleNextButton);

    connect(ui->previousButton, &QPushButton::clicked,
            this, &mainwindow::handlePreviousButton);

    connect(ui->shuffleButton, &QPushButton::clicked,
            this, &mainwindow::handleShuffleButton);

    connect(ui->addFavouriteButton, &QPushButton::clicked,
        this, &mainwindow::handleAddFavouriteButton);

    showLibraryView();
}

void mainwindow::loadSongsToTable()
{
    if (database == nullptr) {
        showEmptyTableMessage("Brak polaczenia z baza danych.");
        playerController.clear();
        return;
    }

    QVector<SongRecord> songs = database->getAllSongs();

    playerController.setSongs(songs);

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

    if (songs.isEmpty()) {
        showEmptyTableMessage("Brak utworow w bibliotece.");
    }
}

void mainwindow::showEmptyTableMessage(const QString& message)
{
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(1);

    QTableWidgetItem *item = new QTableWidgetItem(message);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);

    ui->songsTableWidget->setItem(0, 0, item);
    ui->songsTableWidget->setSpan(0, 0, 1, ui->songsTableWidget->columnCount());
}

void mainwindow::resetPlayerState()
{
    playerController.clear();

    ui->playButton->setText("Play");
    ui->currentSongLabel->setText("Nie wybrano utworu");
}

void mainwindow::updateCurrentItemLabel(const std::shared_ptr<Playable>& item, bool isPlaying)
{
    if (item == nullptr) {
        ui->playButton->setText("Play");
        ui->currentSongLabel->setText("Nie wybrano utworu");
        return;
    }

    QString text;

    if (item->getType() == "Song") {
        text = item->getTitle() + " - " + item->getArtist();
    } else if (item->getType() == "PodcastEpisode") {
        text = item->getTitle() + " - " + item->getArtist();
    } else if (item->getType() == "Audiobook") {
        text = item->getTitle() + " - " + item->getArtist();
    } else {
        text = item->getTitle();
    }

    if (isPlaying) {
        ui->playButton->setText("Pause");
        ui->currentSongLabel->setText("Teraz odtwarzane: " + text);
    } else {
        ui->playButton->setText("Play");
        ui->currentSongLabel->setText("Pauza: " + text);
    }
}

void mainwindow::showLibraryView()
{
    ui->sectionTitleLabel->setText("Biblioteka");

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(5);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Tytul",
        "Artysta",
        "Album",
        "Czas",
        "Gatunek"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    loadSongsToTable();
}

void mainwindow::showPlaylistsView()
{
    ui->sectionTitleLabel->setText("Playlisty");

    resetPlayerState();

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(3);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Nazwa playlisty",
        "Liczba utworow",
        "Data utworzenia"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    showEmptyTableMessage("Brak utworzonych playlist.");
}

void mainwindow::showFavouritesView()
{
    ui->sectionTitleLabel->setText("Ulubione");

    resetPlayerState();

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(6);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Tytul",
        "Wykonawca",
        "Album / nazwa",
        "Czas",
        "Gatunek",
        "Typ"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    QVector<std::shared_ptr<Playable>> favouriteItems = playlistController.getFavouriteItems();

    playerController.setItems(favouriteItems);

    if (favouriteItems.isEmpty()) {
        showEmptyTableMessage("Brak ulubionych utworow.");
        return;
    }

    for (const std::shared_ptr<Playable>& item : favouriteItems) {
        if (item == nullptr) {
            continue;
        }

        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(item->getTitle()));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(item->getArtist()));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(item->getAlbum()));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(item->getDuration()));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(item->getGenre()));
        ui->songsTableWidget->setItem(row, 5, new QTableWidgetItem(item->getType()));
    }
}

void mainwindow::showHistoryView()
{
    ui->sectionTitleLabel->setText("Historia");

    resetPlayerState();

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(7);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Tytul",
        "Wykonawca",
        "Album / nazwa",
        "Czas",
        "Gatunek",
        "Typ",
        "Odtworzono"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    QVector<HistoryEntry> historyEntries = playerController.getHistoryEntries();

    if (historyEntries.isEmpty()) {
        showEmptyTableMessage("Historia odtwarzania jest pusta.");
        return;
    }

    for (const HistoryEntry& entry : historyEntries) {
        if (entry.item == nullptr) {
            continue;
        }

        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(entry.item->getTitle()));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(entry.item->getArtist()));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(entry.item->getAlbum()));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(entry.item->getDuration()));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(entry.item->getGenre()));
        ui->songsTableWidget->setItem(row, 5, new QTableWidgetItem(entry.item->getType()));
        ui->songsTableWidget->setItem(row, 6, new QTableWidgetItem(entry.playedAt));
    }
}

void mainwindow::showPremiumView()
{
    ui->sectionTitleLabel->setText("Premium");

    resetPlayerState();

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(2);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Funkcja",
        "Status"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    QString accountTypeLower = loggedUser.accountType.toLower();
    bool isPremium = accountTypeLower == "premium";

    QString status = isPremium ? "Aktywne" : "Dostepne po ulepszeniu konta";

    QStringList features = {
        "Brak reklam",
        "Tworzenie playlist",
        "Ulubione utwory",
        "Historia odtwarzania",
        "Tryb shuffle"
    };

    ui->songsTableWidget->setRowCount(features.size());

    for (int i = 0; i < features.size(); ++i) {
        ui->songsTableWidget->setItem(i, 0, new QTableWidgetItem(features[i]));
        ui->songsTableWidget->setItem(i, 1, new QTableWidgetItem(status));
    }
}

void mainwindow::handleSongDoubleClick(int row, int column)
{
    Q_UNUSED(column);

    std::shared_ptr<Playable> item;
    bool isPlaying = false;

    if (playerController.playAt(row, item, isPlaying)) {
        updateCurrentItemLabel(item, isPlaying);
    }
}

void mainwindow::handlePlayButton()
{
    std::shared_ptr<Playable> item;
    bool isPlaying = false;

    if (playerController.togglePlay(item, isPlaying)) {
        updateCurrentItemLabel(item, isPlaying);
    } else {
        ui->currentSongLabel->setText("Brak utworow do odtworzenia");
    }
}

void mainwindow::handleNextButton()
{
    std::shared_ptr<Playable> item;
    bool isPlaying = false;

    if (playerController.next(item, isPlaying)) {
        updateCurrentItemLabel(item, isPlaying);
    }
}

void mainwindow::handlePreviousButton()
{
    std::shared_ptr<Playable> item;
    bool isPlaying = false;

    if (playerController.previous(item, isPlaying)) {
        updateCurrentItemLabel(item, isPlaying);
    }
}

void mainwindow::handleShuffleButton()
{
    std::shared_ptr<Playable> item;
    bool isPlaying = false;

    if (playerController.shuffle(item, isPlaying)) {
        updateCurrentItemLabel(item, isPlaying);
    }
}

void mainwindow::handleAddFavouriteButton()
{
    std::shared_ptr<Playable> item;

    if (!playerController.getCurrentItem(item) || item == nullptr) {
        ui->currentSongLabel->setText("Najpierw wybierz utwor.");
        return;
    }

    if (playlistController.isFavourite(item->getId())) {
        ui->currentSongLabel->setText(
            "Ten element jest juz w ulubionych: " + item->getTitle()
        );
        return;
    }

    if (playlistController.addFavourite(item)) {
        ui->currentSongLabel->setText(
            "Dodano do ulubionych: " + item->getTitle() + " - " + item->getArtist()
        );
    } else {
        ui->currentSongLabel->setText("Nie udalo sie dodac do ulubionych.");
    }

    if (ui->sectionTitleLabel->text() == "Ulubione") {
        showFavouritesView();
    }
}

mainwindow::~mainwindow()
{
    delete ui;
}