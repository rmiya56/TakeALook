#include "imageproperties.h"
#include <QtMath>
#include <QDebug>



ImageProperties::ImageProperties()
{
}

ImageProperties::ImageProperties(QString suffix, cv::Mat mat)
{
    name = suffix;
    channels = mat.channels();
    width = mat.cols;
    height = mat.rows;
    digitsX = qCeil(log10(width));
    digitsY = qCeil(log10(height));

    depth = mat.depth();
    switch(depth)
    {
    case CV_8U: // 0 ~ 255
        alpha = 1;
        beta = 0;
        digitsD = 3;
        break;

    case CV_8S: // -128 ~ +127
        alpha = 1;
        beta = 128;
        digitsD = 4;
        break;

    case CV_16U: // 0 ~ 65535
        alpha = 255.0/65535;
        beta = 0;
        digitsD = 5;
        break;

    case CV_16S: // -32768 ~ 32767
        alpha = 255.0/32767;
        beta = 128;
        digitsD = 6;
        break;

    case CV_32S: // -2147483648 ~ +2147483647
        alpha = 255.0/2147483647;
        beta = 128;
        digitsD = 11;
        break;

    case CV_32F:
    case CV_64F:
        double minVal, maxVal;
        cv::minMaxLoc(mat, &minVal, &maxVal, NULL, NULL);
        alpha = 255.0/maxVal;
        beta = 0;
        digitsD = 5;
        break;

    default:
        alpha = 1;
        beta = 0;
        break;
    }
}

QString ImageProperties::formatPixelLocation(QPointF pos)
{
    QPoint pix(qFloor(pos.x()), qFloor(pos.y()));
    QString x = QString::number(pix.x()).rightJustified(digitsX, ' ');
    QString y = QString::number(pix.y()).rightJustified(digitsY, ' ');
    return QString("[%1, %2]").arg(x, y);
}

QStringList ImageProperties::formatPixelColor(QImage &q_image, QPointF pos, QPoint offset)
{
    QPoint pix = QPoint(qFloor(pos.x()), qFloor(pos.y())) - offset;
    QStringList color;

    if (q_image.valid(pix))
    {
        QColor c = q_image.pixel(pix);
        QString r = QString::number((c.red()-beta)/alpha).rightJustified(digitsD, ' ');
        QString g = QString::number((c.green()-beta)/alpha).rightJustified(digitsD, ' ');
        QString b = QString::number((c.blue()-beta)/alpha).rightJustified(digitsD, ' ');

        if (channels == 3)
            color << r << g << b;
        else if (channels == 1)
            color << r;
    }
    return color;
}
