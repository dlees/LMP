#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T22:56:39
#
#-------------------------------------------------

QT       += core gui += phonon

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
    pane.cpp \
    XML/tinystr.cpp \
    XML/tinyxml.cpp \
    XML/tinyxml.h \
    XML/tinystr.h \
    XML/tinyxmlerror.cpp \
    XML/tinyxmlparser.cpp \
    XML/action_store.cpp \
    XML/xpath_expression.h \
    XML/action_store.h \
    XML/xpath_processor.cpp \
    XML/byte_stream.h \
    XML/xpath_processor.h \
    XML/htmlutil.cpp \
    XML/xpath_stack.cpp \
    XML/htmlutil.h \
    XML/xpath_stack.h \
    XML/lex_token.h \
    XML/tinyxpath_conf.h \
    XML/xpath_static.cpp \
    XML/lex_util.cpp \
    XML/tokenlist.cpp \
    XML/xpath_static.h \
    XML/lex_util.h \
    XML/tokenlist.h \
    XML/xpath_stream.cpp \
    XML/node_set.cpp \
    XML/xml_util.cpp \
    XML/xpath_stream.h \
    XML/node_set.h \
    XML/xml_util.h \
    XML/xpath_syntax.cpp \
    XML/xpath_expression.cpp \
    XML/xpath_syntax.h


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
    XML/tinyxml.h \
    XML/xpath_processor.h

OTHER_FILES +=


RESOURCES += \
    icons/buttons.qrc \
