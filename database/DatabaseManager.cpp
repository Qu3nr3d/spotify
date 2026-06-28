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

    const QString createAudiobooksTable = R"(
        CREATE TABLE IF NOT EXISTS audiobooks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            narrator TEXT,
            duration TEXT,
            category TEXT,
            UNIQUE(title, author)
        )
    )";

    const QString createPodcastEpisodesTable = R"(
        CREATE TABLE IF NOT EXISTS podcast_episodes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            host TEXT NOT NULL,
            podcast_name TEXT NOT NULL,
            duration TEXT,
            category TEXT,
            UNIQUE(title, podcast_name)
        )
    )";

    if (!query.exec(createPodcastEpisodesTable)) {
        qDebug() << "Create podcast_episodes table error:" << query.lastError().text();
        return false;
    }

    if (!query.exec(createAudiobooksTable)) {
        qDebug() << "Create audiobooks table error:" << query.lastError().text();
        return false;
    }

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

    if (!query.exec(createHistoryTable)) {
        qDebug() << "Create history table error:" << query.lastError().text();
        return false;
    }

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

    const QString createPlaylistsTable = R"(
        CREATE TABLE IF NOT EXISTS playlists (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            created_at TEXT NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        )
    )";

    if (!query.exec(createPlaylistsTable)) {
        qDebug() << "Create playlists table error:" << query.lastError().text();
        return false;
    }

    const QString createPlaylistItemsTable = R"(
        CREATE TABLE IF NOT EXISTS playlist_items (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            playlist_id INTEGER NOT NULL,
            song_id INTEGER NOT NULL,
            added_at TEXT NOT NULL,
            UNIQUE(playlist_id, song_id),
            FOREIGN KEY(playlist_id) REFERENCES playlists(id),
            FOREIGN KEY(song_id) REFERENCES songs(id)
        )
    )";

    if (!query.exec(createPlaylistItemsTable)) {
        qDebug() << "Create playlist_items table error:" << query.lastError().text();
        return false;
    }

    const QString createRatingsTable = R"(
        CREATE TABLE IF NOT EXISTS ratings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            song_id INTEGER NOT NULL,
            value INTEGER NOT NULL CHECK(value >= 1 AND value <= 5),
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL,
            UNIQUE(user_id, song_id),
            FOREIGN KEY(user_id) REFERENCES users(id),
            FOREIGN KEY(song_id) REFERENCES songs(id)
        )
    )";

    if (!query.exec(createRatingsTable)) {
        qDebug() << "Create ratings table error:" << query.lastError().text();
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

bool DatabaseManager::seedAudiobooks()
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT OR IGNORE INTO audiobooks (title, author, narrator, duration, category)
        VALUES (:title, :author, :narrator, :duration, :category)
    )");

    struct SeedAudiobook {
        QString title;
        QString author;
        QString narrator;
        QString duration;
        QString category;
    };

    QVector<SeedAudiobook> audiobooks = {
        {"Harry Potter i Kamien Filozoficzny", "J.K. Rowling", "Piotr Fronczewski", "8:17:00", "Fantasy"},
        {"Wiedzmin: Ostatnie zyczenie", "Andrzej Sapkowski", "Krzysztof Gosztyla", "10:42:00", "Fantasy"},
        {"Lalka", "Boleslaw Prus", "Wiktor Zborowski", "27:15:00", "Klasyka"},
        {"Rok 1984", "George Orwell", "Maciej Gudowski", "11:20:00", "Dystopia"},
        {"Zbrodnia i kara", "Fiodor Dostojewski", "Adam Ferency", "23:40:00", "Klasyka"}
    };

    for (const SeedAudiobook& audiobook : audiobooks) {
        query.bindValue(":title", audiobook.title);
        query.bindValue(":author", audiobook.author);
        query.bindValue(":narrator", audiobook.narrator);
        query.bindValue(":duration", audiobook.duration);
        query.bindValue(":category", audiobook.category);

        if (!query.exec()) {
            qDebug() << "Seed audiobook error:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool DatabaseManager::seedPodcastEpisodes()
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT OR IGNORE INTO podcast_episodes (title, host, podcast_name, duration, category)
        VALUES (:title, :host, :podcast_name, :duration, :category)
    )");

    struct SeedPodcastEpisode {
        QString title;
        QString host;
        QString podcastName;
        QString duration;
        QString category;
    };

    QVector<SeedPodcastEpisode> podcastEpisodes = {
        {"Jak dziala sztuczna inteligencja?", "Karol Stryja", "Tech Rozmowy", "42:15", "Technologia"},
        {"Historia Internetu", "Marek Nowak", "Cyfrowy Swiat", "55:30", "Technologia"},
        {"Dlaczego ludzie lubia muzyke?", "Anna Kowalska", "Nauka Codzienna", "37:45", "Nauka"},
        {"Kryminalna Warszawa", "Jan Malinowski", "Historie z Miasta", "48:20", "Kryminalne"},
        {"Jak odpoczywac skutecznie?", "Magda Zielinska", "Zdrowa Glowa", "29:10", "Lifestyle"}
    };

    for (const SeedPodcastEpisode& episode : podcastEpisodes) {
        query.bindValue(":title", episode.title);
        query.bindValue(":host", episode.host);
        query.bindValue(":podcast_name", episode.podcastName);
        query.bindValue(":duration", episode.duration);
        query.bindValue(":category", episode.category);

        if (!query.exec()) {
            qDebug() << "Seed podcast episode error:" << query.lastError().text();
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

QVector<AudiobookRecord> DatabaseManager::getAllAudiobooks()
{
    QVector<AudiobookRecord> audiobooks;

    QSqlQuery query(database);

    if (!query.exec(R"(
        SELECT id, title, author, narrator, duration, category
        FROM audiobooks
        ORDER BY title ASC
    )")) {
        qDebug() << "Get all audiobooks error:" << query.lastError().text();
        return audiobooks;
    }

    while (query.next()) {
        AudiobookRecord audiobook;
        audiobook.id = query.value("id").toInt();
        audiobook.title = query.value("title").toString();
        audiobook.author = query.value("author").toString();
        audiobook.narrator = query.value("narrator").toString();
        audiobook.duration = query.value("duration").toString();
        audiobook.category = query.value("category").toString();

        audiobooks.push_back(audiobook);
    }

    return audiobooks;
}

QVector<PodcastEpisodeRecord> DatabaseManager::getAllPodcastEpisodes()
{
    QVector<PodcastEpisodeRecord> podcastEpisodes;

    QSqlQuery query(database);

    if (!query.exec(R"(
        SELECT id, title, host, podcast_name, duration, category
        FROM podcast_episodes
        ORDER BY title ASC
    )")) {
        qDebug() << "Get all podcast episodes error:" << query.lastError().text();
        return podcastEpisodes;
    }

    while (query.next()) {
        PodcastEpisodeRecord episode;
        episode.id = query.value("id").toInt();
        episode.title = query.value("title").toString();
        episode.host = query.value("host").toString();
        episode.podcastName = query.value("podcast_name").toString();
        episode.duration = query.value("duration").toString();
        episode.category = query.value("category").toString();

        podcastEpisodes.push_back(episode);
    }

    return podcastEpisodes;
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

bool DatabaseManager::createPlaylist(int userId, const QString& name)
{
    if (userId <= 0 || name.trimmed().isEmpty()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO playlists (user_id, name, created_at)
        VALUES (:user_id, :name, datetime('now', 'localtime'))
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":name", name.trimmed());

    if (!query.exec()) {
        qDebug() << "Create playlist error:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<PlaylistRecord> DatabaseManager::getUserPlaylists(int userId)
{
    QVector<PlaylistRecord> playlists;

    if (userId <= 0) {
        return playlists;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT
            p.id,
            p.user_id,
            p.name,
            p.created_at,
            COUNT(pi.id) AS songs_count
        FROM playlists p
        LEFT JOIN playlist_items pi ON p.id = pi.playlist_id
        WHERE p.user_id = :user_id
        GROUP BY p.id, p.user_id, p.name, p.created_at
        ORDER BY p.created_at DESC, p.id DESC
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Get user playlists error:" << query.lastError().text();
        return playlists;
    }

    while (query.next()) {
        PlaylistRecord playlist;
        playlist.id = query.value("id").toInt();
        playlist.userId = query.value("user_id").toInt();
        playlist.name = query.value("name").toString();
        playlist.createdAt = query.value("created_at").toString();
        playlist.songsCount = query.value("songs_count").toInt();

        playlists.push_back(playlist);
    }

    return playlists;
}

bool DatabaseManager::addSongToPlaylist(int playlistId, int songId)
{
    if (playlistId <= 0 || songId <= 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT OR IGNORE INTO playlist_items (playlist_id, song_id, added_at)
        VALUES (:playlist_id, :song_id, datetime('now', 'localtime'))
    )");

    query.bindValue(":playlist_id", playlistId);
    query.bindValue(":song_id", songId);

    if (!query.exec()) {
        qDebug() << "Add song to playlist error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::removeSongFromPlaylist(int playlistId, int songId)
{
    if (playlistId <= 0 || songId <= 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        DELETE FROM playlist_items
        WHERE playlist_id = :playlist_id
          AND song_id = :song_id
    )");

    query.bindValue(":playlist_id", playlistId);
    query.bindValue(":song_id", songId);

    if (!query.exec()) {
        qDebug() << "Remove song from playlist error:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<SongRecord> DatabaseManager::getPlaylistSongs(int playlistId)
{
    QVector<SongRecord> songs;

    if (playlistId <= 0) {
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
        FROM playlist_items pi
        JOIN songs s ON pi.song_id = s.id
        WHERE pi.playlist_id = :playlist_id
        ORDER BY pi.added_at DESC, pi.id DESC
    )");

    query.bindValue(":playlist_id", playlistId);

    if (!query.exec()) {
        qDebug() << "Get playlist songs error:" << query.lastError().text();
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

bool DatabaseManager::deletePlaylist(int playlistId)
{
    if (playlistId <= 0) {
        return false;
    }

    QSqlQuery deleteItemsQuery(database);
    deleteItemsQuery.prepare(R"(
        DELETE FROM playlist_items
        WHERE playlist_id = :playlist_id
    )");

    deleteItemsQuery.bindValue(":playlist_id", playlistId);

    if (!deleteItemsQuery.exec()) {
        qDebug() << "Delete playlist items error:" << deleteItemsQuery.lastError().text();
        return false;
    }

    QSqlQuery deletePlaylistQuery(database);
    deletePlaylistQuery.prepare(R"(
        DELETE FROM playlists
        WHERE id = :playlist_id
    )");

    deletePlaylistQuery.bindValue(":playlist_id", playlistId);

    if (!deletePlaylistQuery.exec()) {
        qDebug() << "Delete playlist error:" << deletePlaylistQuery.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::addOrUpdateRating(int userId, int songId, int value)
{
    if (userId <= 0 || songId <= 0 || value < 1 || value > 5) {
        return false;
    }

    QSqlQuery checkQuery(database);

    QString checkSql = QString(
        "SELECT id FROM ratings "
        "WHERE user_id = %1 AND song_id = %2"
    ).arg(userId).arg(songId);

    if (!checkQuery.exec(checkSql)) {
        qDebug() << "Check rating error:" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next()) {
        QSqlQuery updateQuery(database);

        QString updateSql = QString(
            "UPDATE ratings "
            "SET value = %1, updated_at = datetime('now', 'localtime') "
            "WHERE user_id = %2 AND song_id = %3"
        ).arg(value).arg(userId).arg(songId);

        if (!updateQuery.exec(updateSql)) {
            qDebug() << "Update rating error:" << updateQuery.lastError().text();
            return false;
        }

        return true;
    }

    QSqlQuery insertQuery(database);

    QString insertSql = QString(
        "INSERT INTO ratings (user_id, song_id, value, created_at, updated_at) "
        "VALUES (%1, %2, %3, datetime('now', 'localtime'), datetime('now', 'localtime'))"
    ).arg(userId).arg(songId).arg(value);

    if (!insertQuery.exec(insertSql)) {
        qDebug() << "Insert rating error:" << insertQuery.lastError().text();
        return false;
    }

    return true;
}

int DatabaseManager::getUserRating(int userId, int songId)
{
    if (userId <= 0 || songId <= 0) {
        return 0;
    }

    QSqlQuery query(database);

    QString sql = QString(
        "SELECT value FROM ratings "
        "WHERE user_id = %1 AND song_id = %2"
    ).arg(userId).arg(songId);

    if (!query.exec(sql)) {
        qDebug() << "Get user rating error:" << query.lastError().text();
        return 0;
    }

    if (query.next()) {
        return query.value("value").toInt();
    }

    return 0;
}

double DatabaseManager::getAverageRating(int songId)
{
    if (songId <= 0) {
        return 0.0;
    }

    QSqlQuery query(database);

    QString sql = QString(
        "SELECT AVG(value) AS average_rating FROM ratings "
        "WHERE song_id = %1"
    ).arg(songId);

    if (!query.exec(sql)) {
        qDebug() << "Get average rating error:" << query.lastError().text();
        return 0.0;
    }

    if (query.next()) {
        return query.value("average_rating").toDouble();
    }

    return 0.0;
}

QVector<RatingRecord> DatabaseManager::getUserRatings(int userId)
{
    QVector<RatingRecord> ratings;

    if (userId <= 0) {
        return ratings;
    }

    QSqlQuery query(database);

    QString sql = QString(
        "SELECT id, user_id, song_id, value, created_at, updated_at "
        "FROM ratings "
        "WHERE user_id = %1 "
        "ORDER BY updated_at DESC, id DESC"
    ).arg(userId);

    if (!query.exec(sql)) {
        qDebug() << "Get user ratings error:" << query.lastError().text();
        return ratings;
    }

    while (query.next()) {
        RatingRecord rating;
        rating.id = query.value("id").toInt();
        rating.userId = query.value("user_id").toInt();
        rating.songId = query.value("song_id").toInt();
        rating.value = query.value("value").toInt();
        rating.createdAt = query.value("created_at").toString();
        rating.updatedAt = query.value("updated_at").toString();

        ratings.push_back(rating);
    }

    return ratings;
}

bool DatabaseManager::updateUserAccountType(int userId, const QString& accountType)
{
    if (userId <= 0) {
        return false;
    }

    QString normalizedAccountType = accountType.trimmed().toLower();

    if (normalizedAccountType != "free" && normalizedAccountType != "premium") {
        return false;
    }

    QSqlQuery query(database);

    QString sql = QString(
        "UPDATE users "
        "SET account_type = '%1' "
        "WHERE id = %2"
    ).arg(normalizedAccountType).arg(userId);

    if (!query.exec(sql)) {
        qDebug() << "Update user account type error:" << query.lastError().text();
        return false;
    }

    return true;
}