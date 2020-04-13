#include "scene.h"
//#include "../pixmap/areaselectitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include "../utility/mouseeventutil.h"
#include "../utility/shapetojson.h"
#include "../pixmap/pixmapitem.h"



Scene::Scene()
    : QGraphicsScene()
{

    actionCrop = new QAction(tr("Crop"));
    connect(actionCrop, SIGNAL(triggered()), this, SLOT(crop_area_rect()));
    menuArea.addAction(actionCrop);

    actionZoom = new QAction(tr("Zoom"));
    connect(actionZoom, SIGNAL(triggered()), this, SLOT(zoom_in_area()));
    menuArea.addAction(actionZoom);

    actionClear = new QAction(tr("Clear"));
    connect(actionClear , SIGNAL(triggered()), this, SLOT(clear_area_rect()));
    menuArea.addAction(actionClear);

    actionSaveROI = new QAction(tr("Save ROI"));
    connect(actionSaveROI, SIGNAL(triggered()), this, SLOT(save_roi()));
    menuArea.addAction(actionSaveROI);
}

void Scene::setImage(QImage image)
{
    pixmapItem = new PixmapItem(image);
    addItem(pixmapItem);
}

QPixmap Scene::pixmap()
{
    if (pixmapItem)
        return pixmapItem->pixmap();
    else
        return QPixmap();
}

QRect Scene::pixmapRect()
{
    if (pixmapItem)
        return pixmapItem->pixmap().rect();
    else
        return QRect();
}

QRect Scene::areaRect()
{
    return pixmapItem->areaRect();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "press (scene)";
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "release (scene)";
    QGraphicsScene::mouseReleaseEvent(event);

    if(event->button() == Qt::RightButton)
    {
        QGraphicsItem *item = this->itemAt(event->scenePos(), QTransform());
        if (item && item->type() == QGraphicsItem::UserType + 1)
        {
            menuArea.exec(event->screenPos());
        }
        else if (!menuScene.isEmpty())
        {
            menuScene.exec(event->screenPos());
        }
    }
}

void Scene::clear_area_rect()
{
    for (auto &item : items())
    {
        if(item->type() == QGraphicsItem::UserType + 1)
            removeItem(item);
    }
}

void Scene::crop_area_rect()
{
    //QRect cropRect0 = areaItem->toQRect();
    QRect cropRect0 = pixmapItem->areaRect();
    QRect cropRect1 = QRect(cropRect0);
    cropRect1.translate(-pixmapItem->pos().toPoint());
    QPixmap pixmap = pixmapItem->pixmap();
    QPixmap cropped = pixmap.copy(cropRect1);
    pixmapItem->setPixmap(cropped);
    pixmapItem->setPos(cropRect0.topLeft());
    pixmapItem->clearAreaRect();
}

void Scene::zoom_in_area()
{
    zoom_in_area(pixmapItem->areaRect());
    clear_area_rect();
}

void Scene::save_roi()
{
    //ShapeToJson::saveRect(areaItem->toQRect(), file_path);
}


void Scene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
}
