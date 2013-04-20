#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H


#include <QMainWindow>

#include "media_manager.h"

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
    void play();
    void open_and_play();
    void add_files();
    void quit();
    void send_mini_mode();
    void new_create_playlist_files(QString name);


    // when you double click on playlist pane: play the song
    void select_song(QModelIndex);

    // when you click on lib pane: put that list in center
    void select_lib_item(QModelIndex);

    // when you double click on lib pane: play that playlist
    void open_playlist_from_lib(QModelIndex);

    // when you double click on table: play that item, load list into playlist
    void select_center_item(QModelIndex);

    void create_playlist_files();
    void delete_playlist_from_library();

    void remove_selected_song();

private:
    QStringList* get_files();
    QAction *add_menu_item(char name[], bool enabled);

    QSplitter *splitter;

    QMenu *fileMenu;
    QMenu *editMenu;
    QPushButton *MiniMode;
    QListView *playlist;
    QTableView *table;
    QListView *lib_list;
};


#endif // MAIN_VIEW_H
