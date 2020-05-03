#include "pixmapitem.h"
#include "../utility/mouseeventutil.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>



PixmapItem::PixmapItem()
    : QGraphicsPixmapItem()
{
}

PixmapItem::PixmapItem(QImage image)
    :  PixmapItem::PixmapItem()
{
    setImage(image);
}

void PixmapItem::setImage(QImage image)
{
    setPixmap(QPixmap::fromImage(image));
}

QRect PixmapItem::areaRect()
{
    if (areaSelectItem)
        return areaSelectItem->toQRect();
    else
        return pixmap().rect();
}

void PixmapItem::clearAreaRect()
{
    if (areaSelectItem)
    {
        delete areaSelectItem;
        areaSelectItem = nullptr;
    }
}

void PixmapItem::cropAreaRect()
{
    QPixmap cropped = pixmap().copy(areaRect());
    setPixmap(cropped);
    setPos(pos() + areaRect().topLeft());
    clearAreaRect();
}

void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "press (pixmap)";
    clearAreaRect();

    if (event->button() == Qt::LeftButton)
    {
        initLeftButtonPos = event->pos();
        expandingRect = new ExpandingRectItem(QRectF(initLeftButtonPos, initLeftButtonPos), this);
    }
    else // Right button
    {
        initLeftButtonPos = QPointF();
    }
    //QGraphicsPixmapItem::mousePressEvent(event);
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!initLeftButtonPos.isNull())
        expandingRect->setRect(QRectF(initLeftButtonPos, event->pos()));
}

void PixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "release (pixmap)";
    QGraphicsPixmapItem::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        if (!expandingRect) return;
        if (!MouseEventUtil::isValidDragMove(initLeftButtonPos, event->pos())) return;

        areaSelectItem = new AreaSelectItem(expandingRect->rect(), this);
        delete expandingRect;
        expandingRect = nullptr;
    }
}


//void PixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << "dbl click (pixmap)";
//    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
//}
