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

    loadContours();
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
    connectedPolygons = extractContours(canvas);

    QPixmap overlay = this->pixmap();
    overlay.fill(colorOverlay);
    QPainter painter(&overlay);
    painter.setPen(QPen(colorContour, contourWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);
    for(auto const& p : connectedPolygons) painter.drawPolygon(p);
    this->setPixmap(overlay);
}

void OverlayPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        initPos = event->pos();
        oneStroke.clear();
        oneStroke << event->scenePos();
        dragMoving = true;
    }
    if(event->button() == Qt::RightButton)
    {
        undoStack->undo();
        updateCanvasImage();
        updateConnectedContours();
    }
    //QGraphicsPixmapItem::mousePressEvent(event);
}

void OverlayPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && dragMoving)
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
    if(event->button() == Qt::LeftButton && dragMoving)
    {
        dragMoving = false;
        QPointF d = event->pos() - initPos;
        qreal distance = qSqrt(QPointF::dotProduct(d,d));
        if (distance < MIN_DISTANCE) return;

        drawPolylineOnCanvas();
        ContourItem *cont_item = new ContourItem(extractContours(canvas)[0], this);
        undoStack->push( new AddPolygonCommand(cont_item, &contItems));

        updateCanvasImage();
        updateConnectedContours();
    }
    JsonFile::save(connectedPolygons);
    //QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void OverlayPixmapItem::drawPolylineOnCanvas()
{
    canvas.fill(255);
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, brushWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPolyline(oneStroke);
}

void OverlayPixmapItem::loadContours()
{
    JsonFile json_file;
    QVector<QPolygonF> polygons = json_file.read();

    for (auto poly : polygons)
    {
        ContourItem *cont_item = new ContourItem(poly, this);
        contItems.push_back(cont_item);
    }
    updateCanvasImage();
    updateConnectedContours();
}
