#include "areaselectpixmapitem.h"
#include "../utility/mouseeventutil.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>



AreaSelectPixmapItem::AreaSelectPixmapItem()
    : QGraphicsPixmapItem()
{
}

AreaSelectPixmapItem::AreaSelectPixmapItem(QImage image)
    :  AreaSelectPixmapItem::AreaSelectPixmapItem()
{
    setImage(image);
}

void AreaSelectPixmapItem::setImage(QImage image)
{
    setPixmap(QPixmap::fromImage(image));
}

QRect AreaSelectPixmapItem::areaRect()
{
    if (areaSelectItem)
        return areaSelectItem->toQRect();
    else
        return pixmap().rect();
}

void AreaSelectPixmapItem::clearAreaRect()
{
    if (areaSelectItem)
    {
        delete areaSelectItem;
        areaSelectItem = nullptr;
    }
}

void AreaSelectPixmapItem::cropAreaRect()
{
    QPixmap cropped = pixmap().copy(areaRect());
    setPixmap(cropped);
    setPos(pos() + areaRect().topLeft());
    clearAreaRect();
}

void AreaSelectPixmapItem::setDragSelect(bool is_active)
{
   area_selection_is_active = is_active;
}

void AreaSelectPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "press (pixmap)";
    clearAreaRect();
    if(!area_selection_is_active) return;

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

void AreaSelectPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!area_selection_is_active) return;

    if (!initLeftButtonPos.isNull())
        expandingRect->setRect(QRectF(initLeftButtonPos, event->pos()));
}

void AreaSelectPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "release (pixmap)";
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    if(!area_selection_is_active) return;

    if(event->button() == Qt::LeftButton)
    {
        if (!expandingRect) return;
        if (!MouseEventUtil::isValidDragMove(initLeftButtonPos, event->pos())) return;

        areaSelectItem = new AreaSelectItem(expandingRect->rect(), this);
        delete expandingRect;
        expandingRect = nullptr;
    }
}

