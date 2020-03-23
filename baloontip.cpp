#include "baloontip.h"
#include <QFont>
#include <QPen>


BaloonTip::BaloonTip(qreal x, qreal y, qreal width, qreal height)
    : QGraphicsRectItem(x, y, width, height)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    textItem = new QGraphicsTextItem("", this);
    textItem->setPos(x, y);
    textItem->setFont(QFont("Monospace"));

    effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    setGraphicsEffect(effect);

    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setWidthF(0.5);
    setPen(pen);
    setBrush(QBrush(Qt::white, Qt::SolidPattern));
}

//void BaloonTip::setPos(QPointF pos)
//{
//    QGraphicsRectItem::setPos(pos);
//}

void BaloonTip::setHtml(QString html)
{
    textItem->setHtml(html);
}

void BaloonTip::setPlainText(QString text)
{
   textItem->setPlainText(text);
}
