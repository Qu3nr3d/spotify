#ifndef SPOTIFY_USERCONTROLLER_H
#define SPOTIFY_USERCONTROLLER_H

#include <memory>

#include <QString>

#include "../database/DatabaseManager.h"
#include "../models/User.h"
#include "../models/FreeUser.h"
#include "../models/PremiumUser.h"

class UserController
{
public:
    UserController();

    void setCurrentUser(const LoggedUser& loggedUser);
    std::shared_ptr<User> getCurrentUser() const;

    bool hasUser() const;
    bool isPremium() const;

    bool canUseShuffle() const;
    bool canUseSmartPlaylists() const;
    bool canCreatePlaylist(int currentPlaylistCount) const;
    bool canAddFavourite(int currentFavouriteCount) const;
    bool canRateSongs() const;
    bool canPlayAudiobooks() const;

    QString getAccountType() const;
    QString getFullName() const;

    void clear();

private:
    std::shared_ptr<User> currentUser;

    std::shared_ptr<User> createUserFromLoggedUser(const LoggedUser& loggedUser) const;
};

#endif // SPOTIFY_USERCONTROLLER_H