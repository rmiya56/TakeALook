#include "overlaypixmaptoolbar.h"
#include <src/mainwindow.h>
#include <utility/icons.h>
#include <utility/pathutilities.h>
#include <QLineEdit>
#include <QDebug>



OverlayPixmapToolBar::OverlayPixmapToolBar(Scene *scene, ImageHandler* image_handler)
    : QToolBar(),
      scene(scene),
      imageHandler(image_handler),
      penWidthEdit(new QLineEdit("10"))
{

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

    actionUndo = new QAction(QIcon(Icons::undo), tr("Undo"), this);
    addAction(actionUndo);
    actionRedo = new QAction(QIcon(Icons::redo), tr("Redo"), this);
    addAction(actionRedo);
    actionSaveAnnotations = new QAction(QIcon(Icons::save), tr("Save Annotations"), this);
    addAction(actionSaveAnnotations);
    actionDelete = new QAction(QIcon(Icons::trashbox), tr("Delete"), this);
    addAction(actionDelete);

}

void OverlayPixmapToolBar::activate()
{
    connect(scene, &Scene::key_delete, this, &OverlayPixmapToolBar::on_action_delete_triggered);
    connect(penWidthEdit, &QLineEdit::textChanged, this, &OverlayPixmapToolBar::on_pen_width_changed);
    connect(actionUndo, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_undo_triggered);
    connect(actionRedo, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_redo_triggered);
    connect(actionSaveAnnotations, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_save_annotations_triggered);
    connect(actionDelete, &QAction::triggered, this, &OverlayPixmapToolBar::on_action_delete_triggered);
}

void OverlayPixmapToolBar::deactivate()
{
    disconnect(nullptr, nullptr, this, nullptr);
}

void OverlayPixmapToolBar::image_changed()
{
    overlayItem = new OverlayPixmapItem(imageHandler->currentImage().size());
    QString annotation_path = PathUtilities::replaceSuffix(imageHandler->currentFileInfo(), "json");
    overlayItem->readAnnotations(annotation_path);
    scene->addItem(overlayItem);
}

void OverlayPixmapToolBar::on_action_canvas_mode_toggled(bool toggled)
{
     if (toggled)
    {
        activate();

        actionCanvasMode->setChecked(true);
        overlayItem = new OverlayPixmapItem(imageHandler->currentImage().size());
        QString annotation_path = PathUtilities::replaceSuffix(imageHandler->currentFileInfo(), "json");
        overlayItem->readAnnotations(annotation_path);
        scene->addItem(overlayItem);
    }
    else
    {
        deactivate();
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
