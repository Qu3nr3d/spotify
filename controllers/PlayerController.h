#ifndef SPOTIFY_PLAYERCONTROLLER_H
#define SPOTIFY_PLAYERCONTROLLER_H

#include <memory>

#include <QVector>

#include "../models/Player.h"
#include "../models/Playable.h"
#include "../models/Song.h"
#include "../models/History.h"
#include "../database/DatabaseManager.h"

using namespace std;

class PlayerController
{
public:
    PlayerController();
    void setItems(const QVector<shared_ptr<Playable>>& items);
    void setSongs(const QVector<SongRecord>& songs);
    void clear();
    void setDatabase(DatabaseManager *database);
    void setUserId(int userId);
    bool playAt(int index, shared_ptr<Playable>& item, bool& isPlaying);
    bool togglePlay(shared_ptr<Playable>& item, bool& isPlaying);
    bool next(shared_ptr<Playable>& item, bool& isPlaying);
    bool previous(shared_ptr<Playable>& item, bool& isPlaying);
    bool shuffle(shared_ptr<Playable>& item, bool& isPlaying);
    bool getCurrentItem(shared_ptr<Playable>& item) const;
    bool isPlaying() const;
    QVector<HistoryEntry> getHistoryEntries() const;
    bool getHistoryEntryAt(int index, HistoryEntry& entry) const;
    void clearHistory();

private:
    Player player;
    History history;
    DatabaseManager *database;
    int userId;
    void saveHistory(const shared_ptr<Playable>& item);
};

#endif // SPOTIFY_PLAYERCONTROLLER_H