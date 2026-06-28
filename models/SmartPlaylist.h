#ifndef SPOTIFY_SMARTPLAYLIST_H
#define SPOTIFY_SMARTPLAYLIST_H

#include <memory>

#include <QString>
#include <QVector>

#include "Playlist.h"
#include "Playable.h"

class SmartPlaylist : public Playlist
{
public:
    SmartPlaylist()
        : Playlist("Smart Playlist"),
          filterGenre("All")
    {
    }

    SmartPlaylist(const QString& name, const QString& filterGenre)
        : Playlist(name),
          filterGenre(filterGenre)
    {
    }

    QString getType() const override
    {
        return "SmartPlaylist";
    }

    QString getFilterGenre() const
    {
        return filterGenre;
    }

    void setFilterGenre(const QString& filterGenre)
    {
        this->filterGenre = filterGenre;
    }

    void buildFromLibrary(const QVector<std::shared_ptr<Playable>>& libraryItems)
    {
        clear();

        for (const std::shared_ptr<Playable>& item : libraryItems) {
            if (item == nullptr) {
                continue;
            }

            if (filterGenre == "All" ||
                item->getGenre().compare(filterGenre, Qt::CaseInsensitive) == 0) {
                addItem(item);
                }
        }
    }

private:
    QString filterGenre;
};

#endif // SPOTIFY_SMARTPLAYLIST_H