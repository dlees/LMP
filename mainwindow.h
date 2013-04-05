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

private:
    void setupMenu();

};

#endif // MAINWINDOW_H
