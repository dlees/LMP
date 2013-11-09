#ifndef STAREDITOR_H
#define STAREDITOR_H

#include "star_rating.h"

class StarEditor : public QWidget
{
    Q_OBJECT

public:
    StarEditor(QWidget *parent = 0);

    QSize sizeHint() const;
    void setStarRating(const Star_Rating &starRating) {
        myStarRating = starRating;
    }
    Star_Rating starRating() { return myStarRating; }

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int starAtPosition(int x);

    Star_Rating myStarRating;
};
#endif // STAREDITOR_H
