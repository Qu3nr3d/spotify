#ifndef SPOTIFY_USER_H
#define SPOTIFY_USER_H

#include <QString>

class User
{
public:
    User()
        : id(-1)
    {
    }

    User(int id,
         const QString& name,
         const QString& surname,
         const QString& email,
         const QString& accountType)
        : id(id),
          name(name),
          surname(surname),
          email(email),
          accountType(accountType)
    {
    }

    virtual ~User() = default;

    int getId() const
    {
        return id;
    }

    QString getName() const
    {
        return name;
    }

    QString getSurname() const
    {
        return surname;
    }

    QString getEmail() const
    {
        return email;
    }

    QString getAccountType() const
    {
        return accountType;
    }

    QString getFullName() const
    {
        return name + " " + surname;
    }

    virtual bool canUseShuffle() const = 0;
    virtual bool canUseSmartPlaylists() const = 0;

    virtual int getMaxPlaylists() const = 0;
    virtual int getMaxFavourites() const = 0;

    virtual bool canCreatePlaylist(int currentPlaylistCount) const
    {
        return currentPlaylistCount < getMaxPlaylists();
    }

    virtual bool canAddFavourite(int currentFavouriteCount) const
    {
        return currentFavouriteCount < getMaxFavourites();
    }

    virtual bool canRateSongs() const
    {
        return true;
    }

    virtual bool canPlayAudiobooks() const
    {
        return true;
    }

protected:
    int id;
    QString name;
    QString surname;
    QString email;
    QString accountType;
};

#endif // SPOTIFY_USER_H