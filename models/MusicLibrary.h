#pragma once
#include "Playable.h"
#include "Playlist.h"
#include <QVector>

class MusicLibrary {
public:
    QVector<Playable*> utwory;
    QVector<Playlist*> playlisty;

    void dodajUtwor(Playable* u) { utwory.append(u); }
    void dodajPlayliste(Playlist* p) { playlisty.append(p); }
};