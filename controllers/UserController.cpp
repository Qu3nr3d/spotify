#include "UserController.h"

UserController::UserController()
    : currentUser(nullptr)
{
}

void UserController::setCurrentUser(const LoggedUser& loggedUser)
{
    currentUser = createUserFromLoggedUser(loggedUser);
}

std::shared_ptr<User> UserController::getCurrentUser() const
{
    return currentUser;
}

bool UserController::hasUser() const
{
    return currentUser != nullptr;
}

bool UserController::isPremium() const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->getAccountType().toLower() == "premium";
}

bool UserController::canUseShuffle() const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->canUseShuffle();
}

bool UserController::canUseSmartPlaylists() const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->canUseSmartPlaylists();
}

bool UserController::canCreatePlaylist(int currentPlaylistCount) const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->canCreatePlaylist(currentPlaylistCount);
}

bool UserController::canAddFavourite(int currentFavouriteCount) const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->canAddFavourite(currentFavouriteCount);
}

bool UserController::canRateSongs() const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->canRateSongs();
}

bool UserController::canPlayAudiobooks() const
{
    if (currentUser == nullptr) {
        return false;
    }

    return currentUser->canPlayAudiobooks();
}

QString UserController::getAccountType() const
{
    if (currentUser == nullptr) {
        return "unknown";
    }

    return currentUser->getAccountType();
}

QString UserController::getFullName() const
{
    if (currentUser == nullptr) {
        return "";
    }

    return currentUser->getFullName();
}

void UserController::clear()
{
    currentUser = nullptr;
}

std::shared_ptr<User> UserController::createUserFromLoggedUser(const LoggedUser& loggedUser) const
{
    QString accountType = loggedUser.accountType.toLower();

    if (accountType == "premium") {
        return std::make_shared<PremiumUser>(
            loggedUser.id,
            loggedUser.name,
            loggedUser.surname,
            loggedUser.email
        );
    }

    return std::make_shared<FreeUser>(
        loggedUser.id,
        loggedUser.name,
        loggedUser.surname,
        loggedUser.email
    );
}