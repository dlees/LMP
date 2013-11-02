#include "stardelegate.h"

#include "star_rating.h"

void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if (qVariantCanConvert<Star_Rating>(index.data()))
    {
        Star_Rating star_rating = qVariantValue<Star_Rating>(index.data());

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        star_rating.paint(painter, option.rect, option.palette,
                         Star_Rating::ReadOnly);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
   {
       if (qVariantCanConvert<Star_Rating>(index.data())) {
           Star_Rating star_rating = qVariantValue<Star_Rating>(index.data());
           return star_rating.sizeHint();
       } else {
           return QStyledItemDelegate::sizeHint(option, index);
       }
   }
/*
QWidget *StarDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const

  {
      if (qVariantCanConvert<Star_Rating>(index.data())) {
          StarEditor *editor = new StarEditor(parent);
          connect(editor, SIGNAL(editingFinished()),
                  this, SLOT(commitAndCloseEditor()));
          return editor;
      } else {
          return QStyledItemDelegate::createEditor(parent, option, index);
      }
  }*/
