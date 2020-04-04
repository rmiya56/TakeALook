#ifndef SCENE_H
#define SCENE_H

#include "areaselectionitem.h"
#include "takealook/baloontip/pixbaloontip.h"
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

    bool area_selection_is_active = false;
    QMenu menuArea;
    QMenu menuScene;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    AreaSelectionItem *areaItem = nullptr;
    PixBaloonTip *baloonTip = nullptr;

    QAction *actionSelectArea;


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)  override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF mousePos;
    QGraphicsRectItem *tempRect = nullptr;
    QPen penArea;
    QBrush brushArea;
    QAction *actionClear;
    QAction *actionCrop;
    QAction *actionZoom;

signals: // to MainWindow
    void done_selection(bool);
    void zoom_in_area(QRect rect);

signals:  // to Scene
    void key_delete();

private slots:
    void clear_area_rect();
    void crop_area_rect();
    void zoom_in_area();


    // QGraphicsScene interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // SCENE_H
