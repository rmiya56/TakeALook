#ifndef OVERLAYPIXMAPTOOLBAR_H
#define OVERLAYPIXMAPTOOLBAR_H

#include "overlaypixmapitem.h"

#include <QToolBar>
#include <src/scene.h>
#include <src/toggleaction.h>
#include <src/imagehandler.h>


class OverlayPixmapToolBar : public QToolBar
{
public:
    OverlayPixmapToolBar(Scene *scene, ImageHandler* image_handler);


private:
    OverlayPixmapItem *overlayItem = nullptr;
    Scene *scene;
    ImageHandler *imageHandler;

    ToggleAction *actionCanvasMode;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionSaveAnnotations;
    QAction *actionDelete;


private slots:
    void on_action_canvas_mode_toggled(bool toggled);
    void on_action_undo_triggered();
    void on_action_redo_triggered();
    void on_action_delete_triggered();
    void on_action_save_annotations_triggered();

};

#endif // OVERLAYPIXMAPTOOLBAR_H
