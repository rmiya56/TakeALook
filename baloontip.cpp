#include "baloontip.h"
#include <QFont>
#include <QPen>


BaloonTip::BaloonTip(qreal width, qreal height)
    : QGraphicsRectItem(0, 0, width, height)
{
    //setRect(0, 0, width, height);
    textItem = new QGraphicsTextItem("", this);
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

void BaloonTip::setPos(QPointF pos)
{
    pos = QPointF(10, 20);
    QGraphicsRectItem::setPos(pos);
}

void BaloonTip::setHtml(QString html)
{
    textItem->setHtml(html);
}

void BaloonTip::setPlainText(QString text)
{
   textItem->setPlainText(text);
}