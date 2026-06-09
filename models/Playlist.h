#pragma once
#include "Playable.h"
#include <QVector>
#include <QString>

class Playlist {
public:
    Playlist(const QString& n) : nazwa(n) {}
    QString nazwa;
    QVector<Playable*> utwory;

    void dodajUtwor(Playable* u) { utwory.append(u); }
};