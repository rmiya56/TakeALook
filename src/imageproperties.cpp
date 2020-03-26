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

    qDebug()	<< ""
                << "format:" << name
                << "type:" <<  type
                << "channels:" << channels
                << QString("size:%1x%2").arg(QString::number(width), QString::number(height));
}

QString ImageProperties::getType(cv::Mat mat)
{
    std::string r;
    uchar depth = mat.type() & CV_MAT_DEPTH_MASK;
    switch(depth) {
        case CV_8U:		return "8U";
        case CV_8S:  	return "8S";
        case CV_16U: 	return "16U";
        case CV_16S: 	return "16S";
        case CV_32S: 	return "32S";
        case CV_32F:	return "32F";
        case CV_64F:	return "64F";
        default: 		return "Unknown";
    }
}
