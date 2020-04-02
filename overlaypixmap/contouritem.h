#ifndef CONTOURITEM_H
#define CONTOURITEM_H

#include <QGraphicsRectItem>



class ContourItem : public QGraphicsPolygonItem
{
public:
    ContourItem(const QPolygonF &polygon, QGraphicsItem *parent);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CONTOURITEM_H
