#pragma once
#include <QString>
#include <QVector>

// ----------------- Song -----------------
class Song {
public:
    Song(const QString& t, const QString& a, int d)
        : title(t), artist(a), duration(d) {}
    QString title;
    QString artist;
    int duration; // w sekundach
};

// ----------------- Playlist -----------------
class Playlist {
public:
    Playlist(const QString& n) : name(n) {}
    QString name;
    QVector<Song*> songs;

    void addSong(Song* song) { songs.append(song); }
};

// ----------------- MusicLibrary -----------------
class MusicLibrary {
public:
    QVector<Song*> songs;
    QVector<Playlist*> playlists;

    void addSong(Song* song) { songs.append(song); }
    void addPlaylist(Playlist* playlist) { playlists.append(playlist); }
};

// ----------------- Player -----------------
class Player {
public:
    Playlist* currentPlaylist = nullptr;
    int currentIndex = 0;

    Song* getCurrentSong() {
        if (!currentPlaylist || currentPlaylist->songs.isEmpty()) return nullptr;
        return currentPlaylist->songs[currentIndex];
    }

    void next() {
        if (!currentPlaylist || currentPlaylist->songs.isEmpty()) return;
        currentIndex = (currentIndex + 1) % currentPlaylist->songs.size();
    }

    void previous() {
        if (!currentPlaylist || currentPlaylist->songs.isEmpty()) return;
        currentIndex = (currentIndex - 1 + currentPlaylist->songs.size()) % currentPlaylist->songs.size();
    }
};