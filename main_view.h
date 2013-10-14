#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H


#include <QMainWindow>
#include <QtGui>
#include "qcustomplot.h"

class Pane;
class Collection;


class Main_View : public QMainWindow
{
    Q_OBJECT

public:
    Main_View(QWidget *parent = 0);
    ~Main_View();

signals:
    void to_mini_mode();
    void got_name(QString name);

public slots:
    void create_library_pane();
    void create_center_pane();
    void create_playlist_pane();
    void create_graph();
    void play();
    void open_and_play();
    void add_files();
    void quit();
    void send_mini_mode();
    void new_create_playlist_files(QString name);
    void update_center(Collection *new_center);
    void update_playlist(Collection *collection);

    // when you double click on playlist pane: play the song
    void select_song(QModelIndex);

    // when you click on lib pane: put that list in center
    void select_lib_item(QModelIndex);

    // when you double click on lib pane: play that playlist
    void open_playlist_from_lib(QModelIndex);

    // when you double click on table: play that item, load list into playlist
    void select_center_item(QModelIndex);

    void add_selected_to_playlist();
    void create_playlist_files();
    void delete_playlist_from_library();

    void remove_selected_song();
    void remove_selected_from_table();

    void create_catalog();
    void add_selected_lib_list_to_catalog();

private:
    QStringList* get_files();
    QAction *add_menu_item(char name[], bool enabled);

    QSplitter *splitter;

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolButton *MiniMode;
    QListView *playlist;
    QTableView *table;
    QListView *lib_list;

    Pane *libraryPane;
    Pane *centerPane;
    Pane *playlistPane;

    QCustomPlot *customPlot;
};

#endif // MAIN_VIEW_H
