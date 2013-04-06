#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T22:56:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LMP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    song.cpp \
    playing_song.cpp \
    music_player.cpp \
    music_item.cpp \
    collection.cpp \
    media_manager.cpp \
    database.cpp \
    library.cpp \
    playlist.cpp \
    button_container.cpp \
    play_controller.cpp \
    pane.cpp

HEADERS  += mainwindow.h \
    song.h \
    playing_song.h \
    music_player.h \
    music_item.h \
    error.h \
    collection.h \
    media_manager.h \
    database.h \
    library.h \
    playlist.h \
    button_container.h \
    play_controller.h \
    pane.h

OTHER_FILES +=

RESOURCES += \
    icons/buttons.qrc