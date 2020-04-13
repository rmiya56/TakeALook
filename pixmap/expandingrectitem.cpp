#include "expandingrectitem.h"
#include <QPen>



ExpandingRectItem::ExpandingRectItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setCosmetic(true);
    setPen(pen);

    QBrush brush(QColor(0,255,0,32));
    setBrush(brush);
}

ExpandingRectItem::ExpandingRectItem(QRectF rect, QGraphicsItem *parent)
    :  ExpandingRectItem::ExpandingRectItem(parent)
{
    this->setRect(rect);
}
