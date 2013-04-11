#ifndef MINI_MODE_H
#define MINI_MODE_H



#include <QMainWindow>

#include "media_manager.h"

class Mini_Mode : public QMainWindow
{
    Q_OBJECT

public:
    Mini_Mode(QWidget *parent = 0);
    ~Mini_Mode();

signals:
    void to_main_mode();

public slots:
    void play();
    void open_and_play();
    void add_files();
    void quit();
    void send_to_main();

private:
    void create_menu();
    QAction *add_menu_item(char name[], bool enabled);

    QSplitter *splitter;

    QMenu *fileMenu;
    QMenu *editMenu;
    QPushButton *MainMode;
};


#endif // MINI_MODE_H
