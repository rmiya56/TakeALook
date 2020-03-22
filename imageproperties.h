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
    int digits_x = 3;
    int digits_y = 3;

private:
    QString getType(cv::Mat mat);
    //int depth(cv::Mat mat);

};

#endif // IMAGEFORMAT_H
