cmake_minimum_required(VERSION 4.0)

project(spotify LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(spotify
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        models.h
)

target_link_libraries(spotify
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)