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

public slots:
    void play();
    void open_and_play();
    void add_files();
    void quit();
    void send_mini_mode();

private:
    QAction *add_menu_item(char name[], bool enabled);

    QSplitter *splitter;

    QMenu *fileMenu;
    QMenu *editMenu;
    QPushButton *MiniMode;
};


#endif // MAIN_VIEW_H
