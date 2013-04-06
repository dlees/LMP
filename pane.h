#ifndef PANE_H
#define PANE_H

#include <QAbstractItemView>

class Pane : public QWidget
{
    Q_OBJECT

public:
    Pane(QAbstractItemView *items_);

private:
    QAbstractItemView *items;
};

#endif // PANE_H
