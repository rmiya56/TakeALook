#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QStringList>
#include "opencv2/opencv.hpp"
#include "imageproperties.h"

class ImageHandler
{
public:
    ImageHandler();
    void loadImage(QFileInfo file_info);
    void loadPrev();
    void loadNext();
    QImage currentImage();
    QString currentFilePath();
    ImageProperties currentProperties();

    void writeToFile(QRect rect);

private:
    QImage _image;
    ImageProperties _properties;
    QStringList support_formats;
    QDir current_dir;
    QFileInfoList entries;
    int file_index;
    cv::Mat raw_mat;
};

#endif // IMAGEHANDLER_H
