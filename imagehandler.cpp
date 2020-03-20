#include "imageproperties.h"
#include "imagehandler.h"
#include "opencv2/opencv.hpp"
#include <QDebug>


ImageHandler::ImageHandler()
{
    support_formats << "*.png";
    support_formats << "*.jpg";
    support_formats << "*.tif";
    support_formats << "*.tiff";
    support_formats << "*.bmp";
}

void ImageHandler::loadImage(QFileInfo file_info)
{
    current_dir = file_info.absoluteDir();
    entries = current_dir.entryInfoList(support_formats, QDir::Files);
    file_index = entries.indexOf(file_info);
    QString filepath = entries[file_index].absoluteFilePath();
    raw_mat = cv::imread(filepath.toStdString(), cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

    qDebug() << raw_mat.step;
    _properties = ImageProperties(file_info.suffix(), raw_mat);

    cv::Mat mat_8U;
    raw_mat.convertTo(mat_8U, CV_8U);

    QImage qImg;
    QImage::Format format;
    if (raw_mat.channels() == 1) format = QImage::Format_Grayscale8;
    if (raw_mat.channels() == 3) format = QImage::Format_RGB888;

    qImg = QImage((const unsigned char*)mat_8U.data, mat_8U.cols, mat_8U.rows, mat_8U.step, format);
    qImg.bits(); // deep copy
    _image = qImg.rgbSwapped();
}

void ImageHandler::loadPrev()
{
    if(file_index > 0)
        loadImage(entries[--file_index]);
}

void ImageHandler::loadNext()
{
    if(file_index < entries.length()-1)
        loadImage(entries[++file_index]);
}

QImage ImageHandler::currentImage()
{
    return _image;
}

QString ImageHandler::currentFilePath()
{
    return entries[file_index].absoluteFilePath();
}

ImageProperties ImageHandler::currentProperties()
{
   return _properties;
}

void ImageHandler::writeToFile(QRect rect)
{
    cv::Rect roi;
    roi.x = rect.x();
    roi.y = rect.y();
    roi.width = rect.width();
    roi.height =rect.height();

    QFileInfo fileInfo = entries[file_index];
    QString savePath = QString("%1/%2_mod.%3").arg(
                                fileInfo.absolutePath(), fileInfo.baseName(), fileInfo.suffix() );

    cv::Mat mat = raw_mat(roi);
    cv::imwrite(savePath.toStdString(), mat);
}
