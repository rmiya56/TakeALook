#include "imageproperties.h"
#include "imagehandler.h"
#include "opencv2/opencv.hpp"
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

    cv::Mat mat_8U;
    rawMat.convertTo(mat_8U, CV_8U);

    QImage qImg;
    QImage::Format format;
    if (rawMat.channels() == 1) format = QImage::Format_Grayscale8;
    if (rawMat.channels() == 3) format = QImage::Format_RGB888;

    qImg = QImage((const unsigned char*)mat_8U.data, mat_8U.cols, mat_8U.rows, mat_8U.step, format);
    qImg.bits(); // deep copy
    qImage = qImg.rgbSwapped();
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

void ImageHandler::writeToFile(QRect rect)
{
    cv::Rect roi;
    roi.x = rect.x();
    roi.y = rect.y();
    roi.width = rect.width();
    roi.height =rect.height();

    QFileInfo fileInfo = entries[fileIndex];
    QString savePath = QString("%1/%2_mod.%3").arg(
                                fileInfo.absolutePath(), fileInfo.baseName(), fileInfo.suffix() );

    cv::Mat mat = rawMat(roi);
    cv::imwrite(savePath.toStdString(), mat);
}
