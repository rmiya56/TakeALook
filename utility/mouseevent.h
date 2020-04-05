#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QPointF>



class MouseEvent
{
public:
    MouseEvent();
    static bool isValidDragMove(QPointF start_pos, QPointF end_pos);

private:
    static const int MIN_DISTANCE = 3;
};

#endif // MOUSEEVENT_H
