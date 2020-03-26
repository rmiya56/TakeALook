#include "imageproperties.h"
#include "imagehandler.h"
#include "opencv2/opencv.hpp"
#include "cv_util.h"
#include <QDebug>


ImageHandler::ImageHandler()
{
    supportFormats << "*.png";
    supportFormats << "*.jpg";
    supportFormats << "*.tif";
    supportFormats << "*.tiff";
    supportFormats << "*.bmp";
}

void ImageHandler::loadImage(QFileInfo file_info)
{
    currentDir = file_info.absoluteDir();
    entries = currentDir.entryInfoList(supportFormats, QDir::Files);
    fileIndex = entries.indexOf(file_info);
    QString filepath = entries[fileIndex].absoluteFilePath();
    rawMat = cv::imread(filepath.toStdString(), cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

    qDebug() << rawMat.step;
    properties = ImageProperties(file_info.suffix(), rawMat);

    cv::Mat mat8U = convTo8U(rawMat);

    QImage qImg;
    QImage::Format format;
    if (rawMat.channels() == 1) format = QImage::Format_Grayscale8;
    if (rawMat.channels() == 3) format = QImage::Format_RGB888;

    qImg = QImage((const unsigned char*)mat8U.data, mat8U.cols, mat8U.rows, mat8U.step, format);
    qImg.bits(); // deep copy
    qImage = qImg.rgbSwapped();
}

void ImageHandler::writeToFile(QFileInfo file_info, QRect rect)
{
    cv::Rect roi;
    roi.x = rect.x();
    roi.y = rect.y();
    roi.width = rect.width();
    roi.height =rect.height();

    QString savePath = QString("%1/%2.%3").arg( file_info.absolutePath(), file_info.baseName(), file_info.suffix() );
    cv::imwrite(savePath.toStdString(), rawMat(roi));
}

void ImageHandler::loadPrev()
{
    if(fileIndex > 0)
        loadImage(entries[--fileIndex]);
}

void ImageHandler::loadNext()
{
    if(fileIndex < entries.length()-1)
        loadImage(entries[++fileIndex]);
}

QImage ImageHandler::currentImage()
{
    return qImage;
}

QString ImageHandler::currentFilePath()
{
    return entries[fileIndex].absoluteFilePath();
}

ImageProperties ImageHandler::currentProperties()
{
   return properties;
}
