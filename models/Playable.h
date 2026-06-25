#ifndef SPOTIFY_PLAYABLE_H
#define SPOTIFY_PLAYABLE_H

#include <QString>

class Playable
{
public:
    virtual ~Playable() = default;

    virtual int getId() const = 0;

    virtual QString getTitle() const = 0;
    virtual QString getArtist() const = 0;
    virtual QString getAlbum() const = 0;
    virtual QString getDuration() const = 0;
    virtual QString getGenre() const = 0;

    virtual QString getType() const = 0;
};

#endif // SPOTIFY_PLAYABLE_H