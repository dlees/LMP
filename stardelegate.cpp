#include "stardelegate.h"

#include "star_rating.h"
#include "stareditor.h"

// http://harmattan-dev.nokia.com/docs/platform-api-reference/xml/daily-docs/libqt4/itemviews-stardelegate.html

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
    qDebug() << "Create Editor";
      if (qVariantCanConvert<Star_Rating>(index.data())) {
          StarEditor *editor = new StarEditor(parent);
          connect(editor, SIGNAL(editingFinished()),
                  this, SLOT(commitAndCloseEditor()));
          return editor;
      } else {
          return QStyledItemDelegate::createEditor(parent, option, index);
      }
  }

void StarDelegate::commitAndCloseEditor()
{
    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void StarDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    qDebug() << "Set Model Data: Star_Rating";
    if (qVariantCanConvert<Star_Rating>(index.data())) {
        Star_Rating starRating = qVariantValue<Star_Rating>(index.data());
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        starEditor->setStarRating(starRating);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    qDebug() << "Set Model Data: Star_Rating";
    if (qVariantCanConvert<Star_Rating>(index.data())) {
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        model->setData(index, qVariantFromValue(starEditor->starRating()));
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
*/
