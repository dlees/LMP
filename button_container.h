#ifndef BUTTON_CONTAINER_H
#define BUTTON_CONTAINER_H

#include <QHBoxLayout>
#include <QToolButton>
#include <QToolBar>

class Button_Container : public QToolBar
{
    Q_OBJECT

public:
    Button_Container(int w, int h);
    ~Button_Container();

private:
    QToolButton *playPause;
    QToolButton *previous;
    QToolButton *next;

public slots:
    void set_icon_play();
    void set_icon_pause();

signals:

};

#endif // BUTTON_CONTAINER_H
