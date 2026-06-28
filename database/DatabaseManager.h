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

struct AudiobookRecord {
    int id;
    QString title;
    QString author;
    QString narrator;
    QString duration;
    QString category;
};

struct PodcastEpisodeRecord {
    int id;
    QString title;
    QString host;
    QString podcastName;
    QString duration;
    QString category;
};

struct HistoryRecord {
    int id;
    int userId;
    int songId;
    QString title;
    QString artist;
    QString album;
    QString duration;
    QString genre;
    QString playedAt;
};

struct PlaylistRecord {
    int id;
    int userId;
    QString name;
    QString createdAt;
    int songsCount;
};

struct RatingRecord {
    int id;
    int userId;
    int songId;
    int value;
    QString createdAt;
    QString updatedAt;
};

class DatabaseManager {
public:
    DatabaseManager();

    bool openDatabase();
    bool createTables();

    bool seedSongs();
    QVector<SongRecord> getAllSongs();

    bool seedAudiobooks();
    QVector<AudiobookRecord> getAllAudiobooks();

    bool seedPodcastEpisodes();
    QVector<PodcastEpisodeRecord> getAllPodcastEpisodes();

    bool addHistory(int userId, int songId);
    QVector<HistoryRecord> getUserHistory(int userId);

    bool addFavourite(int userId, int songId);
    bool removeFavourite(int userId, int songId);
    bool isFavourite(int userId, int songId);
    QVector<SongRecord> getUserFavourites(int userId);

    bool createPlaylist(int userId, const QString& name);
    QVector<PlaylistRecord> getUserPlaylists(int userId);

    bool addSongToPlaylist(int playlistId, int songId);
    bool removeSongFromPlaylist(int playlistId, int songId);
    QVector<SongRecord> getPlaylistSongs(int playlistId);
    bool deletePlaylist(int playlistId);

    bool addOrUpdateRating(int userId, int songId, int value);
    int getUserRating(int userId, int songId);
    double getAverageRating(int songId);
    QVector<RatingRecord> getUserRatings(int userId);

    bool registerUser(const QString& name,
                      const QString& surname,
                      const QString& email,
                      const QString& password,
                      const QString& accountType = "free");

    bool loginUser(const QString& email,
                   const QString& password);

    LoggedUser getLoggedUser(const QString& email,
                             const QString& password);

    bool updateUserAccountType(int userId, const QString& accountType);

private:
    QSqlDatabase database;

    QString hashPassword(const QString& password) const;
};

#endif // SPOTIFY_DATABASEMANAGER_H