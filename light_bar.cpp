#include "light_bar.h"

#include <algorithm>

namespace
{
    const int kBinaryThresh = 100;
    const double kMinArea = 20.0;

    const float kMinAspectRatio = 2.0f;

    std::vector<cv::Point2f> orderPoints(const cv::Point2f pts[4])
    {
        std::vector<cv::Point2f> v(pts, pts + 4);

        std::sort(v.begin(), v.end(),
                  [](const cv::Point2f& a, const cv::Point2f& b)
                  { return a.y < b.y; });

        std::vector<cv::Point2f> result(4);

        if (v[0].x < v[1].x)
        {
            result[0] = v[0];
            result[1] = v[1];
        }
        else
        {
            result[0] = v[1];
            result[1] = v[0];
        }

        if (v[2].x < v[3].x)
        {
            result[3] = v[2];
            result[2] = v[3];
        }
        else
        {
            result[3] = v[3];
            result[2] = v[2];
        }

        return result;
    }
} 

std::vector<LightBar> detectLightBars(const cv::Mat& frame)
{
    std::vector<LightBar> bars;

    if (frame.empty())
    {
        return bars;
    }

    // 1. 分离 BGR 三个通道 
    std::vector<cv::Mat> bgr;
    cv::split(frame, bgr);

    cv::Mat mask;
    cv::subtract(bgr[2], bgr[0], mask);

    // 2. 二值化 
    cv::threshold(mask, mask, kBinaryThresh, 255, cv::THRESH_BINARY);
