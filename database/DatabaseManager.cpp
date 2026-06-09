#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

DatabaseManager::DatabaseManager() = default;

bool DatabaseManager::openDatabase()
{
    if (QSqlDatabase::contains("spotify_connection")) {
        database = QSqlDatabase::database("spotify_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE", "spotify_connection");
        database.setDatabaseName("spotify.db");
    }

    if (!database.open()) {
        qDebug() << "Database open error:" << database.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(database);

    const QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            surname TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            account_type TEXT NOT NULL DEFAULT 'free'
        )
)";
    if (!query.exec(createUsersTable)) {
        qDebug() << "Create users table error:" << query.lastError().text();
        return false;
    }

    const QString createSongsTable = R"(
    CREATE TABLE IF NOT EXISTS songs (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        title TEXT NOT NULL,
        artist TEXT NOT NULL,
        album TEXT,
        duration TEXT,
        genre TEXT,
        UNIQUE(title, artist)
    )
)";

    if (!query.exec(createSongsTable)) {
        qDebug() << "Create songs table error:" << query.lastError().text();
        return false;
    }
    return true;
}

QString DatabaseManager::hashPassword(const QString& password) const
{
    QByteArray passwordBytes = password.toUtf8();

    QByteArray hash = QCryptographicHash::hash(
        passwordBytes,
        QCryptographicHash::Sha256
    );

    return QString(hash.toHex());
}

bool DatabaseManager::registerUser(const QString& name,
                                   const QString& surname,
                                   const QString& email,
                                   const QString& password,
                                   const QString& accountType)
{
    if (name.isEmpty() || surname.isEmpty() || email.isEmpty() || password.isEmpty()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO users (name, surname, email, password_hash, account_type)
        VALUES (:name, :surname, :email, :password_hash, :account_type)
    )");

    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":email", email);
    query.bindValue(":password_hash", hashPassword(password));
    query.bindValue(":account_type", accountType);

    if (!query.exec()) {
        qDebug() << "Register user error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::loginUser(const QString& email,
                                const QString& password)
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT id
        FROM users
        WHERE email = :email
          AND password_hash = :password_hash
    )");

    query.bindValue(":email", email);
    query.bindValue(":password_hash", hashPassword(password));

    if (!query.exec()) {
        qDebug() << "Login user error:" << query.lastError().text();
        return false;
    }

    return query.next();
}

LoggedUser DatabaseManager::getLoggedUser(const QString& email,
                                          const QString& password)
{
    LoggedUser user;
    user.id = -1;

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT id, name, surname, email, account_type
        FROM users
        WHERE email = :email
          AND password_hash = :password_hash
    )");

    query.bindValue(":email", email);
    query.bindValue(":password_hash", hashPassword(password));

    if (!query.exec()) {
        qDebug() << "Get logged user error:" << query.lastError().text();
        return user;
    }

    if (query.next()) {
        user.id = query.value("id").toInt();
        user.name = query.value("name").toString();
        user.surname = query.value("surname").toString();
        user.email = query.value("email").toString();
        user.accountType = query.value("account_type").toString();
    }

    return user;
}

bool DatabaseManager::seedSongs()
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT OR IGNORE INTO songs (title, artist, album, duration, genre)
        VALUES (:title, :artist, :album, :duration, :genre)
    )");

    struct SeedSong {
        QString title;
        QString artist;
        QString album;
        QString duration;
        QString genre;
    };

    QVector<SeedSong> songs = {
        {"Blinding Lights", "The Weeknd", "After Hours", "3:20", "Pop"},
        {"Shape of You", "Ed Sheeran", "Divide", "3:53", "Pop"},
        {"Bohemian Rhapsody", "Queen", "A Night at the Opera", "5:55", "Rock"},
        {"Billie Jean", "Michael Jackson", "Thriller", "4:54", "Pop"},
        {"Smells Like Teen Spirit", "Nirvana", "Nevermind", "5:01", "Grunge"},
        {"Lose Yourself", "Eminem", "8 Mile", "5:26", "Rap"},
        {"Get Lucky", "Daft Punk", "Random Access Memories", "4:08", "Electronic"},
        {"Numb", "Linkin Park", "Meteora", "3:07", "Rock"}
    };

    for (const SeedSong& song : songs) {
        query.bindValue(":title", song.title);
        query.bindValue(":artist", song.artist);
        query.bindValue(":album", song.album);
        query.bindValue(":duration", song.duration);
        query.bindValue(":genre", song.genre);

        if (!query.exec()) {
            qDebug() << "Seed song error:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

QVector<SongRecord> DatabaseManager::getAllSongs()
{
    QVector<SongRecord> songs;

    QSqlQuery query(database);

    if (!query.exec(R"(
        SELECT id, title, artist, album, duration, genre
        FROM songs
        ORDER BY title ASC
    )")) {
        qDebug() << "Get all songs error:" << query.lastError().text();
        return songs;
    }

    while (query.next()) {
        SongRecord song;
        song.id = query.value("id").toInt();
        song.title = query.value("title").toString();
        song.artist = query.value("artist").toString();
        song.album = query.value("album").toString();
        song.duration = query.value("duration").toString();
        song.genre = query.value("genre").toString();

        songs.push_back(song);
    }

    return songs;
}