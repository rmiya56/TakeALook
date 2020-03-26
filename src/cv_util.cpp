#include "cv_util.h"


cv::Mat convTo8U(cv::Mat mat)
{
    double alpha, beta;
    double minVal, maxVal;

    switch(mat.depth())
    {
    case CV_8U:
        alpha = 1;
        beta = 0;
        break;

    case CV_8S:
        alpha = 1;
        beta = 128;
        break;

    case CV_16U:
        alpha = 255.0/655355;
        beta = 0;
        break;

    case CV_16S:
        alpha = 255.0/32767;
        beta = 128;
        break;

    case CV_32S:
        alpha = 255.0/2147483647;
        beta = 128;
        break;

    case CV_32F:
    case CV_64F:
        cv::minMaxLoc(mat, &minVal, &maxVal, NULL, NULL);
        alpha = 255.0/maxVal;
        beta = 0;
        break;

    default:
        alpha = 1;
        beta = 0;
        break;
    }

    cv::Mat mat8U;
    mat.convertTo(mat8U, 8, alpha, beta);
    return mat8U;
}

