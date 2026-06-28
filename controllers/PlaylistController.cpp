#include "PlaylistController.h"

using namespace std;

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
        favoritePlaylist.addItem(createPlayableFromSongRecord(record));
    }
}

void PlaylistController::loadPlaylistsFromDatabase()
{
    playlists.clear();
    playlistRecords.clear();

    if (database == nullptr || userId <= 0) {
        return;
    }

    playlistRecords = database->getUserPlaylists(userId);

    for (const PlaylistRecord& record : playlistRecords) {
        Playlist playlist(record.id, record.name);
        playlists.push_back(playlist);
    }
}

bool PlaylistController::addFavourite(const shared_ptr<Playable>& item)
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

QVector<shared_ptr<Playable>> PlaylistController::getFavouriteItems() const
{
    return favoritePlaylist.getItems();
}

bool PlaylistController::getFavouriteItemAt(int index, shared_ptr<Playable>& item) const
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

bool PlaylistController::createPlaylist(const QString& name)
{
    if (name.trimmed().isEmpty()) {
        return false;
    }

    if (database != nullptr && userId > 0) {
        bool success = database->createPlaylist(userId, name.trimmed());

        if (success) {
            loadPlaylistsFromDatabase();
        }

        return success;
    }

    Playlist playlist(name.trimmed());
    playlists.push_back(playlist);

    PlaylistRecord record;
    record.id = playlist.getId();
    record.userId = userId;
    record.name = playlist.getName();
    record.createdAt = "";
    record.songsCount = 0;

    playlistRecords.push_back(record);

    return true;
}

bool PlaylistController::addItemToPlaylist(int playlistIndex, const shared_ptr<Playable>& item)
{
    if (!isValidPlaylistIndex(playlistIndex) || item == nullptr || item->getId() <= 0) {
        return false;
    }

    if (item->getType() != "Song") {
        return false;
    }

    if (database != nullptr && userId > 0) {
        int playlistId = playlists[playlistIndex].getId();

        QVector<SongRecord> currentSongs = database->getPlaylistSongs(playlistId);

        for (const SongRecord& song : currentSongs) {
            if (song.id == item->getId()) {
                return false;
            }
        }

        bool success = database->addSongToPlaylist(playlistId, item->getId());

        if (success) {
            loadPlaylistsFromDatabase();
        }

        return success;
    }

    if (playlists[playlistIndex].containsItem(item->getId())) {
        return false;
    }

    playlists[playlistIndex].addItem(item);

    if (playlistIndex >= 0 && playlistIndex < playlistRecords.size()) {
        playlistRecords[playlistIndex].songsCount = playlists[playlistIndex].size();
    }

    return true;
}

bool PlaylistController::removeItemFromPlaylist(int playlistIndex, int itemId)
{
    if (!isValidPlaylistIndex(playlistIndex) || itemId <= 0) {
        return false;
    }

    if (database != nullptr && userId > 0) {
        int playlistId = playlists[playlistIndex].getId();

        bool success = database->removeSongFromPlaylist(playlistId, itemId);

        if (success) {
            loadPlaylistsFromDatabase();
        }

        return success;
    }

    if (!playlists[playlistIndex].containsItem(itemId)) {
        return false;
    }

    playlists[playlistIndex].removeItem(itemId);

    if (playlistIndex >= 0 && playlistIndex < playlistRecords.size()) {
        playlistRecords[playlistIndex].songsCount = playlists[playlistIndex].size();
    }

    return true;
}

QVector<Playlist> PlaylistController::getPlaylists() const
{
    return playlists;
}

QVector<PlaylistRecord> PlaylistController::getPlaylistRecords() const
{
    return playlistRecords;
}

QVector<shared_ptr<Playable>> PlaylistController::getPlaylistItems(int playlistIndex) const
{
    QVector<shared_ptr<Playable>> items;

    if (!isValidPlaylistIndex(playlistIndex)) {
        return items;
    }

    if (database != nullptr && userId > 0) {
        int playlistId = playlists[playlistIndex].getId();
        QVector<SongRecord> songs = database->getPlaylistSongs(playlistId);

        for (const SongRecord& record : songs) {
            items.push_back(createPlayableFromSongRecord(record));
        }

        return items;
    }

    return playlists[playlistIndex].getItems();
}

bool PlaylistController::getPlaylistRecordAt(int index, PlaylistRecord& playlistRecord) const
{
    if (index < 0 || index >= playlistRecords.size()) {
        return false;
    }

    playlistRecord = playlistRecords[index];
    return true;
}

int PlaylistController::getPlaylistCount() const
{
    return playlists.size();
}

void PlaylistController::clearPlaylists()
{
    playlists.clear();
    playlistRecords.clear();
}

bool PlaylistController::isValidPlaylistIndex(int index) const
{
    return index >= 0 && index < playlists.size();
}

shared_ptr<Playable> PlaylistController::createPlayableFromSongRecord(const SongRecord& record) const
{
    return make_shared<Song>(
        record.id,
        record.title,
        record.artist,
        record.album,
        record.duration,
        record.genre
    );
}

bool PlaylistController::removePlaylist(int playlistIndex)
{
    if (!isValidPlaylistIndex(playlistIndex)) {
        return false;
    }

    if (database != nullptr && userId > 0) {
        int playlistId = playlists[playlistIndex].getId();

        bool success = database->deletePlaylist(playlistId);

        if (success) {
            loadPlaylistsFromDatabase();
        }

        return success;
    }

    playlists.removeAt(playlistIndex);

    if (playlistIndex >= 0 && playlistIndex < playlistRecords.size()) {
        playlistRecords.removeAt(playlistIndex);
    }

    return true;
}

void PlaylistController::loadSmartPlaylistsFromDatabase()
{
    smartPlaylists.clear();

    if (database == nullptr) {
        return;
    }

    QVector<SongRecord> songs = database->getAllSongs();
    QVector<shared_ptr<Playable>> libraryItems;

    for (const SongRecord& record : songs) {
        libraryItems.push_back(createPlayableFromSongRecord(record));
    }

    SmartPlaylist rockPlaylist("Smart Rock", "Rock");
    rockPlaylist.buildFromLibrary(libraryItems);
    smartPlaylists.push_back(rockPlaylist);

    SmartPlaylist popPlaylist("Smart Pop", "Pop");
    popPlaylist.buildFromLibrary(libraryItems);
    smartPlaylists.push_back(popPlaylist);

    SmartPlaylist hipHopPlaylist("Smart Hip-Hop", "Hip-Hop");
    hipHopPlaylist.buildFromLibrary(libraryItems);
    smartPlaylists.push_back(hipHopPlaylist);

    SmartPlaylist electronicPlaylist("Smart Electronic", "Electronic");
    electronicPlaylist.buildFromLibrary(libraryItems);
    smartPlaylists.push_back(electronicPlaylist);
}

QVector<SmartPlaylist> PlaylistController::getSmartPlaylists() const
{
    return smartPlaylists;
}

bool PlaylistController::getSmartPlaylistAt(int index, SmartPlaylist& smartPlaylist) const
{
    if (index < 0 || index >= smartPlaylists.size()) {
        return false;
    }

    smartPlaylist = smartPlaylists[index];
    return true;
}

QVector<shared_ptr<Playable>> PlaylistController::getSmartPlaylistItems(int smartPlaylistIndex) const
{
    if (smartPlaylistIndex < 0 || smartPlaylistIndex >= smartPlaylists.size()) {
        return {};
    }

    return smartPlaylists[smartPlaylistIndex].getItems();
}

int PlaylistController::getSmartPlaylistCount() const
{
    return smartPlaylists.size();
}