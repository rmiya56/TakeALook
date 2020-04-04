#include "view.h"
//#include "scene.h"
#include <QWheelEvent>
#include <QDebug>
#include <QScrollBar>



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
       sceneMousePos = event->pos();
       return;
   }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "move (view)";
    QGraphicsView::mouseMoveEvent(event);

    if (right_mouse_pressed)
    {
        int dx = horizontalScrollBar()->value() - (event->x() - sceneMousePos.x());
        int dy = verticalScrollBar()->value() - (event->y() - sceneMousePos.y());
        horizontalScrollBar()->setValue(dx);
        verticalScrollBar()->setValue(dy);
        sceneMousePos = event->pos();
        event->accept();
        return;
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "release (view)";
    QGraphicsView::mouseReleaseEvent(event);

    if(event->button() == Qt::RightButton)
    {
        right_mouse_pressed = false;
    }
}


void View::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}
