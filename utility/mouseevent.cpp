#include "mouseevent.h"
#include <QtMath>



MouseEvent::MouseEvent()
{
}

bool MouseEvent::isValidDragMove(QPointF start_pos, QPointF end_pos)
{
    QPointF vector_start_to_end = end_pos - start_pos;
    qreal distance = qSqrt(QPointF::dotProduct(vector_start_to_end, vector_start_to_end));

    if (distance > MIN_DISTANCE)
    {
        return true;
    }
    else
    {
        return false;
    }
}
