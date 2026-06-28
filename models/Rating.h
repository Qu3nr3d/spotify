#ifndef SPOTIFY_RATING_H
#define SPOTIFY_RATING_H

#include <QString>

class Rating
{
public:
    Rating()
        : id(-1),
          userId(-1),
          playableId(-1),
          value(0)
    {
    }

    Rating(int id,
           int userId,
           int playableId,
           const QString& playableType,
           int value)
        : id(id),
          userId(userId),
          playableId(playableId),
          playableType(playableType),
          value(value)
    {
    }

    int getId() const
    {
        return id;
    }

    int getUserId() const
    {
        return userId;
    }

    int getPlayableId() const
    {
        return playableId;
    }

    QString getPlayableType() const
    {
        return playableType;
    }

    int getValue() const
    {
        return value;
    }

    void setValue(int value)
    {
        if (value < 1) {
            this->value = 1;
            return;
        }

        if (value > 5) {
            this->value = 5;
            return;
        }

        this->value = value;
    }

private:
    int id;
    int userId;
    int playableId;
    QString playableType;
    int value;
};

#endif // SPOTIFY_RATING_H