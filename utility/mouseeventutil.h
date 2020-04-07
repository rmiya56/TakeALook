#ifndef MOUSEEVENTUTIL_H
#define MOUSEEVENTUTIL_H

#include <QPointF>



class MouseEventUtil
{
public:
    MouseEventUtil();
    static bool isValidDragMove(QPointF start_pos, QPointF end_pos);

private:
    static const int MIN_DISTANCE = 3;
};

#endif // MOUSEEVENTUTIL_H
