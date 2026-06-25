#ifndef SPOTIFY_FAVORITEPLAYLIST_H
#define SPOTIFY_FAVORITEPLAYLIST_H

#include "Playlist.h"

class FavoritePlaylist : public Playlist
{
public:
    FavoritePlaylist()
        : Playlist("Ulubione")
    {
    }

    explicit FavoritePlaylist(int id)
        : Playlist(id, "Ulubione")
    {
    }

    QString getType() const override
    {
        return "FavoritePlaylist";
    }

    void addItem(const std::shared_ptr<Playable>& item) override
    {
        if (item == nullptr) {
            return;
        }

        if (containsItem(item->getId())) {
            return;
        }

        items.push_back(item);
    }

    void toggleItem(const std::shared_ptr<Playable>& item)
    {
        if (item == nullptr) {
            return;
        }

        if (containsItem(item->getId())) {
            removeItem(item->getId());
        } else {
            addItem(item);
        }
    }

    bool isFavourite(int itemId) const
    {
        return containsItem(itemId);
    }
};

#endif // SPOTIFY_FAVORITEPLAYLIST_H