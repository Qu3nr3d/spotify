#ifndef SPOTIFY_QUEUE_H
#define SPOTIFY_QUEUE_H

#include <memory>

#include <QVector>
#include <QRandomGenerator>

#include "Playable.h"

class Queue
{
public:
    Queue()
        : currentIndex(-1)
    {
    }

    void setItems(const QVector<std::shared_ptr<Playable>>& newItems)
    {
        items = newItems;
        currentIndex = items.isEmpty() ? -1 : 0;
    }

    void addItem(const std::shared_ptr<Playable>& item)
    {
        if (item == nullptr) {
            return;
        }

        items.push_back(item);

        if (currentIndex == -1) {
            currentIndex = 0;
        }
    }

    void removeItem(int index)
    {
        if (index < 0 || index >= items.size()) {
            return;
        }

        items.removeAt(index);

        if (items.isEmpty()) {
            currentIndex = -1;
            return;
        }

        if (currentIndex >= items.size()) {
            currentIndex = items.size() - 1;
        }
    }

    void clear()
    {
        items.clear();
        currentIndex = -1;
    }

    bool isEmpty() const
    {
        return items.isEmpty();
    }

    int size() const
    {
        return items.size();
    }

    int getCurrentIndex() const
    {
        return currentIndex;
    }

    bool setCurrentIndex(int index)
    {
        if (index < 0 || index >= items.size()) {
            return false;
        }

        currentIndex = index;
        return true;
    }

    bool getCurrentItem(std::shared_ptr<Playable>& item) const
    {
        if (currentIndex < 0 || currentIndex >= items.size()) {
            return false;
        }

        item = items[currentIndex];
        return item != nullptr;
    }

    bool getItemAt(int index, std::shared_ptr<Playable>& item) const
    {
        if (index < 0 || index >= items.size()) {
            return false;
        }

        item = items[index];
        return item != nullptr;
    }

    bool next(std::shared_ptr<Playable>& item)
    {
        if (items.isEmpty()) {
            return false;
        }

        if (currentIndex < 0) {
            currentIndex = 0;
        } else {
            currentIndex++;

            if (currentIndex >= items.size()) {
                currentIndex = 0;
            }
        }

        item = items[currentIndex];
        return item != nullptr;
    }

    bool previous(std::shared_ptr<Playable>& item)
    {
        if (items.isEmpty()) {
            return false;
        }

        if (currentIndex < 0) {
            currentIndex = 0;
        } else {
            currentIndex--;

            if (currentIndex < 0) {
                currentIndex = items.size() - 1;
            }
        }

        item = items[currentIndex];
        return item != nullptr;
    }

    bool shuffle(std::shared_ptr<Playable>& item)
    {
        if (items.isEmpty()) {
            return false;
        }

        currentIndex = QRandomGenerator::global()->bounded(items.size());

        item = items[currentIndex];
        return item != nullptr;
    }

private:
    QVector<std::shared_ptr<Playable>> items;
    int currentIndex;
};

#endif // SPOTIFY_QUEUE_H