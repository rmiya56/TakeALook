#include "cv_contours.h"
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <vector>



std::vector<std::vector<cv::Point>> approxContours(std::vector<std::vector<cv::Point>> contours)
{
    double epsilon;
    std::vector<cv::Point> approxed;

    for (unsigned long i=0; i<contours.size(); i++)
    {
        epsilon = 0.005*cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approxed, epsilon, true);
        contours[i] = approxed;
    }
    return contours;
}

QVector<QPolygonF> extractContours(QImage gray_image)
{
    cv::Mat mat(gray_image.height(), gray_image.width(), CV_8U, gray_image.scanLine(0));
    cv::Mat inv_mat;
    cv::threshold(mat, inv_mat, 125, 255, cv::THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(inv_mat, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    //contours = approxContours(contours);

    QVector<QPolygonF> polygons;
    for(auto const& cont : contours) {
        QPolygonF poly;
        for (auto const& p : cont) { poly << QPoint(p.x, p.y); }
        polygons.push_back(poly);
    }
    return polygons;
}
