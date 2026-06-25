#include "PlaylistController.h"

PlaylistController::PlaylistController()
    : database(nullptr),
      userId(-1)
{
}

void PlaylistController::setDatabase(DatabaseManager *database)
{
    this->database = database;
}

void PlaylistController::setUserId(int userId)
{
    this->userId = userId;
}

void PlaylistController::loadFavouritesFromDatabase()
{
    favoritePlaylist.clear();

    if (database == nullptr || userId <= 0) {
        return;
    }

    QVector<SongRecord> songs = database->getUserFavourites(userId);

    for (const SongRecord& record : songs) {
        std::shared_ptr<Playable> item = std::make_shared<Song>(
            record.id,
            record.title,
            record.artist,
            record.album,
            record.duration,
            record.genre
        );

        favoritePlaylist.addItem(item);
    }
}

bool PlaylistController::addFavourite(const std::shared_ptr<Playable>& item)
{
    if (item == nullptr || item->getId() <= 0) {
        return false;
    }

    if (favoritePlaylist.isFavourite(item->getId())) {
        return false;
    }

    favoritePlaylist.addItem(item);

    if (database != nullptr && userId > 0 && item->getType() == "Song") {
        database->addFavourite(userId, item->getId());
    }

    return true;
}

bool PlaylistController::removeFavourite(int itemId)
{
    if (itemId <= 0) {
        return false;
    }

    if (!favoritePlaylist.isFavourite(itemId)) {
        return false;
    }

    favoritePlaylist.removeItem(itemId);

    if (database != nullptr && userId > 0) {
        database->removeFavourite(userId, itemId);
    }

    return true;
}

bool PlaylistController::isFavourite(int itemId) const
{
    if (itemId <= 0) {
        return false;
    }

    return favoritePlaylist.isFavourite(itemId);
}

QVector<std::shared_ptr<Playable>> PlaylistController::getFavouriteItems() const
{
    return favoritePlaylist.getItems();
}

bool PlaylistController::getFavouriteItemAt(int index, std::shared_ptr<Playable>& item) const
{
    return favoritePlaylist.getItemAt(index, item);
}

int PlaylistController::getFavouriteCount() const
{
    return favoritePlaylist.size();
}

void PlaylistController::clearFavourites()
{
    favoritePlaylist.clear();
}

void PlaylistController::createPlaylist(const QString& name)
{
    if (name.trimmed().isEmpty()) {
        return;
    }

    playlists.push_back(Playlist(name));
}

bool PlaylistController::addItemToPlaylist(int playlistIndex, const std::shared_ptr<Playable>& item)
{
    if (!isValidPlaylistIndex(playlistIndex) || item == nullptr || item->getId() <= 0) {
        return false;
    }

    if (playlists[playlistIndex].containsItem(item->getId())) {
        return false;
    }

    playlists[playlistIndex].addItem(item);
    return true;
}

bool PlaylistController::removeItemFromPlaylist(int playlistIndex, int itemId)
{
    if (!isValidPlaylistIndex(playlistIndex) || itemId <= 0) {
        return false;
    }

    if (!playlists[playlistIndex].containsItem(itemId)) {
        return false;
    }

    playlists[playlistIndex].removeItem(itemId);
    return true;
}

QVector<Playlist> PlaylistController::getPlaylists() const
{
    return playlists;
}

QVector<std::shared_ptr<Playable>> PlaylistController::getPlaylistItems(int playlistIndex) const
{
    if (!isValidPlaylistIndex(playlistIndex)) {
        return {};
    }

    return playlists[playlistIndex].getItems();
}

int PlaylistController::getPlaylistCount() const
{
    return playlists.size();
}

void PlaylistController::clearPlaylists()
{
    playlists.clear();
}

bool PlaylistController::isValidPlaylistIndex(int index) const
{
    return index >= 0 && index < playlists.size();
}