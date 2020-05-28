#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QGraphicsView>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    void setDragScroll(bool is_active);


protected:
    void wheelEvent(QWheelEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPointF mousePos;
    QPointF initPos;
    bool drag_scroll_is_active;
    bool mousePressed = false;
    static constexpr qreal zoomFactor = 1.05;

};
#endif // VIEW_H
