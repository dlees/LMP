#ifndef PANE_H
#define PANE_H

#include <QAbstractItemView>
#include <QDockWidget>
#include <QGroupBox>
#include <QLabel>
#include "collection.h"

class Pane : public QFrame //QDockWidget
{
    Q_OBJECT

public:
    Pane(QString title, QAbstractItemView *items_);
    ~Pane();

private:
    QAbstractItemView *items;
    QLabel *title;

public slots:
    void set_title(Collection *new_center);
};

#endif // PANE_H
