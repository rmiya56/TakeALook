#ifndef SCENE_H
#define SCENE_H

#include "areaselectionitem.h"
#include <QGraphicsScene>
#include <QMenu>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene();
    QPixmap pixmap();
    QRect areaRect();
    QRect pixmapRect();

    bool area_selection_is_active = false;
    QMenu menu_area;
    QMenu menu_scene;
    QGraphicsPixmapItem *pixmap_item = nullptr;
    AreaSelectionItem *area_item = nullptr;
    QAction *actionSelectArea;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)  override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF mouse_pos;
    QGraphicsRectItem *temp_rect = nullptr;
    QPen pen_area;
    QBrush brush_area;
    QAction *actionClear;
    QAction *actionCrop;
    QAction *actionZoom;


signals:
    void done_selection();
    void zoom_in_area(QRect rect);

private slots:
    void clearAreaRect();
    void cropAreaRect();
    void zoomInArea();

};

#endif // SCENE_H
