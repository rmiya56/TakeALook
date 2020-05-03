#include "imageproperties.h"
#include "imagehandler.h"
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <QFileDialog>
#include <QSettings>


ImageHandler::ImageHandler(QWidget *parent)
{
    this->parent = parent;
    supportFormats << "*.png";
    supportFormats << "*.jpg";
    supportFormats << "*.tif";
    supportFormats << "*.tiff";
    supportFormats << "*.bmp";
}

void ImageHandler::openFile()
{
    QString dir, filter;

    QSettings read("settings.ini", QSettings::IniFormat, parent);
    read.beginGroup("ImageFiles");
    if(read.contains("dir")) dir = read.value("dir", "").toString();
    if(read.contains("open_filter"))  filter = read.value("filter", QObject::tr("All(*.*)")).toString();
    QString fileName = QFileDialog::getOpenFileName(parent, QObject::tr("Open File"),
                                                   dir,
                                                   QObject::tr("All(*.*);;PNG(*.png);;JPG(*.jpg);;BMP(*.bmp);;TIFF(*.tif*)"),
                                                   &filter,
                                                   QFileDialog::DontUseCustomDirectoryIcons);
    if (fileName.isEmpty()) return;

    QFileInfo file_info(fileName);
    loadFile(file_info);

    QSettings settings("settings.ini", QSettings::IniFormat, parent);
    settings.beginGroup("ImageFiles");
    settings.setValue("dir", file_info.absoluteDir().absolutePath());
    settings.setValue("filter", filter);
    settings.endGroup();

}

void ImageHandler::saveFile(QRect rect)
{
    QString dir, filter;

    QSettings read("settings.ini", QSettings::IniFormat, parent);
    read.beginGroup("ImageFiles");
    if(read.contains("save_filter"))  filter = read.value("filter", QObject::tr("All(*.*)")).toString();

    QString fileName = QFileDialog::getSaveFileName(parent,
                                                    QObject::tr("Save File"),
                                                    currentFilePath(),
                                                    QObject::tr("All(*.*);;PNG(*.png);;JPG(*.jpg);;BMP(*.bmp);;TIFF(*.tif*)"),
                                                    &filter,
                                                    QFileDialog::DontUseCustomDirectoryIcons);

    if (fileName.isEmpty()) return;

    QFileInfo file_info(fileName);

    cv::Rect roi;
    roi.x = rect.x();
    roi.y = rect.y();
    roi.width = rect.width();
    roi.height =rect.height();

    QString savePath = QString("%1/%2.%3").arg( file_info.absolutePath(), file_info.baseName(), file_info.suffix() );
    cv::imwrite(savePath.toStdString(), matRaw(roi));

    QSettings settings("settings.ini", QSettings::IniFormat, parent);
    settings.beginGroup("ImageFiles");
    settings.setValue("filter", filter);
    settings.endGroup();
}

void ImageHandler::loadPrev()
{
    if(fileIndex > 0)
        loadFile(entries[--fileIndex]);
}

void ImageHandler::loadNext()
{
    if(fileIndex < entries.length()-1)
        loadFile(entries[++fileIndex]);
}

QImage ImageHandler::currentImage()
{
    return qImage;
}

QFileInfo ImageHandler::currentFileInfo()
{
    return entries[fileIndex];
}

QString ImageHandler::currentFilePath()
{
    return entries[fileIndex].absoluteFilePath();
}

ImageProperties ImageHandler::currentProperties()
{
   return imgProp;
}

void ImageHandler::loadFile(QFileInfo file_info)
{
    currentDir = file_info.absoluteDir();
    entries = currentDir.entryInfoList(supportFormats, QDir::Files);
    fileIndex = entries.indexOf(file_info);
    QString filepath = entries[fileIndex].absoluteFilePath();
    matRaw = cv::imread(filepath.toStdString(), cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
    imgProp = ImageProperties(file_info.suffix(), matRaw);

    cv::Mat mat8U;
    matRaw.convertTo(mat8U, 8, imgProp.alpha, imgProp.beta);

    QImage qImg;
    QImage::Format format;
    if (imgProp.channels == 1) format = QImage::Format_Grayscale8;
    if (imgProp.channels == 3) format = QImage::Format_RGB888;

    qImg = QImage((const unsigned char*)mat8U.data, mat8U.cols, mat8U.rows, mat8U.step, format);
    qImg.bits(); // deep copy
    qImage = qImg.rgbSwapped();
}
