#ifndef TRACINGPAPERITEM_H
#define TRACINGPAPERITEM_H

#include "contouritem.h"

#include <QGraphicsRectItem>
#include <QUndoCommand>


class OverlayPixmapItem : public QGraphicsPixmapItem
{

public:
    OverlayPixmapItem(QSize size);
    void undo();
    void redo();
    void readAnnotations(QString file_path);
    void saveAnnotations(QString file_path);
    void deleteSelectedContours();


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:
    void updateCanvasImage();
    void updateConnectedContours();
    void drawPolylineOnCanvas();

    QImage canvas;
    QPolygonF oneStroke;
    QVector<ContourItem*> contItems;
    QVector<QPolygonF> connectedPolygons;
    QUndoStack *undoStack;

    bool dragMoving = false;
    QPointF initPos;
    static const int brushWidth = 10;
    static constexpr qreal contourWidth = 2;
    static const QColor colorContour;
    static const QColor colorOverlay;
    static const int MIN_DISTANCE = 3;


signals:
    void call_undo();
    void call_redo();


    // QGraphicsItem interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // TRACINGPAPERITEM_H
