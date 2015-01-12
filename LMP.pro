#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T22:56:39
#
#-------------------------------------------------

QT       += core gui phonon xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LMP
TEMPLATE = app

LIBS += -L/usr/lib

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
    pane.cpp \
    main_view.cpp \
    mini_mode.cpp \
    mini_mode_controller.cpp \
    hotspots.cpp \
    qcustomplot.cpp \
    catalog.cpp \
    star_rating.cpp \
    stardelegate.cpp \
    datapoint.cpp \
    datalist.cpp \
    datalistdecorator.cpp \
    datapointcomparers.cpp \
    comparitor.cpp \
    megamix.cpp \
    dynamicplaylist.cpp


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
    pane.h \
    main_view.h \
    mini_mode.h \
    mini_mode_controller.h \
    hotspots.h \
    qcustomplot.h \
    catalog.h \
    star_rating.h \
    stardelegate.h \
    datapoint.h \
    datalist.h \
    datalistdecorator.h \
    datapointcomparers.h \
    comparitor.h \
    megamix.h \
    dynamicplaylist.h

OTHER_FILES +=


RESOURCES += \
    icons/buttons.qrc \
