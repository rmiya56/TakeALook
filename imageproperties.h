#ifndef IMAGEFORMAT_H
#define IMAGEFORMAT_H

#include <QString>
#include "opencv2/opencv.hpp"


class ImageProperties
{
public:
    ImageProperties();
    ImageProperties(QString suffix, cv::Mat mat);
    QString name;
    QString type;
    int channels;
    int width;
    int height;
    int digitsX = 3;
    int digitsY = 3;

private:
    QString getType(cv::Mat mat);
    //int depth(cv::Mat mat);

};

#endif // IMAGEFORMAT_H
