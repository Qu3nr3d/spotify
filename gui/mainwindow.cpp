#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../models/Song.h"
#include "../models/Audiobook.h"
#include <memory>
#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidget>
#include <QStringList>
#include <QInputDialog>
#include <QLineEdit>
#include "../models/PodcastEpisode.h"

mainwindow::mainwindow(const LoggedUser& user, DatabaseManager *database, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::mainwindow),
      loggedUser(user),
      database(database),
      currentPlaylistIndex(-1)
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
    userController.setCurrentUser(loggedUser);

    ui->welcomeLabel->setMinimumWidth(250);
    ui->accounttypeLabel->setMinimumWidth(180);
    ui->accounttypeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ui->currentSongLabel->setText("Nie wybrano utworu");
    ui->playButton->setText("Play");

    ui->addFavouriteButton->setText("Dodaj do ulubionych");
    ui->rateButton->setText("Ocen utwor");
    ui->addToPlaylistButton->setText("Dodaj do playlisty");
    ui->removeFromPlaylistButton->setText("Usun z playlisty");
    ui->createPlaylistButton->setText("Utworz playliste");
    ui->removePlaylistButton->setText("Usun playliste");

    ui->upgradePremiumButton->setText("Aktywuj Premium");
    ui->upgradePremiumButton->hide();

    playerController.setDatabase(database);
    playerController.setUserId(loggedUser.id);
    playlistController.setDatabase(database);
    playlistController.setUserId(loggedUser.id);
    playlistController.loadFavouritesFromDatabase();
    playlistController.loadPlaylistsFromDatabase();
    playlistController.loadSmartPlaylistsFromDatabase();
    ratingController.setDatabase(database);
    ratingController.setUserId(loggedUser.id);

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

    connect(ui->createPlaylistButton, &QPushButton::clicked,
        this, &mainwindow::handleCreatePlaylistButton);

    connect(ui->addToPlaylistButton, &QPushButton::clicked,
            this, &mainwindow::handleAddToPlaylistButton);

    connect(ui->removeFromPlaylistButton, &QPushButton::clicked,
        this, &mainwindow::handleRemoveFromPlaylistButton);

    connect(ui->removePlaylistButton, &QPushButton::clicked,
        this, &mainwindow::handleRemovePlaylistButton);

    connect(ui->rateButton, &QPushButton::clicked,
        this, &mainwindow::handleRateButton);

    connect(ui->upgradePremiumButton, &QPushButton::clicked,
        this, &mainwindow::handleUpgradePremiumButton);

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
    QVector<AudiobookRecord> audiobooks = database->getAllAudiobooks();
    QVector<PodcastEpisodeRecord> podcastEpisodes = database->getAllPodcastEpisodes();

    QVector<std::shared_ptr<Playable>> libraryItems;

    ui->songsTableWidget->setRowCount(0);

    for (const SongRecord& song : songs) {
        std::shared_ptr<Playable> item = std::make_shared<Song>(
            song.id,
            song.title,
            song.artist,
            song.album,
            song.duration,
            song.genre
        );

        libraryItems.push_back(item);

        int userRating = ratingController.getUserRating(item);
        double averageRating = ratingController.getAverageRating(item);

        QString userRatingText = userRating > 0
                                     ? QString::number(userRating) + "/5"
                                     : "brak";

        QString averageRatingText = averageRating > 0.0
                                        ? QString::number(averageRating, 'f', 2) + "/5"
                                        : "brak";

        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(song.title));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(song.artist));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(song.album));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(song.duration));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(song.genre));
        ui->songsTableWidget->setItem(row, 5, new QTableWidgetItem("Song"));
        ui->songsTableWidget->setItem(row, 6, new QTableWidgetItem(userRatingText));
        ui->songsTableWidget->setItem(row, 7, new QTableWidgetItem(averageRatingText));
    }

    for (const AudiobookRecord& audiobook : audiobooks) {
        std::shared_ptr<Playable> item = std::make_shared<Audiobook>(
            audiobook.id,
            audiobook.title,
            audiobook.author,
            audiobook.narrator,
            audiobook.duration,
            audiobook.category
        );

        libraryItems.push_back(item);

        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(audiobook.title));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(audiobook.author));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(audiobook.narrator));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(audiobook.duration));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(audiobook.category));
        ui->songsTableWidget->setItem(row, 5, new QTableWidgetItem("Audiobook"));
        ui->songsTableWidget->setItem(row, 6, new QTableWidgetItem("nie dotyczy"));
        ui->songsTableWidget->setItem(row, 7, new QTableWidgetItem("nie dotyczy"));
    }

    for (const PodcastEpisodeRecord& episode : podcastEpisodes) {
        std::shared_ptr<Playable> item = std::make_shared<PodcastEpisode>(
            episode.id,
            episode.title,
            episode.host,
            episode.podcastName,
            episode.duration,
            episode.category
        );

        libraryItems.push_back(item);

        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(episode.title));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(episode.host));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(episode.podcastName));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(episode.duration));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(episode.category));
        ui->songsTableWidget->setItem(row, 5, new QTableWidgetItem("PodcastEpisode"));
        ui->songsTableWidget->setItem(row, 6, new QTableWidgetItem("nie dotyczy"));
        ui->songsTableWidget->setItem(row, 7, new QTableWidgetItem("nie dotyczy"));
    }

    playerController.setItems(libraryItems);

    if (libraryItems.isEmpty()) {
        showEmptyTableMessage("Biblioteka jest pusta.");
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
    ui->addFavouriteButton->setText("Dodaj do ulubionych");
    ui->currentSongLabel->setText("Nie wybrano utworu");
}

