#include "pane.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

Pane::Pane(QAbstractItemView *items_)
{
    items = items_;

    QVBoxLayout *layout = new QVBoxLayout();
/*    QLabel *pane = new QLabel("Pane");
    pane->setFrameStyle(QFrame::Panel | QFrame::Raised);
    pane->setLineWidth(1);
    layout->addWidget(pane);
*/
    layout->addWidget(items);

    setLayout(layout);
}
