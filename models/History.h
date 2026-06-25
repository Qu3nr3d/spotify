#ifndef SPOTIFY_HISTORY_H
#define SPOTIFY_HISTORY_H

#include <memory>

#include <QVector>
#include <QString>
#include <QDateTime>

#include "Playable.h"

struct HistoryEntry
{
    std::shared_ptr<Playable> item;
    QString playedAt;
};

class History
{
public:
    History() = default;

    void addItem(const std::shared_ptr<Playable>& item)
    {
        if (item == nullptr) {
            return;
        }

        HistoryEntry entry;
        entry.item = item;
        entry.playedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        entries.prepend(entry);
    }

    QVector<HistoryEntry> getEntries() const
    {
        return entries;
    }

    bool getEntryAt(int index, HistoryEntry& entry) const
    {
        if (index < 0 || index >= entries.size()) {
            return false;
        }

        entry = entries[index];
        return entry.item != nullptr;
    }

    bool isEmpty() const
    {
        return entries.isEmpty();
    }

    int size() const
    {
        return entries.size();
    }

    void clear()
    {
        entries.clear();
    }

private:
    QVector<HistoryEntry> entries;
};

#endif // SPOTIFY_HISTORY_H