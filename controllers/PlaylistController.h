#ifndef SPOTIFY_PLAYLISTCONTROLLER_H
#define SPOTIFY_PLAYLISTCONTROLLER_H

#include <memory>

#include <QString>
#include <QVector>

#include "../database/DatabaseManager.h"
#include "../models/Playlist.h"
#include "../models/FavoritePlaylist.h"
#include "../models/Playable.h"
#include "../models/Song.h"

class PlaylistController
{
public:
    PlaylistController();

    void setDatabase(DatabaseManager *database);
    void setUserId(int userId);

    void loadFavouritesFromDatabase();

    bool addFavourite(const std::shared_ptr<Playable>& item);
    bool removeFavourite(int itemId);
    bool isFavourite(int itemId) const;

    QVector<std::shared_ptr<Playable>> getFavouriteItems() const;
    bool getFavouriteItemAt(int index, std::shared_ptr<Playable>& item) const;
    int getFavouriteCount() const;
    void clearFavourites();

    void createPlaylist(const QString& name);
    bool addItemToPlaylist(int playlistIndex, const std::shared_ptr<Playable>& item);
    bool removeItemFromPlaylist(int playlistIndex, int itemId);
    QVector<Playlist> getPlaylists() const;
    QVector<std::shared_ptr<Playable>> getPlaylistItems(int playlistIndex) const;
    int getPlaylistCount() const;
    void clearPlaylists();

private:
    DatabaseManager *database;
    int userId;

    FavoritePlaylist favoritePlaylist;
    QVector<Playlist> playlists;

    bool isValidPlaylistIndex(int index) const;
};

#endif // SPOTIFY_PLAYLISTCONTROLLER_H