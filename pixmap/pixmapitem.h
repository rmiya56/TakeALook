#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include "areaselectitem.h"
#include "expandingrectitem.h"

#include <QGraphicsRectItem>



class PixmapItem : public QGraphicsPixmapItem
{
public:
    PixmapItem();
    PixmapItem(QImage image);

    void setImage(QImage image);
    QRect areaRect();
    void clearAreaRect();

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF initPos;
    ExpandingRectItem *expandingRect = nullptr;
    AreaSelectItem *areaSelectItem = nullptr;
};

#endif // PIXMAPITEM_H
