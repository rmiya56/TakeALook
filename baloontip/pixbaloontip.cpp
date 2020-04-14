#include "pixbaloontip.h"
#include <QFont>
#include <QtMath>


PixBaloonTip::PixBaloonTip()
    : BaloonTip(offsetX, offsetY, width, height)
{
    textItem->setFont(QFont("Monospace"));
}

void PixBaloonTip::setPixProperties(QPointF pos, QStringList color)
{

    setPos(pos);
    QString x = QString::number(qFloor(pos.x()));
    QString y = QString::number(qFloor(pos.y()));
    QString coordinates = QString("[%1, %2]</br>").arg(x, y);

    QString r, g, b;
    switch(color.size())
    {
        case 1:
            r = color[0];
            break;
        case 3:
            r = color[0];
            g = color[1];
            b = color[2];
            break;
        default:
            break;
    }

    QString html = "<div align='center'><pre><tt>"
                   "<font size='1'>%1</font><br/>"
                   "<font color='red'   size='1'> r:%2</font><br/>"
                   "<font color='green' size='1'> g:%3</font><br/>"
                   "<font color='blue'  size='1'> b:%4</font><br/>"
                   "</pre></tt></div>";

    html = QString(html).arg(coordinates, r, g, b);
    setHtml(html);
}
