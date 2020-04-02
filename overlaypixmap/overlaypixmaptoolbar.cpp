#include "overlaypixmaptoolbar.h"
#include <src/icons.h>
#include <QLineEdit>



OverlayPixmapToolBar::OverlayPixmapToolBar(Scene *scene, ImageHandler* image_handler)
    : QToolBar()
{

    this->scene = scene;
    imageHandler = image_handler;

    actionCanvasMode = new ToggleAction(QIcon(Icons::pen), QIcon(Icons::pen_toggled), tr("Canvas"), this);
    connect(actionCanvasMode, &QAction::toggled, this, &OverlayPixmapToolBar::on_action_canvas_mode_toggled);
    addAction(actionCanvasMode);

    QLineEdit *lineEdit = new QLineEdit("1");
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setFixedSize(QSize(30, 30));
    QFont font;
    font.setPointSize(14);
    lineEdit->setFont(font);
    lineEdit->setStyleSheet("QLineEdit {background-color: lightgray}");
    addWidget(lineEdit);

    actionUndo = new QAction(QIcon(Icons::undo), tr("Undo"), this);
    connect(actionUndo, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_undo_triggered);
    addAction(actionUndo);

    actionRedo = new QAction(QIcon(Icons::redo), tr("Redo"), this);
    connect(actionRedo, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_redo_triggered);
    addAction(actionRedo);

    actionSaveAnnotations = new QAction(QIcon(Icons::save), tr("Save Annotations"), this);
    connect(actionSaveAnnotations, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_save_annotations_triggered);
    addAction(actionSaveAnnotations);

    actionDelete = new QAction(QIcon(Icons::trashbox), tr("Delete"), this);
    connect(actionDelete, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_delete_triggered);
    addAction(actionDelete);

}

void OverlayPixmapToolBar::on_action_canvas_mode_toggled(bool toggled)
{
     if (toggled)
    {
        actionCanvasMode->setChecked(true);
        overlayItem = new OverlayPixmapItem(imageHandler->currentImage().size());
        scene->addItem(overlayItem);
    }
    else
    {
        actionCanvasMode->setChecked(false);
        scene->removeItem(overlayItem);
     }
}

void OverlayPixmapToolBar::on_action_undo_triggered()
{
    overlayItem->undo();
}

void OverlayPixmapToolBar::on_action_redo_triggered()
{
    overlayItem->redo();
}

void OverlayPixmapToolBar::on_action_delete_triggered()
{
    overlayItem->deleteSelectedContours();
}

void OverlayPixmapToolBar::on_action_save_annotations_triggered()
{
    QFileInfo file_info = imageHandler->currentFileInfo();
    QString save_path = file_info.absolutePath() + QDir::separator() + file_info.baseName() + ".json";
    overlayItem->saveAnnotations(save_path);
}
