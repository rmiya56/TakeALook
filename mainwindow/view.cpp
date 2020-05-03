#include "view.h"
#include "../utility/mouseeventutil.h"
#include <QWheelEvent>
#include <QDebug>
#include <QScrollBar>



View::View(QWidget *parent)
    : QGraphicsView (parent)
{
    setBackgroundBrush(Qt::gray);
    setAlignment(Qt::AlignCenter);
}

void View::setDragScroll(bool is_active)
{
    drag_scroll_is_active = is_active;
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
   qDebug() << "press (view)";
   QGraphicsView::mousePressEvent(event);
   if (!drag_scroll_is_active) return;

   if (event->button() == Qt::LeftButton)
   {
       mousePressed = true;
       mousePos = event->pos();
       initPos = mousePos;
   }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "move (view)";
    QGraphicsView::mouseMoveEvent(event);
   if (!drag_scroll_is_active) return;

    if (mousePressed)
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
    QGraphicsView::mouseReleaseEvent(event);
   if (!drag_scroll_is_active) return;

    if(event->button() == Qt::LeftButton)
    {
        mousePressed = false;
        if (MouseEventUtil::isValidDragMove(initPos, event->pos()))
            return;
    }
}

//void View::mouseDoubleClickEvent(QMouseEvent *event)
//{
//    qDebug() << "dbl click (view)";
//    QGraphicsView::mouseDoubleClickEvent(event);
//}
