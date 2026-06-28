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