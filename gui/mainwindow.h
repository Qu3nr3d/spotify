#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QWidget>
#include <QString>
#include "../database/DatabaseManager.h"
#include "../controllers/PlayerController.h"
#include "../controllers/PlaylistController.h"
#include "../models/Playable.h"

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

private slots:
    void showLibraryView();
    void showPlaylistsView();
    void showFavouritesView();
    void showHistoryView();
    void showPremiumView();

    void handleSongDoubleClick(int row, int column);
    void handlePlayButton();
    void handleNextButton();
    void handlePreviousButton();
    void handleShuffleButton();
    void handleAddFavouriteButton();

private:
    Ui::mainwindow *ui;
    LoggedUser loggedUser;
    DatabaseManager *database;

    PlayerController playerController;
    PlaylistController playlistController;

    void loadSongsToTable();
    void showEmptyTableMessage(const QString& message);
    void resetPlayerState();
    void updateCurrentItemLabel(const std::shared_ptr<Playable>& item, bool isPlaying);
};

#endif // MAINWINDOW_H