#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QWidget>
#include <QString>
#include "../database/DatabaseManager.h"
#include "../controllers/PlayerController.h"
#include "../controllers/PlaylistController.h"
#include "../models/Playable.h"
#include "../controllers/RatingController.h"
#include "../controllers/UserController.h"

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
    void handleCreatePlaylistButton();
    void handleAddToPlaylistButton();
    void handleRemoveFromPlaylistButton();
    void handleRemovePlaylistButton();
    void handleRateButton();
    void handleUpgradePremiumButton();

private:
    Ui::mainwindow *ui;
    LoggedUser loggedUser;
    DatabaseManager *database;
    PlayerController playerController;
    PlaylistController playlistController;
    RatingController ratingController;
    UserController userController;
    int currentPlaylistIndex;

    void showPlaylistContentView(int playlistIndex);
    void loadSongsToTable();
    void showEmptyTableMessage(const QString& message);
    void resetPlayerState();
    void updateCurrentItemLabel(const std::shared_ptr<Playable>& item, bool isPlaying);
    void updateFavouriteButtonText(const std::shared_ptr<Playable>& item);
    void showSmartPlaylistContentView(int smartPlaylistIndex);
};

#endif // MAINWINDOW_H