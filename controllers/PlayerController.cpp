#include "PlayerController.h"

using namespace std;

PlayerController::PlayerController()
    : database(nullptr),
      userId(-1){}

void PlayerController::setItems(const QVector<shared_ptr<Playable>>& items)
{
    player.setQueue(items);
}

void PlayerController::setSongs(const QVector<SongRecord>& songs)
{
    QVector<shared_ptr<Playable>> items;

    for (const SongRecord& record : songs) {
        items.push_back(make_shared<Song>(
            record.id,
            record.title,
            record.artist,
            record.album,
            record.duration,
            record.genre
        ));
    }
    player.setQueue(items);
}

void PlayerController::clear()
{
    player.clear();
}

void PlayerController::setDatabase(DatabaseManager *database)
{
    this->database = database;
}

void PlayerController::setUserId(int userId)
{
    this->userId = userId;
}

bool PlayerController::playAt(int index,
                              shared_ptr<Playable>& item,
                              bool& isPlaying)
{
    if (!player.playAt(index, item)) {
        return false;
    }
    isPlaying = player.isPlaying();
    saveHistory(item);
    return true;
}

bool PlayerController::togglePlay(shared_ptr<Playable>& item,
                                  bool& isPlaying)
{
    if (!player.togglePlay(item)) {
        return false;
    }
    isPlaying = player.isPlaying();
    if (isPlaying) {
        saveHistory(item);
    }
    return true;
}

bool PlayerController::next(shared_ptr<Playable>& item,
                            bool& isPlaying)
{
    if (!player.next(item)) {
        return false;
    }

    isPlaying = player.isPlaying();
    saveHistory(item);

    return true;
}

bool PlayerController::previous(shared_ptr<Playable>& item,
                                bool& isPlaying)
{
    if (!player.previous(item)) {
        return false;
    }

    isPlaying = player.isPlaying();
    saveHistory(item);

    return true;
}

bool PlayerController::shuffle(shared_ptr<Playable>& item,
                               bool& isPlaying)
{
    if (!player.shuffle(item)) {
        return false;
    }

    isPlaying = player.isPlaying();
    saveHistory(item);

    return true;
}

bool PlayerController::getCurrentItem(shared_ptr<Playable>& item) const
{
    return player.getCurrentItem(item);
}

bool PlayerController::isPlaying() const
{
    return player.isPlaying();
}

QVector<HistoryEntry> PlayerController::getHistoryEntries() const
{
    return history.getEntries();
}

bool PlayerController::getHistoryEntryAt(int index, HistoryEntry& entry) const
{
    return history.getEntryAt(index, entry);
}

void PlayerController::clearHistory()
{
    history.clear();
}

void PlayerController::saveHistory(const shared_ptr<Playable>& item)
{
    if (item == nullptr) {
        return;
    }

    history.addItem(item);
}