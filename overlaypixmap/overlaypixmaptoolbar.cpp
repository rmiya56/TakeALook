#include "overlaypixmaptoolbar.h"
#include <src/mainwindow.h>
#include <utility/icons.h>
#include <utility/pathutilities.h>
#include <QLineEdit>



OverlayPixmapToolBar::OverlayPixmapToolBar(Scene *scene, ImageHandler* image_handler)
    : QToolBar(),
      scene(scene),
      imageHandler(image_handler),
      penWidthEdit(new QLineEdit("10"))
{

    connect(scene, &Scene::key_delete, this, &OverlayPixmapToolBar::on_action_delete_triggered);

    actionCanvasMode = new ToggleAction(QIcon(Icons::pen), QIcon(Icons::pen_toggled), tr("Canvas"), this);
    connect(actionCanvasMode, &QAction::toggled, this, &OverlayPixmapToolBar::on_action_canvas_mode_toggled);
    addAction(actionCanvasMode);

    penWidthEdit->setAlignment(Qt::AlignRight);
    penWidthEdit->setFixedSize(QSize(30, 30));
    QFont font;
    font.setPointSize(14);
    penWidthEdit->setFont(font);
    penWidthEdit->setStyleSheet("QLineEdit {background-color: lightgray}");
    addWidget(penWidthEdit);
    connect(penWidthEdit, &QLineEdit::textChanged, this, &OverlayPixmapToolBar::on_pen_width_changed);

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

void OverlayPixmapToolBar::image_changed()
{
    overlayItem = new OverlayPixmapItem(imageHandler->currentImage().size());
    QString annotation_path = PathUtilities::replaceSuffix(imageHandler->currentFileInfo(), "json");
    overlayItem->readAnnotations(annotation_path);
}

void OverlayPixmapToolBar::on_action_canvas_mode_toggled(bool toggled)
{
     if (toggled)
    {
        actionCanvasMode->setChecked(true);
        overlayItem = new OverlayPixmapItem(imageHandler->currentImage().size());
        QString annotation_path = PathUtilities::replaceSuffix(imageHandler->currentFileInfo(), "json");
        overlayItem->readAnnotations(annotation_path);
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
    QString save_path =  PathUtilities::replaceSuffix(imageHandler->currentFileInfo(), "json");
    overlayItem->saveAnnotations(save_path);
}

void OverlayPixmapToolBar::on_pen_width_changed(const QString &width)
{
    overlayItem->setPenWidth(width.toDouble());
}
