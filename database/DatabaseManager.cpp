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

    const QString createHistoryTable = R"(
        CREATE TABLE IF NOT EXISTS history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            song_id INTEGER NOT NULL,
            played_at TEXT NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id),
            FOREIGN KEY(song_id) REFERENCES songs(id)
        )
    )";

    const QString createFavouritesTable = R"(
    CREATE TABLE IF NOT EXISTS favourites (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER NOT NULL,
        song_id INTEGER NOT NULL,
        created_at TEXT NOT NULL,
        UNIQUE(user_id, song_id),
        FOREIGN KEY(user_id) REFERENCES users(id),
        FOREIGN KEY(song_id) REFERENCES songs(id)
    )
)";

    if (!query.exec(createFavouritesTable)) {
        qDebug() << "Create favourites table error:" << query.lastError().text();
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

bool DatabaseManager::addHistory(int userId, int songId)
{
    if (userId <= 0 || songId <= 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO history (user_id, song_id, played_at)
        VALUES (:user_id, :song_id, datetime('now', 'localtime'))
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":song_id", songId);

    if (!query.exec()) {
        qDebug() << "Add history error:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<HistoryRecord> DatabaseManager::getUserHistory(int userId)
{
    QVector<HistoryRecord> history;

    if (userId <= 0) {
        return history;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT
            h.id AS history_id,
            h.user_id,
            h.song_id,
            h.played_at,
            s.title,
            s.artist,
            s.album,
            s.duration,
            s.genre
        FROM history h
        JOIN songs s ON h.song_id = s.id
        WHERE h.user_id = :user_id
        ORDER BY h.id DESC
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Get user history error:" << query.lastError().text();
        return history;
    }

    while (query.next()) {
        HistoryRecord record;
        record.id = query.value("history_id").toInt();
        record.userId = query.value("user_id").toInt();
        record.songId = query.value("song_id").toInt();
        record.playedAt = query.value("played_at").toString();
        record.title = query.value("title").toString();
        record.artist = query.value("artist").toString();
        record.album = query.value("album").toString();
        record.duration = query.value("duration").toString();
        record.genre = query.value("genre").toString();

        history.push_back(record);
    }

    return history;
}

bool DatabaseManager::addFavourite(int userId, int songId)
{
    if (userId <= 0 || songId <= 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT OR IGNORE INTO favourites (user_id, song_id, created_at)
        VALUES (:user_id, :song_id, datetime('now', 'localtime'))
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":song_id", songId);

    if (!query.exec()) {
        qDebug() << "Add favourite error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::removeFavourite(int userId, int songId)
{
    if (userId <= 0 || songId <= 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        DELETE FROM favourites
        WHERE user_id = :user_id
          AND song_id = :song_id
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":song_id", songId);

    if (!query.exec()) {
        qDebug() << "Remove favourite error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::isFavourite(int userId, int songId)
{
    if (userId <= 0 || songId <= 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT id
        FROM favourites
        WHERE user_id = :user_id
          AND song_id = :song_id
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":song_id", songId);

    if (!query.exec()) {
        qDebug() << "Is favourite error:" << query.lastError().text();
        return false;
    }

    return query.next();
}

QVector<SongRecord> DatabaseManager::getUserFavourites(int userId)
{
    QVector<SongRecord> songs;

    if (userId <= 0) {
        return songs;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT
            s.id,
            s.title,
            s.artist,
            s.album,
            s.duration,
            s.genre
        FROM favourites f
        JOIN songs s ON f.song_id = s.id
        WHERE f.user_id = :user_id
        ORDER BY f.created_at DESC, f.id DESC
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Get user favourites error:" << query.lastError().text();
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