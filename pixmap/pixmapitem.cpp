#include "pixmapitem.h"

#include <QGraphicsSceneMouseEvent>
#include "../utility/mouseeventutil.h"



PixmapItem::PixmapItem()
    : QGraphicsPixmapItem()
{
    expandingRect = new ExpandingRectItem(this);
    areaRect = new AreaSelectItem(this);
}

PixmapItem::PixmapItem(QImage image)
    :  PixmapItem::PixmapItem()
{
   setImage(image);
}


void PixmapItem::setImage(QImage image)
{
    this->setPixmap(QPixmap::fromImage(image));
}


void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    initPos = event->scenePos();
    if (event->button() == Qt::LeftButton)
       expandingRect->show();
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (expandingRect->isVisible())
        expandingRect->setRect(QRectF(initPos, event->scenePos()));
}

void PixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        if (!expandingRect->isVisible()) return;
        if (!MouseEventUtil::isValidDragMove(initPos, event->scenePos())) return;

        delete areaRect;
        areaRect = new AreaSelectItem(expandingRect->rect(), this);
        expandingRect->hide();
        //done_selection(true);
    }

}
