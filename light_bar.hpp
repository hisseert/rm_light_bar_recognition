#pragma once

#include <opencv2/opencv.hpp>

#include <vector>

struct LightBar
{
    cv::RotatedRect rect;     
    cv::Point2f corners[4];   
    cv::Point2f top;          
    cv::Point2f bottom;       
};

std::vector<LightBar> detectLightBars(const cv::Mat& frame);

void drawLightBars(cv::Mat& frame, const std::vector<LightBar>& bars);
