#ifndef EXPANDINGRECTITEM_H
#define EXPANDINGRECTITEM_H

#include <QGraphicsRectItem>



class ExpandingRectItem : public QGraphicsRectItem
{
public:
    ExpandingRectItem(QGraphicsItem *parent=nullptr);
    ExpandingRectItem(QRectF rect, QGraphicsItem *parent=nullptr);

private:
};


#endif // EXPANDINGRECTITEM_H