void mainwindow::updateCurrentItemLabel(const std::shared_ptr<Playable>& item, bool isPlaying)
{
    if (item == nullptr) {
        ui->playButton->setText("Play");
        ui->currentSongLabel->setText("Nie wybrano utworu");
        ui->addFavouriteButton->setText("Dodaj do ulubionych");
        return;
    }

    QString text = item->getTitle() + " - " + item->getArtist();

    if (item->getType() == "Song") {
        int userRating = ratingController.getUserRating(item);
        double averageRating = ratingController.getAverageRating(item);

        QString userRatingText = userRating > 0
                                     ? QString::number(userRating) + "/5"
                                     : "brak";

        QString averageRatingText = averageRating > 0.0
                                        ? QString::number(averageRating, 'f', 2) + "/5"
                                        : "brak";

        text += " | Twoja ocena: " + userRatingText;
        text += " | Srednia: " + averageRatingText;
    }

    if (isPlaying) {
        ui->playButton->setText("Pause");
        ui->currentSongLabel->setText("Teraz odtwarzane: " + text);
    } else {
        ui->playButton->setText("Play");
        ui->currentSongLabel->setText("Pauza: " + text);
    }

    updateFavouriteButtonText(item);
}

void mainwindow::updateFavouriteButtonText(const std::shared_ptr<Playable>& item)
{
    if (item == nullptr || item->getId() <= 0) {
        ui->addFavouriteButton->setText("Dodaj do ulubionych");
        return;
    }

    if (playlistController.isFavourite(item->getId())) {
        ui->addFavouriteButton->setText("Usun z ulubionych");
    } else {
        ui->addFavouriteButton->setText("Dodaj do ulubionych");
    }
}

void mainwindow::showLibraryView()
{
    currentPlaylistIndex = -1;

    ui->sectionTitleLabel->setText("Biblioteka");

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(8);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Tytul",
        "Autor / artysta",
        "Album / lektor",
        "Czas",
        "Gatunek / kategoria",
        "Typ",
        "Twoja ocena",
        "Srednia"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    loadSongsToTable();
}

void mainwindow::showPlaylistsView()
{
    currentPlaylistIndex = -1;

    ui->sectionTitleLabel->setText("Playlisty");

    resetPlayerState();

    playlistController.loadPlaylistsFromDatabase();

    QVector<PlaylistRecord> playlists = playlistController.getPlaylistRecords();
    QVector<SmartPlaylist> smartPlaylists;

    if (userController.canUseSmartPlaylists()) {
        playlistController.loadSmartPlaylistsFromDatabase();
        smartPlaylists = playlistController.getSmartPlaylists();
    }

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(5);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Typ",
        "ID",
        "Nazwa",
        "Liczba utworow",
        "Utworzono / filtr"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    if (playlists.isEmpty() && smartPlaylists.isEmpty()) {
        showEmptyTableMessage("Brak playlist.");
        return;
    }

    for (const PlaylistRecord& playlist : playlists) {
        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem("Zwykla"));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(playlist.id)));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(playlist.name));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(playlist.songsCount)));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem(playlist.createdAt));
    }

    for (const SmartPlaylist& smartPlaylist : smartPlaylists) {
        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem("Smart"));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem("-"));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(smartPlaylist.getName()));
        ui->songsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(smartPlaylist.size())));
        ui->songsTableWidget->setItem(row, 4, new QTableWidgetItem("Gatunek: " + smartPlaylist.getFilterGenre()));
    }
}

