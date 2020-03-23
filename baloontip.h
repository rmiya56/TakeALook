#ifndef BALOONTIP_H
#define BALOONTIP_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsTextItem>


class BaloonTip : public QGraphicsRectItem
{

public:
    BaloonTip();
    BaloonTip(qreal width, qreal height);
    void setHtml(QString html);
    void setPlainText(QString text);

protected:
    QGraphicsTextItem *textItem = nullptr;
    void setPos(QPointF pos);

private:
    QGraphicsDropShadowEffect *effect;

};

#endif //  BALOONTIP_H
