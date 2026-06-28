#ifndef SPOTIFY_PREMIUMUSER_H
#define SPOTIFY_PREMIUMUSER_H

#include <limits>

#include "User.h"

using namespace std;

class PremiumUser : public User
{
public:
    PremiumUser()
        : User(-1, "", "", "", "premium")
    {
    }

    PremiumUser(int id,
                const QString& name,
                const QString& surname,
                const QString& email)
        : User(id, name, surname, email, "premium")
    {
    }

    bool canUseShuffle() const override
    {
        return true;
    }

    bool canUseSmartPlaylists() const override
    {
        return true;
    }

    int getMaxPlaylists() const override
    {
        return numeric_limits<int>::max();
    }

    int getMaxFavourites() const override
    {
        return numeric_limits<int>::max();
    }
};

#endif // SPOTIFY_PREMIUMUSER_H