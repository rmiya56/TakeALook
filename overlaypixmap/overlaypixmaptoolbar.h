#ifndef OVERLAYPIXMAPTOOLBAR_H
#define OVERLAYPIXMAPTOOLBAR_H

#include "overlaypixmapitem.h"
#include <src/scene.h>
#include <src/toggleaction.h>
#include <src/imagehandler.h>
#include <QLineEdit>
#include <QToolBar>


class OverlayPixmapToolBar : public QToolBar
{
public:
    OverlayPixmapToolBar(Scene *scene, ImageHandler* image_handler);

public slots:
    void image_changed();


private:

    void activate();
    void deactivate();


    Scene *scene;
    OverlayPixmapItem *overlayItem = nullptr;
    ImageHandler *imageHandler;
    QLineEdit *penWidthEdit;

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
    void on_pen_width_changed(const QString &width);

};

#endif // OVERLAYPIXMAPTOOLBAR_H
