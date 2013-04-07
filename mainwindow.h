#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "media_manager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void play();
    void open_and_play();
    void add_files();
    void quit();

private:
    void create_menu();
    QAction *add_menu_item(char name[], bool enabled);

    QMenu *fileMenu;
    QMenu *editMenu;
};

#endif // MAINWINDOW_H
