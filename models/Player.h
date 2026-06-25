#ifndef SPOTIFY_PLAYER_H
#define SPOTIFY_PLAYER_H

#include <memory>

#include "Queue.h"
#include "Playable.h"

class Player
{
public:
    Player()
        : playing(false)
    {
    }

    void setQueue(const QVector<std::shared_ptr<Playable>>& items)
    {
        queue.setItems(items);
        playing = false;
    }

    void clear()
    {
        queue.clear();
        playing = false;
    }

    bool isPlaying() const
    {
        return playing;
    }

    bool hasItems() const
    {
        return !queue.isEmpty();
    }

    bool playAt(int index, std::shared_ptr<Playable>& item)
    {
        if (!queue.setCurrentIndex(index)) {
            return false;
        }

        if (!queue.getCurrentItem(item)) {
            return false;
        }

        playing = true;
        return true;
    }

    bool togglePlay(std::shared_ptr<Playable>& item)
    {
        if (!queue.getCurrentItem(item)) {
            if (!queue.next(item)) {
                return false;
            }

            playing = true;
            return true;
        }

        playing = !playing;
        return true;
    }

    bool next(std::shared_ptr<Playable>& item)
    {
        if (!queue.next(item)) {
            return false;
        }

        playing = true;
        return true;
    }

    bool previous(std::shared_ptr<Playable>& item)
    {
        if (!queue.previous(item)) {
            return false;
        }

        playing = true;
        return true;
    }

    bool shuffle(std::shared_ptr<Playable>& item)
    {
        if (!queue.shuffle(item)) {
            return false;
        }

        playing = true;
        return true;
    }

    bool getCurrentItem(std::shared_ptr<Playable>& item) const
    {
        return queue.getCurrentItem(item);
    }

private:
    Queue queue;
    bool playing;
};

#endif // SPOTIFY_PLAYER_H