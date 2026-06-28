#ifndef SPOTIFY_PLAYLISTCONTROLLER_H
#define SPOTIFY_PLAYLISTCONTROLLER_H

#include <memory>

#include "../database/DatabaseManager.h"
#include "../models/Playlist.h"
#include "../models/FavoritePlaylist.h"
#include "../models/Playable.h"
#include "../models/Song.h"
#include "../models/SmartPlaylist.h"

class PlaylistController
{
public:
    PlaylistController();

    void setDatabase(DatabaseManager *database);
    void setUserId(int userId);

    void loadFavouritesFromDatabase();
    void loadPlaylistsFromDatabase();

    bool addFavourite(const std::shared_ptr<Playable>& item);
    bool removeFavourite(int itemId);
    bool isFavourite(int itemId) const;

    QVector<std::shared_ptr<Playable>> getFavouriteItems() const;
    bool getFavouriteItemAt(int index, std::shared_ptr<Playable>& item) const;
    int getFavouriteCount() const;
    void clearFavourites();

    bool createPlaylist(const QString& name);
    bool removePlaylist(int playlistIndex);
    bool addItemToPlaylist(int playlistIndex, const std::shared_ptr<Playable>& item);
    bool removeItemFromPlaylist(int playlistIndex, int itemId);

    QVector<Playlist> getPlaylists() const;
    QVector<PlaylistRecord> getPlaylistRecords() const;
    QVector<std::shared_ptr<Playable>> getPlaylistItems(int playlistIndex) const;

    bool getPlaylistRecordAt(int index, PlaylistRecord& playlistRecord) const;

    int getPlaylistCount() const;
    void clearPlaylists();

    void loadSmartPlaylistsFromDatabase();

    QVector<SmartPlaylist> getSmartPlaylists() const;
    bool getSmartPlaylistAt(int index, SmartPlaylist& smartPlaylist) const;
    QVector<std::shared_ptr<Playable>> getSmartPlaylistItems(int smartPlaylistIndex) const;
    int getSmartPlaylistCount() const;


private:
    DatabaseManager *database;
    int userId;

    FavoritePlaylist favoritePlaylist;
    QVector<Playlist> playlists;
    QVector<PlaylistRecord> playlistRecords;

    bool isValidPlaylistIndex(int index) const;
    std::shared_ptr<Playable> createPlayableFromSongRecord(const SongRecord& record) const;
    QVector<SmartPlaylist> smartPlaylists;
};

#endif // SPOTIFY_PLAYLISTCONTROLLER_H