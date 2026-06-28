cmake_minimum_required(VERSION 4.0)

project(spotify LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

add_executable(spotify
        main.cpp

        gui/mainwindow.h
        gui/mainwindow.cpp
        gui/mainwindow.ui

        gui/login.h
        gui/login.cpp
        gui/login.ui

        gui/registerdialog.h
        gui/registerdialog.cpp
        gui/registerdialog.ui

        models/Playable.h
        models/Song.h
        models/PodcastEpisode.h
        models/Audiobook.h
        models/Playlist.h
        models/SmartPlaylist.h
        models/FavoritePlaylist.h
        models/User.h
        models/FreeUser.h
        models/PremiumUser.h
        models/Player.h
        models/Queue.h
        models/History.h
        models/Rating.h

        controllers/PlaylistController.cpp
        controllers/PlaylistController.h
        controllers/PlayerController.cpp
        controllers/PlayerController.h
        controllers/UserController.cpp
        controllers/UserController.h
        database/DatabaseManager.cpp
        database/DatabaseManager.h
        controllers/RatingController.cpp
        controllers/RatingController.h
)

target_link_libraries(spotify
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)