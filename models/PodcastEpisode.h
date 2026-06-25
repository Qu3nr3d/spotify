#ifndef SPOTIFY_PODCASTEPISODE_H
#define SPOTIFY_PODCASTEPISODE_H

#include <QString>

#include "Playable.h"

class PodcastEpisode : public Playable
{
public:
    PodcastEpisode()
        : id(-1)
    {
    }

    PodcastEpisode(int id,
                   const QString& title,
                   const QString& host,
                   const QString& podcastName,
                   const QString& duration,
                   const QString& category)
        : id(id),
          title(title),
          host(host),
          podcastName(podcastName),
          duration(duration),
          category(category)
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
        return host;
    }

    QString getAlbum() const override
    {
        return podcastName;
    }

    QString getDuration() const override
    {
        return duration;
    }

    QString getGenre() const override
    {
        return category;
    }

    QString getType() const override
    {
        return "PodcastEpisode";
    }

    QString getHost() const
    {
        return host;
    }

    QString getPodcastName() const
    {
        return podcastName;
    }

    QString getCategory() const
    {
        return category;
    }

private:
    int id;
    QString title;
    QString host;
    QString podcastName;
    QString duration;
    QString category;
};

#endif // SPOTIFY_PODCASTEPISODE_H