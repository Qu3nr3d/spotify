#ifndef SPOTIFY_DATABASEMANAGER_H
#define SPOTIFY_DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QVector>

struct LoggedUser {
    int id;
    QString name;
    QString surname;
    QString email;
    QString accountType;
};

struct SongRecord {
    int id;
    QString title;
    QString artist;
    QString album;
    QString duration;
    QString genre;
};

class DatabaseManager {
public:
    DatabaseManager();

    bool openDatabase();
    bool createTables();

    bool seedSongs();
    QVector<SongRecord> getAllSongs();

    bool registerUser(const QString& name,
                      const QString& surname,
                      const QString& email,
                      const QString& password,
                      const QString& accountType = "free");

    bool loginUser(const QString& email,
                   const QString& password);

    LoggedUser getLoggedUser(const QString& email,
                             const QString& password);

private:
    QSqlDatabase database;

    QString hashPassword(const QString& password) const;
};

#endif // SPOTIFY_DATABASEMANAGER_H