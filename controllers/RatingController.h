#ifndef SPOTIFY_RATINGCONTROLLER_H
#define SPOTIFY_RATINGCONTROLLER_H

#include <memory>

#include "../database/DatabaseManager.h"
#include "../models/Playable.h"

class RatingController
{
public:
    RatingController();

    void setDatabase(DatabaseManager *database);
    void setUserId(int userId);

    bool rateItem(const std::shared_ptr<Playable>& item, int value);
    int getUserRating(const std::shared_ptr<Playable>& item) const;
    double getAverageRating(const std::shared_ptr<Playable>& item) const;

    bool canRateItem(const std::shared_ptr<Playable>& item) const;

private:
    DatabaseManager *database;
    int userId;
};

#endif // SPOTIFY_RATINGCONTROLLER_H