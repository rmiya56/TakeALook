#ifndef SCENE_H
#define SCENE_H

#include "../areaselectpixmap/areaselectpixmapitem.h"
#include "../image/imagehandler.h"
#include <QGraphicsScene>
#include <QMenu>



class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(ImageHandler *image_handler);
    QPixmap pixmap();
    QRect areaRect();
    QRect pixmapRect();
    void setImage(QImage image);

    QGraphicsPixmapItem *pixmapItem = nullptr;
    ImageHandler *imageHandler;

    //AreaSelectPixmapItem *pixmapItem = nullptr;
    //bool area_selection_is_active = false;


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)  override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;


private:
    QMenu menuArea;
    QAction *actionSelectArea;

    QMenu menuScene;
    QAction *actionClear;
    QAction *actionCrop;
    QAction *actionZoom;
    QAction *actionSaveROI;


signals: // to MainWindow
    void done_selection(bool);
    void zoom_in_area(QRect rect);

private slots:
    void clear_area_rect();
    void crop_area_rect();
    void zoom_in_area();
    void save_roi();

};

#endif // SCENE_H
