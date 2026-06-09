#pragma once
#include <QString>

class Playable {
public:
    virtual QString getTytul() const = 0;
    virtual QString getArtysta() const = 0;
    virtual int getDlugosc() const = 0;
    virtual ~Playable() = default;
};