#ifndef AREARECTPIXMAPITEM_H
#define AREARECTPIXMAPITEM_H

#include "areaselectitem.h"
#include "expandingrectitem.h"

#include <QGraphicsRectItem>



class AreaSelectPixmapItem : public QGraphicsPixmapItem
{
public:
    AreaSelectPixmapItem();
    AreaSelectPixmapItem(QImage image);

    void setImage(QImage image);
    QRect areaRect();
    void clearAreaRect();
    void cropAreaRect();
    void setDragSelect(bool is_active);


    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF initLeftButtonPos;
    ExpandingRectItem *expandingRect = nullptr;
    AreaSelectItem *areaSelectItem = nullptr;
    bool area_selection_is_active = false;

    // QGraphicsItem interface
protected:
    //virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // AREARECTPIXMAPITEM_H
