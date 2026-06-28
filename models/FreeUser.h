#ifndef SPOTIFY_FREEUSER_H
#define SPOTIFY_FREEUSER_H

#include "User.h"

class FreeUser : public User
{
public:
    FreeUser()
        : User(-1, "", "", "", "free")
    {
    }

    FreeUser(int id,
             const QString& name,
             const QString& surname,
             const QString& email)
        : User(id, name, surname, email, "free")
    {
    }

    bool canUseShuffle() const override
    {
        return false;
    }

    bool canUseSmartPlaylists() const override
    {
        return false;
    }

    int getMaxPlaylists() const override
    {
        return 3;
    }

    int getMaxFavourites() const override
    {
        return 5;
    }
};

#endif // SPOTIFY_FREEUSER_H