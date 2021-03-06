#ifndef IMAGEFORMAT_H
#define IMAGEFORMAT_H

#include <QImage>
#include <QPointF>
#include <QString>
#include "opencv2/opencv.hpp"


class ImageProperties
{
public:
    ImageProperties();
    ImageProperties(QString suffix, cv::Mat mat);
    QString formatPixelLocation(QPointF pos);
    QStringList formatPixelColor(QImage &q_image, QPointF pos, QPoint offset);

    QString name;
    QString type;
    int channels;
    int width;
    int height;
    int digitsX = 3;
    int digitsY = 3;
    int depth;
    int digitsD = 3;
    double alpha;
    double beta;

};

#endif // IMAGEFORMAT_H
