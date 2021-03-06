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

    void save_window_settings(const QString &mode);
    void restore_window_settings(const QString &mode);
public slots:
    void play();
    void open_and_play();
    void add_files();
    void quit();
    void switch_view_to_mini();
    void switch_view_to_main();
    void keyPressEvent(QKeyEvent *);

private:
    void create_menu();
    QAction *add_menu_item(char name[], bool enabled, QMenu *menu);

    QSplitter *splitter;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *hsMenu;
    QMenu *plMenu; //playlist
    QMenu *viewMenu;

    Main_View *mainView;
    Mini_Mode *miniView;
    QSize mainSize;
    QSize miniSize;

};

#endif // MAINWINDOW_H
