#ifndef BUTTON_CONTAINER_H
#define BUTTON_CONTAINER_H

#include <QHBoxLayout>
#include <QToolButton>
#include <QToolBar>

class Button_Container : public QToolBar
{
    Q_OBJECT

public:
    //Width, Height parameter?
    Button_Container(QWidget *parent = 0);
    ~Button_Container();

private:
    QToolButton *play;
    QToolButton *previous;
    QToolButton *next;

//public signals:


//slots:


};

#endif // BUTTON_CONTAINER_H
