#include "overlaypixmapitem.h"
#include "cv_contours.h"
#include "jsonfile.h"
#include "addpolygoncommand.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QDebug>


const QColor OverlayPixmapItem::colorContour = QColor(0, 255, 0, 100);
const QColor OverlayPixmapItem::colorOverlay = QColor(255, 255, 255, 50);


OverlayPixmapItem::OverlayPixmapItem(QSize size) :
    QGraphicsPixmapItem(),
    undoStack(new QUndoStack)
{

    QImage image(size, QImage::Format_RGBA8888);
    image.fill(colorOverlay);
    QPixmap overlay = QPixmap::fromImage(image);
    this->setPixmap(overlay);

    canvas = QImage(size, QImage::Format_Grayscale8);
    canvas.fill(255);
}

void OverlayPixmapItem::updateCanvasImage()
{
    canvas.fill(255);
    QPainter painter(&canvas);
    painter.setPen(Qt::black);
    for (auto item : contItems) painter.drawPolygon(item->polygon());
}

void OverlayPixmapItem::updateConnectedContours()
{

    updateCanvasImage();

    connectedPolygons = extractContours(canvas);

    QPixmap overlay = this->pixmap();
    overlay.fill(colorOverlay);
    QPainter painter(&overlay);
    QPen pen =  QPen(colorContour, contourWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    for (auto const& p : connectedPolygons) painter.drawPolygon(p);
    this->setPixmap(overlay);
}

void OverlayPixmapItem::undo()
{
    undoStack->undo();
    updateConnectedContours();
}

void OverlayPixmapItem::redo()
{
    undoStack->redo();
    updateConnectedContours();
}

void OverlayPixmapItem::saveAnnotations(QString file_path)
{
    JsonFile::savePolygons(connectedPolygons, file_path);
}

void OverlayPixmapItem::deleteSelectedContours()
{
    for (auto const& cont_item : contItems )
    {
        if (cont_item->isSelected())
        {
            cont_item->setSelected(false);
            contItems.removeOne(cont_item);
        }
    }
    updateConnectedContours();
}

void OverlayPixmapItem::readAnnotations(QString file_path)
{
    JsonFile json_file;
    QVector<QPolygonF> polygons = JsonFile::readPolygons(file_path);

    for (auto poly : polygons)
    {
        ContourItem *cont_item = new ContourItem(poly, this);
        contItems.push_back(cont_item);
    }
    updateConnectedContours();
}

void OverlayPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        initPos = event->pos();
        oneStroke.clear();
        oneStroke << event->scenePos();
        dragMoving = true;
    }
    else if(event->button() == Qt::LeftButton)
    {
        for (auto cont_item : contItems)
        {
            if(!cont_item->contains(event->scenePos()))
                cont_item->setSelected(false);
        }
    }
    //QGraphicsPixmapItem::mousePressEvent(event);
}

void OverlayPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton && dragMoving)
    {
        oneStroke << event->scenePos();
        QPixmap pixmap = this->pixmap();
        QPainter painter(&pixmap);
        painter.setPen(QPen(colorContour, brushWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPolyline(oneStroke);
        this->setPixmap(pixmap);
    }
    //QGraphicsPixmapItem::mouseMoveEvent(event);
}

void OverlayPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton && dragMoving)
    {
        dragMoving = false;
        QPointF d = event->pos() - initPos;
        qreal distance = qSqrt(QPointF::dotProduct(d,d));
        if (distance < MIN_DISTANCE) return;

        drawPolylineOnCanvas();

        canvas.save("canvas.png");

        ContourItem *cont_item = new ContourItem(extractContours(canvas)[0], this);
        undoStack->push( new AddPolygonCommand(cont_item, &contItems));

        qDebug() << cont_item->polygon();

        updateConnectedContours();
    }
    //QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void OverlayPixmapItem::drawPolylineOnCanvas()
{
    canvas.fill(255);
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, brushWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPolyline(oneStroke);
}



void OverlayPixmapItem::keyPressEvent(QKeyEvent *event)
{

}
