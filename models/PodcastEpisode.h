#pragma once
#include "Playable.h"

class PodcastEpisode : public Playable {
public:
    PodcastEpisode(const QString& t, const QString& h, int d)
        : tytul(t), host(h), dlugosc(d) {}

    QString tytul;
    QString host;
    int dlugosc;

    QString getTytul() const override { return tytul; }
    QString getArtysta() const override { return host; }
    int getDlugosc() const override { return dlugosc; }
};