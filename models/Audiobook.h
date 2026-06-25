#ifndef SPOTIFY_AUDIOBOOK_H
#define SPOTIFY_AUDIOBOOK_H

#include <QString>

#include "Playable.h"

class Audiobook : public Playable
{
public:
    Audiobook()
        : id(-1)
    {
    }

    Audiobook(int id,
              const QString& title,
              const QString& author,
              const QString& narrator,
              const QString& duration,
              const QString& category)
        : id(id),
          title(title),
          author(author),
          narrator(narrator),
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
        return author;
    }

    QString getAlbum() const override
    {
        return narrator;
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
        return "Audiobook";
    }

    QString getAuthor() const
    {
        return author;
    }

    QString getNarrator() const
    {
        return narrator;
    }

    QString getCategory() const
    {
        return category;
    }

private:
    int id;
    QString title;
    QString author;
    QString narrator;
    QString duration;
    QString category;
};

#endif // SPOTIFY_AUDIOBOOK_H