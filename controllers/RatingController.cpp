#include "RatingController.h"

RatingController::RatingController()
    : database(nullptr),
      userId(-1)
{
}

void RatingController::setDatabase(DatabaseManager *database)
{
    this->database = database;
}

void RatingController::setUserId(int userId)
{
    this->userId = userId;
}

bool RatingController::rateItem(const std::shared_ptr<Playable>& item, int value)
{
    if (!canRateItem(item)) {
        return false;
    }

    if (value < 1 || value > 5) {
        return false;
    }

    return database->addOrUpdateRating(userId, item->getId(), value);
}

int RatingController::getUserRating(const std::shared_ptr<Playable>& item) const
{
    if (!canRateItem(item)) {
        return 0;
    }

    return database->getUserRating(userId, item->getId());
}

double RatingController::getAverageRating(const std::shared_ptr<Playable>& item) const
{
    if (database == nullptr || item == nullptr || item->getId() <= 0) {
        return 0.0;
    }

    if (item->getType() != "Song") {
        return 0.0;
    }

    return database->getAverageRating(item->getId());
}

bool RatingController::canRateItem(const std::shared_ptr<Playable>& item) const
{
    if (database == nullptr) {
        return false;
    }

    if (userId <= 0) {
        return false;
    }

    if (item == nullptr) {
        return false;
    }

    if (item->getId() <= 0) {
        return false;
    }

    if (item->getType() != "Song") {
        return false;
    }

    return true;
}