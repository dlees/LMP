#ifndef PANE_H
#define PANE_H

#include <QAbstractItemView>
#include <QDockWidget>
#include <QGroupBox>

class Pane : public QFrame //QDockWidget
{
    Q_OBJECT

public:
    Pane(QString title, QAbstractItemView *items_);

private:
    QAbstractItemView *items;
};

#endif // PANE_H
