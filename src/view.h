#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QGraphicsView>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    bool mouse_control = true;

protected:
    void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QPointF sceneMousePos;
    QPointF initMousePos;
    bool right_mouse_pressed = false;
    const qreal zoomFactor = 1.05;
    bool isInSelectedArea(QPoint p);
};
#endif // VIEW_H
