#ifndef SCENE_H
#define SCENE_H

//#include "../pixmap/areaselectitem.h"
//#include "../pixmap/expandingrectitem.h"
#include "../pixmap/pixmapitem.h"
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
    void setImage(QImage image);

    //QGraphicsPixmapItem *pixmapItem = nullptr;
    PixmapItem *pixmapItem = nullptr;
    bool area_selection_is_active = false;


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)  override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;


private:
    //QPointF initPos;
    //ExpandingRectItem *expandingRect = nullptr;
    //AreaSelectItem *areaItem = nullptr;

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
