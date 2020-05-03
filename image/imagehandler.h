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
    ImageHandler(QWidget *parent);
    void openFile();
    void saveFile(QRect roi);
    void loadFile(QFileInfo file_info);
    void loadPrev();
    void loadNext();
    QImage currentImage();
    QFileInfo currentFileInfo();
    QString currentFilePath();
    ImageProperties currentProperties();

    //void writeToFile(QRect rect);

private:

    QWidget *parent;
    QImage qImage;
    ImageProperties imgProp;
    QStringList supportFormats;
    QDir currentDir;
    QFileInfoList entries;
    int fileIndex;
    cv::Mat matRaw;
};

#endif // IMAGEHANDLER_H
