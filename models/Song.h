#ifndef SPOTIFY_SONG_H
#define SPOTIFY_SONG_H

#include <QString>

#include "Playable.h"

class Song : public Playable
{
public:
    Song()
        : id(-1)
    {
    }

    Song(int id,
         const QString& title,
         const QString& artist,
         const QString& album,
         const QString& duration,
         const QString& genre)
        : id(id),
          title(title),
          artist(artist),
          album(album),
          duration(duration),
          genre(genre)
    {
    }

    int getId() const override
    {
        return id;
    }

    QString getTitle() const override
    {
        return title;
    }

    QString getArtist() const override
    {
        return artist;
    }

    QString getAlbum() const override
    {
        return album;
    }

    QString getDuration() const override
    {
        return duration;
    }

    QString getGenre() const override
    {
        return genre;
    }

    QString getType() const override
    {
        return "Song";
    }

private:
    int id;
    QString title;
    QString artist;
    QString album;
    QString duration;
    QString genre;
};

#endif // SPOTIFY_SONG_H