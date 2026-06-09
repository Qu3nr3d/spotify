#pragma once
#include "Playable.h"

class Song : public Playable {
public:
    Song(const QString& t, const QString& a, int d)
        : tytul(t), artysta(a), dlugosc(d) {}

    QString tytul;
    QString artysta;
    int dlugosc;

    QString getTytul() const override { return tytul; }
    QString getArtysta() const override { return artysta; }
    int getDlugosc() const override { return dlugosc; }
};