#include "scene.h"
#include "areaselectionitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>


Scene::Scene()
    : QGraphicsScene()
{
    // pen & brush
    pen_area.setColor(Qt::green);
    pen_area.setWidth(2);
    pen_area.setCosmetic(true);
    brush_area = QBrush(QColor(0,255,0,32));

    // context menu in roi
    actionCrop = new QAction(tr("Crop"));
    connect(actionCrop, SIGNAL(triggered()), this, SLOT(cropAreaRect()));
    menu_area.addAction(actionCrop);

    actionZoom = new QAction(tr("Zoom"));
    connect(actionZoom, SIGNAL(triggered()), this, SLOT(zoomInArea()));
    menu_area.addAction(actionZoom);

    actionClear = new QAction(tr("Clear"));
    connect(actionClear , SIGNAL(triggered()), this, SLOT(clearAreaRect()));
    menu_area.addAction(actionClear);
}

QPixmap Scene::pixmap()
{
    if (pixmap_item)
        return pixmap_item->pixmap();
    else
        return QPixmap();
}

QRect Scene::pixmapRect()
{
    if (pixmap_item)
        return pixmap_item->pixmap().rect();
    else
        return QRect();
}

QRect Scene::areaRect()
{
   if(area_item)
       return area_item->toQRect();
   else
       return pixmap().rect();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "press (scene)";
    QGraphicsScene::mousePressEvent(event);

    if (!area_selection_is_active) return;

    if (event->button() == Qt::LeftButton)
    {
       if(area_item) removeItem(area_item);
       mouse_pos = event->scenePos();
       temp_rect = addRect(QRectF(mouse_pos, mouse_pos));
       temp_rect->setPen(pen_area);
       temp_rect->setBrush(brush_area);
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (!area_selection_is_active) return;
    if(temp_rect)
        temp_rect->setRect(QRectF(mouse_pos, event->scenePos()));
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "release (scene)";
    QGraphicsScene::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        if (!area_selection_is_active) return;
        if (temp_rect == nullptr) return;

        qreal area_of_temp_rect = temp_rect->rect().width() * temp_rect->rect().height();
        if(area_of_temp_rect > 100)
        {
            area_item = new AreaSelectionItem(temp_rect->rect());
            addItem(area_item);
            done_selection();
        }
        removeItem(temp_rect);
        temp_rect = nullptr;
        return;
    }
    else if(event->button() == Qt::RightButton)
    {
        if(event->isAccepted()) return;
        if (!menu_scene.isEmpty())
            menu_scene.exec(event->screenPos());
    }
}

void Scene::clearAreaRect()
{
    for (auto &item : items())
    {
        if(item->type() == QGraphicsItem::UserType + 1)
            removeItem(item);
    }
}

void Scene::cropAreaRect()
{
    QPixmap original = pixmap_item->pixmap();
    QRect cropRect0; // crop rect in scene coordinate
    QRect cropRect1; // crop rect in pixmap coordinate
    cropRect0 = area_item->toQRect();
    cropRect1 = QRect(	int(cropRect0.x()-pixmap_item->x()),
                        int(cropRect0.y()-pixmap_item->y()),
                        cropRect0.width(),
                        cropRect0.height());

    QPixmap cropped = original.copy(cropRect1);
    pixmap_item->setPixmap(cropped);
    pixmap_item->setPos(cropRect0.topLeft());
    removeItem(area_item); // for cropping edge check (somewhat weird)
}

void Scene::zoomInArea()
{
    zoom_in_area(area_item->toQRect());
    clearAreaRect();
}
