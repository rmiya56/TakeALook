#ifndef BALOONTIP_H
#define BALOONTIP_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsTextItem>


class BaloonTip : public QGraphicsRectItem
{

public:
    BaloonTip(qreal x, qreal y, qreal width, qreal height);
    void setHtml(QString html);
    void setPlainText(QString text);

protected:
    QGraphicsTextItem *textItem = nullptr;

private:
    QGraphicsDropShadowEffect *effect;

};

#endif //  BALOONTIP_H
