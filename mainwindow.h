#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "media_manager.h"
#include "main_view.h"
#include "button_container.h"
#include "mini_mode.h"

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
    void switch_view_to_mini();
    void switch_view_to_main();

private:
    void create_menu();
    QAction *add_menu_item(char name[], bool enabled);

    QSplitter *splitter;

    QMenu *fileMenu;
    QMenu *editMenu;

    Main_View *mainView;
    Mini_Mode *miniView;
    QSize mainSize;
    QSize miniSize;

};

#endif // MAINWINDOW_H
