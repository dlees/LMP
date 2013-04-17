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
    title = new QLabel(title_);
    QVBoxLayout *main_layout = new QVBoxLayout();

    main_layout->addWidget(title);
    main_layout->addWidget(items);
    setLayout(main_layout);
   // setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void Pane::set_title(Collection *new_center)
{
    title->setText(new_center->get_name());
}

Pane::~Pane()
{
    delete items;
    delete title;
}
