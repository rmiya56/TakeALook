#include "contouritem.h"
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QDebug>


ContourItem::ContourItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsPolygonItem(polygon, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}


void ContourItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isSelected())
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(QColor(0,255,0,100)));
        painter->drawPolygon(polygon());
    }
}
