#include "expandingrectitem.h"
#include <QPen>



ExpandingRectItem::ExpandingRectItem(QRectF rect)
    : QGraphicsRectItem(rect)
{
    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setCosmetic(true);

    QBrush brush(QColor(0,255,0,32));

    setPen(pen);
    setBrush(brush);
}
