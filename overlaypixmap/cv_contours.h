#ifndef CV_CONTOURS_H
#define CV_CONTOURS_H

#include <QImage>
#include <QPolygonF>
#include <QVector>



QVector<QPolygonF> extractContours(QImage gray_image);

#endif // CV_CONTOURS_H
