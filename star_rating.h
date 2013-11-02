#ifndef STAR_RATING_H
#define STAR_RATING_H

#include <QtGui>

class Star_Rating
{
public:
    enum EditMode { Editable, ReadOnly };

    Star_Rating(int starCount = 1, int maxStarCount = 5);

    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette, EditMode mode) const;
    QSize sizeHint() const;

    int starCount() const { return myStarCount; }
    int maxStarCount() const { return myMaxStarCount; }

    void setStarCount(int starCount) { myStarCount = starCount; }
    void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }

private:
    QPolygonF starPolygon;
    QPolygonF diamondPolygon;
    int myStarCount;
    int myMaxStarCount;
};

Q_DECLARE_METATYPE(Star_Rating)
#endif // STAR_RATING_H