void mainwindow::showFavouritesView()
{
    currentPlaylistIndex = -1;

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
    currentPlaylistIndex = -1;

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
    currentPlaylistIndex = -1;

    ui->sectionTitleLabel->setText("Premium");

    resetPlayerState();

    ui->songsTableWidget->clear();
    ui->songsTableWidget->clearSpans();
    ui->songsTableWidget->setRowCount(0);
    ui->songsTableWidget->setColumnCount(3);

    ui->songsTableWidget->setHorizontalHeaderLabels({
        "Funkcja",
        "Free",
        "Twoje konto"
    });

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);

    bool premium = userController.isPremium();

    struct FeatureRow {
        QString feature;
        QString freeStatus;
        QString premiumStatus;
    };

    QVector<FeatureRow> features = {
        {"Shuffle", "Nie", premium ? "Tak" : "Nie"},
        {"SmartPlaylist", "Nie", premium ? "Tak" : "Nie"},
        {"Limit playlist", "3", premium ? "Bez limitu" : "3"},
        {"Limit ulubionych", "5", premium ? "Bez limitu" : "5"},
        {"Audiobooki", "Tak", "Tak"},
        {"Oceny utworow", "Tak", "Tak"}
    };

    for (const FeatureRow& feature : features) {
        int row = ui->songsTableWidget->rowCount();
        ui->songsTableWidget->insertRow(row);

        ui->songsTableWidget->setItem(row, 0, new QTableWidgetItem(feature.feature));
        ui->songsTableWidget->setItem(row, 1, new QTableWidgetItem(feature.freeStatus));
        ui->songsTableWidget->setItem(row, 2, new QTableWidgetItem(feature.premiumStatus));
    }

    ui->upgradePremiumButton->show();

    if (userController.isPremium()) {
        ui->upgradePremiumButton->setText("Premium aktywne");
        ui->upgradePremiumButton->setEnabled(false);
    } else {
        ui->upgradePremiumButton->setText("Aktywuj Premium");
        ui->upgradePremiumButton->setEnabled(true);
    }
}

void mainwindow::handleSongDoubleClick(int row, int column)
{
    Q_UNUSED(column);

    if (ui->sectionTitleLabel->text() == "Playlisty") {
        int normalPlaylistCount = playlistController.getPlaylistCount();

        if (row < normalPlaylistCount) {
            showPlaylistContentView(row);
            return;
        }

        if (!userController.canUseSmartPlaylists()) {
            ui->currentSongLabel->setText("SmartPlaylist sa dostepne tylko dla kont Premium.");
            return;
        }

        int smartPlaylistIndex = row - normalPlaylistCount;
        showSmartPlaylistContentView(smartPlaylistIndex);
        return;
    }

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
    if (!userController.canUseShuffle()) {
        ui->currentSongLabel->setText("Shuffle jest dostepny tylko dla kont Premium.");
        return;
    }

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
        ui->addFavouriteButton->setText("Dodaj do ulubionych");
        return;
    }

    bool isAlreadyFavourite = playlistController.isFavourite(item->getId());

    if (isAlreadyFavourite) {
        if (playlistController.removeFavourite(item->getId())) {
            bool isFavouriteView = ui->sectionTitleLabel->text() == "Ulubione";

            if (isFavouriteView) {
                showFavouritesView();
            }

            ui->currentSongLabel->setText(
                "Usunieto z ulubionych: " + item->getTitle() + " - " + item->getArtist()
            );

            ui->addFavouriteButton->setText("Dodaj do ulubionych");
        } else {
            ui->currentSongLabel->setText("Nie udalo sie usunac z ulubionych.");
        }

        return;
    }

    int currentFavouriteCount = playlistController.getFavouriteCount();

    if (!userController.canAddFavourite(currentFavouriteCount)) {
        ui->currentSongLabel->setText("Konto Free moze miec maksymalnie 5 ulubionych utworow.");
        return;
    }

    if (playlistController.addFavourite(item)) {
        ui->currentSongLabel->setText(
            "Dodano do ulubionych: " + item->getTitle() + " - " + item->getArtist()
        );

        ui->addFavouriteButton->setText("Usun z ulubionych");
    } else {
        ui->currentSongLabel->setText("Nie udalo sie dodac do ulubionych.");
    }

    if (ui->sectionTitleLabel->text() == "Ulubione") {
        showFavouritesView();
    }
}

