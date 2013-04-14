#include "pane.h"
#include <QLabel>
#include <QVBoxLayout>

#include <QFrame>
#include <QPushButton>
#include <QLabel>

Pane::Pane(QString title_, QAbstractItemView *items_)
{
    items = items_;

    //setWindowTitle(title);
    //setWidget(items);
    QLabel *title = new QLabel(title_);
    QVBoxLayout *main_layout = new QVBoxLayout();

    main_layout->addWidget(title);
    main_layout->addWidget(items);
    setLayout(main_layout);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

}
