#ifndef PIXBALOONTIP_H
#define PIXBALOONTIP_H

#include "baloontip.h"


class PixBaloonTip : public BaloonTip
{
public:
    PixBaloonTip();
    void setPixProperties(QPointF pos, QString r, QString g, QString b);

private:
    static const int offsetX = 10;
    static const int offsetY = 20;
    static const int width = 120;
    static const int height = 110;
};

#endif // PIXBALOONTIP_H