void mainwindow::showPlaylistContentView(int playlistIndex)
{
    PlaylistRecord playlistRecord;

    if (!playlistController.getPlaylistRecordAt(playlistIndex, playlistRecord)) {
        currentPlaylistIndex = -1;
        ui->currentSongLabel->setText("Nie udalo sie otworzyc playlisty.");
        return;
    }

    currentPlaylistIndex = playlistIndex;

    ui->sectionTitleLabel->setText("Playlista: " + playlistRecord.name);

    resetPlayerState();

    QVector<std::shared_ptr<Playable>> items = playlistController.getPlaylistItems(playlistIndex);

    playerController.setItems(items);

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

    if (items.isEmpty()) {
        showEmptyTableMessage("Ta playlista jest pusta.");
        return;
    }

    for (const std::shared_ptr<Playable>& item : items) {
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

void mainwindow::handleCreatePlaylistButton()
{
    playlistController.loadPlaylistsFromDatabase();

    int currentPlaylistCount = playlistController.getPlaylistCount();

    if (!userController.canCreatePlaylist(currentPlaylistCount)) {
        ui->currentSongLabel->setText("Konto Free moze miec maksymalnie 3 playlisty.");
        return;
    }

    bool ok = false;

    QString name = QInputDialog::getText(
        this,
        "Nowa playlista",
        "Podaj nazwe playlisty:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || name.trimmed().isEmpty()) {
        return;
    }

    if (playlistController.createPlaylist(name.trimmed())) {
        ui->currentSongLabel->setText("Utworzono playliste: " + name.trimmed());

        if (ui->sectionTitleLabel->text() == "Playlisty") {
            showPlaylistsView();
        }
    } else {
        ui->currentSongLabel->setText("Nie udalo sie utworzyc playlisty.");
    }
}

void mainwindow::handleAddToPlaylistButton()
{
    std::shared_ptr<Playable> item;

    if (!playerController.getCurrentItem(item) || item == nullptr) {
        ui->currentSongLabel->setText("Najpierw wybierz utwor.");
        return;
    }

    if (item->getType() != "Song") {
        ui->currentSongLabel->setText("Do playlist mozna teraz dodawac tylko utwory.");
        return;
    }

    playlistController.loadPlaylistsFromDatabase();

    QVector<PlaylistRecord> playlists = playlistController.getPlaylistRecords();

    if (playlists.isEmpty()) {
        ui->currentSongLabel->setText("Najpierw utworz playliste.");
        return;
    }

    QStringList playlistNames;

    for (const PlaylistRecord& playlist : playlists) {
        playlistNames << playlist.name;
    }

    bool ok = false;

    QString selectedPlaylist = QInputDialog::getItem(
        this,
        "Dodaj do playlisty",
        "Wybierz playliste:",
        playlistNames,
        0,
        false,
        &ok
    );

    if (!ok || selectedPlaylist.isEmpty()) {
        return;
    }

    int playlistIndex = playlistNames.indexOf(selectedPlaylist);

    if (playlistIndex < 0) {
        ui->currentSongLabel->setText("Nie znaleziono playlisty.");
        return;
    }

    if (playlistController.addItemToPlaylist(playlistIndex, item)) {
        ui->currentSongLabel->setText(
            "Dodano do playlisty " + selectedPlaylist + ": "
            + item->getTitle() + " - " + item->getArtist()
        );

        if (ui->sectionTitleLabel->text() == "Playlisty") {
            showPlaylistsView();
        }
    } else {
        ui->currentSongLabel->setText("Ten utwor jest juz w tej playliscie albo nie udalo sie dodac.");
    }
}

void mainwindow::handleRemoveFromPlaylistButton()
{
    if (currentPlaylistIndex < 0) {
        ui->currentSongLabel->setText("Najpierw otworz konkretna playliste.");
        return;
    }

    int selectedRow = ui->songsTableWidget->currentRow();

    if (selectedRow < 0) {
        ui->currentSongLabel->setText("Najpierw zaznacz utwor w playliscie.");
        return;
    }

    QVector<std::shared_ptr<Playable>> items =
        playlistController.getPlaylistItems(currentPlaylistIndex);

    if (selectedRow >= items.size() || items[selectedRow] == nullptr) {
        ui->currentSongLabel->setText("Nie udalo sie znalezc wybranego utworu.");
        return;
    }

    std::shared_ptr<Playable> item = items[selectedRow];

    if (playlistController.removeItemFromPlaylist(currentPlaylistIndex, item->getId())) {
        ui->currentSongLabel->setText(
            "Usunieto z playlisty: " + item->getTitle() + " - " + item->getArtist()
        );

        showPlaylistContentView(currentPlaylistIndex);
    } else {
        ui->currentSongLabel->setText("Nie udalo sie usunac utworu z playlisty.");
    }
}

void mainwindow::handleRemovePlaylistButton()
{
    if (ui->sectionTitleLabel->text() != "Playlisty") {
        ui->currentSongLabel->setText("Wejdz w widok Playlisty i zaznacz zwykla playliste.");
        return;
    }

    int selectedRow = ui->songsTableWidget->currentRow();

    if (selectedRow < 0) {
        ui->currentSongLabel->setText("Najpierw zaznacz playliste do usuniecia.");
        return;
    }

    int normalPlaylistCount = playlistController.getPlaylistCount();

    if (selectedRow >= normalPlaylistCount) {
        ui->currentSongLabel->setText("Smart playlist nie mozna usunac recznie.");
        return;
    }

    PlaylistRecord playlistRecord;

    if (!playlistController.getPlaylistRecordAt(selectedRow, playlistRecord)) {
        ui->currentSongLabel->setText("Nie udalo sie znalezc playlisty.");
        return;
    }

    bool removed = playlistController.removePlaylist(selectedRow);

    if (removed) {
        ui->currentSongLabel->setText("Usunieto playliste: " + playlistRecord.name);
        currentPlaylistIndex = -1;
        showPlaylistsView();
    } else {
        ui->currentSongLabel->setText("Nie udalo sie usunac playlisty.");
    }
}

void mainwindow::showSmartPlaylistContentView(int smartPlaylistIndex)
{
    currentPlaylistIndex = -1;

    SmartPlaylist smartPlaylist;

    if (!playlistController.getSmartPlaylistAt(smartPlaylistIndex, smartPlaylist)) {
        ui->currentSongLabel->setText("Nie udalo sie otworzyc smart playlisty.");
        return;
    }

    ui->sectionTitleLabel->setText("Smart playlista: " + smartPlaylist.getName());

    resetPlayerState();

    QVector<std::shared_ptr<Playable>> items =
        playlistController.getSmartPlaylistItems(smartPlaylistIndex);

    playerController.setItems(items);

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

    if (items.isEmpty()) {
        showEmptyTableMessage("Ta smart playlista jest pusta.");
        return;
    }

    for (const std::shared_ptr<Playable>& item : items) {
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

void mainwindow::handleRateButton()
{
    std::shared_ptr<Playable> item;

    if (!playerController.getCurrentItem(item) || item == nullptr) {
        ui->currentSongLabel->setText("Najpierw wybierz utwor.");
        return;
    }

    if (!ratingController.canRateItem(item)) {
        ui->currentSongLabel->setText("Na razie mozna oceniac tylko utwory.");
        return;
    }

    int currentRating = ratingController.getUserRating(item);

    bool ok = false;

    int rating = QInputDialog::getInt(
        this,
        "Ocena utworu",
        "Podaj ocene od 1 do 5:",
        currentRating > 0 ? currentRating : 5,
        1,
        5,
        1,
        &ok
    );

    if (!ok) {
        return;
    }

    if (ratingController.rateItem(item, rating)) {
        double averageRating = ratingController.getAverageRating(item);

        ui->currentSongLabel->setText(
            "Oceniono: " + item->getTitle()
            + " - " + item->getArtist()
            + " | Twoja ocena: " + QString::number(rating) + "/5"
            + " | Srednia: " + QString::number(averageRating, 'f', 2) + "/5"
        );

        if (ui->sectionTitleLabel->text() == "Biblioteka") {
            int selectedRow = ui->songsTableWidget->currentRow();

            if (selectedRow >= 0 && ui->songsTableWidget->columnCount() >= 8) {
                ui->songsTableWidget->setItem(
                    selectedRow,
                    6,
                    new QTableWidgetItem(QString::number(rating) + "/5")
                );

                ui->songsTableWidget->setItem(
                    selectedRow,
                    7,
                    new QTableWidgetItem(QString::number(averageRating, 'f', 2) + "/5")
                );
            }
        }
    } else {
        ui->currentSongLabel->setText("Nie udalo sie zapisac oceny.");
    }
}

void mainwindow::handleUpgradePremiumButton()
{
    if (userController.isPremium()) {
        ui->currentSongLabel->setText("Masz juz konto Premium.");
        return;
    }

    if (database == nullptr) {
        ui->currentSongLabel->setText("Brak polaczenia z baza danych.");
        return;
    }

    bool success = database->updateUserAccountType(loggedUser.id, "premium");

    if (!success) {
        ui->currentSongLabel->setText("Nie udalo sie aktywowac Premium.");
        return;
    }

    loggedUser.accountType = "premium";
    userController.setCurrentUser(loggedUser);

    ui->accounttypeLabel->setText("Typ konta: premium");
    ui->currentSongLabel->setText("Aktywowano Premium. Od teraz masz dostep do funkcji Premium.");

    showPremiumView();
}

mainwindow::~mainwindow()
{
    delete ui;
}