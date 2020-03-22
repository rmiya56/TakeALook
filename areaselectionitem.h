#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsRectItem>
//#include <QPen>


class AreaSelectionItem : public QGraphicsItem
{
public:
    AreaSelectionItem(qreal x, qreal y, qreal width, qreal height);
    AreaSelectionItem(QRectF rect);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    QRect toQRect();

    void setRect(QRectF &rectangle);
    void setRect(qreal x, qreal y, qreal width, qreal height);
    QRectF rect();


protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual QRectF boundingRect() const override;

private:
    void updateMargins();
    qreal calcMargin() const;


    QRectF *area_rect = nullptr;
    QRectF *top, *left, *right, *bottom;
    QPointF prev;
    QColor color = Qt::green;
    bool drag_moving = false;
    QMap<QString, bool> drag_resizing {	{"right", false}, {"left", false},
                                        {"top", false}, {"bottom", false},
                                        {"topLeft", false}, {"topRight", false},
                                        {"bottomLeft", false}, {"bottomRight", false}
                                     };

    int margin_percentage = 5;
    bool show_margin = false;

};

#endif // ITEM_H
