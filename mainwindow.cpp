#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Song* s1 = new Song("Numb", "Linkin Park", 185);
    Song* s2 = new Song("Believer", "Imagine Dragons", 204);
    Song* s3 = new Song("Shape of You", "Ed Sheeran", 240);

    library.addSong(s1);
    library.addSong(s2);
    library.addSong(s3);

    // dodaj do listy GUI
    for (Song* s : library.songs) {
        ui->SongListWidget->addItem(s->title + " - " + s->artist);
    }

    // przykładowa playlista
    Playlist* pl = new Playlist("Moja playlista");
    library.addPlaylist(pl);
    ui->PlayListListWidget->addItem(pl->name);

    connect(ui->AddSongButton, &QPushButton::clicked, this, [this]() {
        QString title = QInputDialog::getText(this, "Dodaj utwór", "Tytuł utworu:");
        QString artist = QInputDialog::getText(this, "Dodaj utwór", "Artysta:");
        if (!title.isEmpty() && !artist.isEmpty()) {
            Song* s = new Song(title, artist, 180);
            library.addSong(s);
            ui->SongListWidget->addItem(s->title + " - " + s->artist);
        }
    });

    connect(ui->AddPlaylistButton, &QPushButton::clicked, this, [this]() {
        QString name = QInputDialog::getText(this, "Dodaj playlistę", "Nazwa playlisty:");
        if (!name.isEmpty()) {
            Playlist* pl = new Playlist(name);
            library.addPlaylist(pl);
            ui->PlayListListWidget->addItem(pl->name);
        }
    });
    connect(ui->AddToPlaylistButton, &QPushButton::clicked, this, [this]() {
        int songIndex = ui->SongListWidget->currentRow();
        int playlistIndex = ui->PlayListListWidget->currentRow();
        if (songIndex >= 0 && playlistIndex >= 0) {
            Song* s = library.songs[songIndex];
            Playlist* pl = library.playlists[playlistIndex];
            pl->addSong(s);

            if (ui->PlayListListWidget->currentRow() == playlistIndex) {
                ui->PlaylistContentWidget->addItem(s->title + " - " + s->artist);
            }
        }
    });
    connect(ui->PlayListListWidget, &QListWidget::currentRowChanged, this, [this](int row) {
        ui->PlaylistContentWidget->clear();
        if (row >= 0 && row < library.playlists.size()) {
            Playlist* pl = library.playlists[row];
            for (Song* s : pl->songs)
                ui->PlaylistContentWidget->addItem(s->title + " - " + s->artist);
        }
    });
    connect(ui->PlayButton, &QPushButton::clicked, this, [this]() {
    int playlistIndex = ui->PlayListListWidget->currentRow();
    if (playlistIndex >= 0) {
        player.currentPlaylist = library.playlists[playlistIndex];
        player.currentIndex = 0;
        Song* s = player.getCurrentSong();
        if (s) ui->CurrentSongLabel->setText("Aktualnie odtwarzane: " + s->title + " - " + s->artist);
    }
});

    connect(ui->NextButton, &QPushButton::clicked, this, [this]() {
        if (player.currentPlaylist) {
            player.next();
            Song* s = player.getCurrentSong();
            if (s) ui->CurrentSongLabel->setText("Aktualnie odtwarzane: " + s->title + " - " + s->artist);
        }
    });

    connect(ui->PreviousButton, &QPushButton::clicked, this, [this]() {
        if (player.currentPlaylist) {
            player.previous();
            Song* s = player.getCurrentSong();
            if (s) ui->CurrentSongLabel->setText("Aktualnie odtwarzane: " + s->title + " - " + s->artist);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}