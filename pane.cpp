#include "pane.h"
#include <QLabel>
#include <QVBoxLayout>

#include <QFrame>
#include <QPushButton>

Pane::Pane(QString title, QAbstractItemView *items_)
{
    items = items_;

    setWindowTitle(title);
    setWidget(items);
}
