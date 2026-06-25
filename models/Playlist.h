#ifndef SPOTIFY_PLAYLIST_H
#define SPOTIFY_PLAYLIST_H

#include <memory>

#include <QString>
#include <QVector>

#include "Playable.h"

class Playlist
{
public:
    Playlist()
        : id(-1),
          name("Playlist")
    {
    }

    explicit Playlist(const QString& name)
        : id(-1),
          name(name)
    {
    }

    Playlist(int id, const QString& name)
        : id(id),
          name(name)
    {
    }

    virtual ~Playlist() = default;

    int getId() const
    {
        return id;
    }

    void setId(int id)
    {
        this->id = id;
    }

    QString getName() const
    {
        return name;
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    virtual QString getType() const
    {
        return "Playlist";
    }

    virtual void addItem(const std::shared_ptr<Playable>& item)
    {
        if (item == nullptr) {
            return;
        }

        items.push_back(item);
    }

    virtual void removeItem(int itemId)
    {
        for (int i = 0; i < items.size(); ++i) {
            if (items[i] != nullptr && items[i]->getId() == itemId) {
                items.removeAt(i);
                return;
            }
        }
    }

    bool containsItem(int itemId) const
    {
        for (const std::shared_ptr<Playable>& item : items) {
            if (item != nullptr && item->getId() == itemId) {
                return true;
            }
        }

        return false;
    }

    QVector<std::shared_ptr<Playable>> getItems() const
    {
        return items;
    }

    bool getItemAt(int index, std::shared_ptr<Playable>& item) const
    {
        if (index < 0 || index >= items.size()) {
            return false;
        }

        item = items[index];
        return item != nullptr;
    }

    int size() const
    {
        return items.size();
    }

    bool isEmpty() const
    {
        return items.isEmpty();
    }

    void clear()
    {
        items.clear();
    }

protected:
    int id;
    QString name;
    QVector<std::shared_ptr<Playable>> items;
};

#endif // SPOTIFY_PLAYLIST_H