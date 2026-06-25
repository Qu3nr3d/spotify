#ifndef SPOTIFY_SMARTPLAYLIST_H
#define SPOTIFY_SMARTPLAYLIST_H

#include <QString>

#include "Playlist.h"

class SmartPlaylist : public Playlist
{
public:
    SmartPlaylist()
        : Playlist("Smart Playlist"),
          filterType("All")
    {
    }

    SmartPlaylist(const QString& name, const QString& filterType)
        : Playlist(name),
          filterType(filterType)
    {
    }

    QString getType() const override
    {
        return "SmartPlaylist";
    }

    QString getFilterType() const
    {
        return filterType;
    }

    void setFilterType(const QString& filterType)
    {
        this->filterType = filterType;
    }

private:
    QString filterType;
};

#endif // SPOTIFY_SMARTPLAYLIST_H