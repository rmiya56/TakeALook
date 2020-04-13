#include "areaselectitem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QtMath>
#include <QDebug>


AreaSelectItem::AreaSelectItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

AreaSelectItem::AreaSelectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : AreaSelectItem::AreaSelectItem(parent)
{
    setRect(x, y, width, height);
}

AreaSelectItem::AreaSelectItem(QRectF rect, QGraphicsItem *parent)
    : AreaSelectItem(rect.x(), rect.y(), rect.width(), rect.height(), parent)
{
}

qreal AreaSelectItem::calcMargin() const
{
    qreal margin = qMin(areaRect->width(), areaRect->height()) * (marginPercentage/100.0);
    return margin;
    //return qMin(5.0, margin);
}

QRectF AreaSelectItem::boundingRect() const
{
    if(!areaRect) return QRectF();
    qreal margin = calcMargin();
    return areaRect->marginsAdded(QMarginsF(margin, margin, margin, margin));
}

void AreaSelectItem::setRect(QRectF &rectangle)
{
    areaRect = &rectangle;
    updateMargins();
}

void AreaSelectItem::setRect(qreal x, qreal y, qreal width, qreal height)
{
    areaRect = new QRectF(x, y, width, height);
    updateMargins();
}

QRectF AreaSelectItem::rect()
{
    return *areaRect;
}

void AreaSelectItem::updateMargins()
{
    qreal width = areaRect->width();
    qreal height = areaRect->height();
    qreal margin = calcMargin();

    qreal x0 = areaRect->topLeft().x();
    qreal y0 = areaRect->topLeft().y();
    qreal x1 = areaRect->topRight().x();
    qreal y1 = areaRect->topRight().y();
    qreal x2 = areaRect->bottomLeft().x();
    qreal y2 = areaRect->bottomLeft().y();

    top = new QRectF(QPointF(x0-margin, y0-margin), QPointF(x1+margin, y1+margin));
    bottom = new QRectF(*top);
    bottom->translate(0, height);
    left = new QRectF(QPointF(x0-margin, y0-margin), QPointF(x2+margin, y2+margin));
    right = new QRectF(*left);
    right->translate(width, 0);
}

void AreaSelectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!areaRect) return;

    QPen pen(color);
    pen.setCosmetic(true);
    painter->setPen(pen);

    if (isSelected()) painter->setBrush(QBrush(QColor(0,255,0,100)));

    painter->drawRect(*areaRect);

    if (show_margin)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::red);
        painter->drawRect(*top);
        painter->drawRect(*left);
        painter->drawRect(*right);
        painter->drawRect(*bottom);
    }
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRect AreaSelectItem::toQRect()
{
    QRectF r = *areaRect;
    int x0 = qCeil(r.x());
    int y0 = qCeil(r.y());
    int x1 = qFloor(r.bottomRight().x());
    int y1 = qFloor(r.bottomRight().y());
    return QRect(QPoint(x0, y0), QPoint(x1, y1));
}

void AreaSelectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //QGraphicsItem::mousePressEvent(event);
    //qDebug() << "press (item)";

    if (event->button() != Qt::LeftButton) return;

    QPointF p = event->pos();
    if (top->intersected(*left).contains(p))
        drag_resizing["topLeft"] = true;

    else if (top->intersected(*right).contains(p))
        drag_resizing["topRight"] = true;

    else if (bottom->intersected(*left).contains(p))
        drag_resizing["bottomLeft"] = true;

    else if (bottom->intersected(*right).contains(p))
        drag_resizing["bottomRight"] = true;

    else if (right->contains(p))
        drag_resizing["right"] = true;

    else if (left->contains(p))
        drag_resizing["left"] = true;

    else if (top->contains(p))
        drag_resizing["top"] = true;

    else if (bottom->contains(p))
        drag_resizing["bottom"] = true;

    else if (contains(p))
    {
        prev = p;
        drag_moving = true;
    }
    else
    {
        drag_moving = false;
        for (auto &flag : drag_resizing) flag = false;
    }
}

void AreaSelectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "move (item)";
    QPointF p = event->pos();

    if (drag_moving)
    {
        moveBy(event->pos().x()-prev.x(), event->pos().y()-prev.y());
        return;
    }

    if (drag_resizing["topLeft"])                areaRect->setTopLeft(p);
    else if (drag_resizing["topRight"])          areaRect->setTopRight(p);
    else if (drag_resizing["bottomLeft"])        areaRect->setBottomLeft(p);
    else if (drag_resizing["bottomRight"])       areaRect->setBottomRight(p);
    else if (drag_resizing["right"])             areaRect->setRight(p.x());
    else if (drag_resizing["left"])              areaRect->setLeft(p.x());
    else if (drag_resizing["top"])               areaRect->setTop(p.y());
    else if (drag_resizing["bottom"])            areaRect->setBottom(p.y());

    updateMargins();
    update();
}

void AreaSelectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "release (item)";
    drag_moving = false;
    for (auto &flag : drag_resizing) flag = false;
    Q_UNUSED(event)
}

void AreaSelectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF p = event->pos();

    if (top->intersected(*right).contains(p) || bottom->intersected(*left).contains(p))
        setCursor(Qt::SizeBDiagCursor);

    else if (top->intersected(*left).contains(p) || bottom->intersected(*right).contains(p))
        setCursor(Qt::SizeFDiagCursor);

    else if (right->contains(p) || left->contains(p))
        setCursor(Qt::SizeHorCursor);

    else if (top->contains(p) || bottom->contains(p))
        setCursor(Qt::SizeVerCursor);

    else if (contains(p))
        setCursor(Qt::SizeAllCursor);
}

