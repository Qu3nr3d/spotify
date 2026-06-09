#pragma once
#include "Playlist.h"

class FavoritePlaylist {
public:
    FavoritePlaylist(Playlist* ulubione) : favoritePlaylist(ulubione) {}

    void dodajDoUlubionych(Playable* item) {
        if (!favorites.contains(item)) {
            favorites.append(item);
            if (favoritePlaylist) favoritePlaylist->dodajUtwor(item);
        }
    }

    QVector<Playable*> getUlubione() const { return favorites; }

private:
    QVector<Playable*> favorites;
    Playlist* favoritePlaylist; // wskaźnik do playlisty "Ulubione"
};