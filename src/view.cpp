#include "view.h"
#include "scene.h"
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
   qDebug() << "press (view)";
   QGraphicsView::mousePressEvent(event);
   if (!mouse_control) return;

   if (isInSelectedArea(event->pos())) return;

   if (event->button() == Qt::LeftButton)
   {
       left_mouse_pressed = true;
       sceneMousePos = event->pos();
       return;
   }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "move (view)";
    QGraphicsView::mouseMoveEvent(event);
    if (!mouse_control) return;

    if (left_mouse_pressed)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x()-sceneMousePos.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y()-sceneMousePos.y()));
        sceneMousePos = event->pos();
        event->accept();
        return;
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "release (view)";
    QGraphicsView::mouseReleaseEvent(event);
    if (!mouse_control) return;

    if (isInSelectedArea(event->pos()))
    {    // in the selected area
        if(event->button() == Qt::RightButton)
        {
            static_cast<Scene*>(scene())->menuArea.exec(event->globalPos());
            event->accept();
        }
    }
    else
    {    // out of the selected area
        if(event->button() == Qt::LeftButton)
        {
            left_mouse_pressed = false;
        }
    }
}

bool View::isInSelectedArea(QPoint p)
{
    QGraphicsItem *item = itemAt(p);
    return (item && item->type() == QGraphicsItem::UserType + 1);
}
