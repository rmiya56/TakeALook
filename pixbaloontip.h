#ifndef PIXBALOONTIP_H
#define PIXBALOONTIP_H

#include "baloontip.h"


class PixBaloonTip : public BaloonTip
{
public:
    PixBaloonTip();
    //void setPixCoordiantes(QPointF pos);
    //void setColors(qreal r, qreal g, qreal b);

    void setPixProperties(QPointF pos, qreal r, qreal g, qreal b);
private:
    static const int width = 120;
    static const int height = 110;
};

#endif // PIXBALOONTIP_H
