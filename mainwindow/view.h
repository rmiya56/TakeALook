#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QGraphicsView>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    QPointF sceneMousePos;
    QPointF initMousePos;
    bool right_mouse_pressed = false;
    static constexpr qreal zoomFactor = 1.05;


};
#endif // VIEW_H
