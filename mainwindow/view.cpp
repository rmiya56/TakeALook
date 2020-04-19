#include "view.h"
#include <QWheelEvent>
#include <QDebug>
#include <QScrollBar>
#include "../utility/mouseeventutil.h"



View::View(QWidget *parent)
    : QGraphicsView (parent)
{
    setBackgroundBrush(Qt::gray);
    setAlignment(Qt::AlignCenter);
}

void View::wheelEvent(QWheelEvent *event)
{
    if(event->isAccepted()) return;
    if(event->angleDelta().y() > 0) scale(zoomFactor, zoomFactor);
    else scale(1/zoomFactor, 1/zoomFactor);
    event->accept();
}

void View::mousePressEvent(QMouseEvent *event)
{
   //qDebug() << "press (view)";
   QGraphicsView::mousePressEvent(event);

   if (event->button() == Qt::RightButton)
   {
       right_mouse_pressed = true;
       mousePos = event->pos();
       initPos = mousePos;
   }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "move (view)";
    QGraphicsView::mouseMoveEvent(event);

    if (right_mouse_pressed)
    {
        int dx = horizontalScrollBar()->value() - (event->x() - mousePos.x());
        int dy = verticalScrollBar()->value() - (event->y() - mousePos.y());
        horizontalScrollBar()->setValue(dx);
        verticalScrollBar()->setValue(dy);
        mousePos = event->pos();
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "release (view)";

    if(event->button() == Qt::RightButton)
    {
        right_mouse_pressed = false;
        if (MouseEventUtil::isValidDragMove(initPos, event->pos()))
            return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}


void View::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}
