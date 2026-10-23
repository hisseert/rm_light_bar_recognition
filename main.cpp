#include <opencv2/opencv.hpp>
using namespace cv;
#include <iostream>
#include <string>

#include "light_bar.h"

int main()
{
    const std::string videoPath = "assets/test.avi"; 
    const std::string outDir = "output/";            
  

    // 打开视频 
    cv::VideoCapture cap;
    cap.open(videoPath);
    //好像可以直接写成一行cv::VideoCapture cap()
    //因为videocapture内部就是调用了open()
    if (!cap.isOpened())
    {
        std::cerr<<"[错误] 打不开视频: "<<videoPath<<"\n"
                << "请把视频放到 assets/ 目录下，并改对 videoPath"<<std::endl;
        return 1;
        //可以用1，2，3分别编号不同的错误情况
    }

    std::cout<<"OpenCV 版本: "<<CV_VERSION<<std::endl;
    std::cout<<"视频分辨率:"
              <<cap.get(cv::CAP_PROP_FRAME_WIDTH)<<" x "
              <<cap.get(cv::CAP_PROP_FRAME_HEIGHT)<<std::endl;

    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0)
    {
        fps = 30.0; 
    }
    int delay = (fps>0) ? static_cast<int>(1000.0/fps) : 33;
    
    cv::Mat frame;
    cv::namedWindow("video", cv::WINDOW_NORMAL);
    while (cap.read(frame))
    {
        // 这里要放一下灯条咋识别的，还没写太明白
         std::vector<LightBar> bars = detectLightBars(frame);
        drawLightBars(frame, bars);


        cv::imshow("video", frame);
        cv::waitKey(delay);//保证严格的读取顺序
    }

/*这里读的是视频，在实际摄像头的应用还没想好咋处理这个delay，
不过waitkey是一定要用的，刷新窗口
ai说如果是摄像头遵循尽快处理尽快显示的原则，把delay设置成1*/

    return 0;

    // 准备输出 
    const bool saveVideo = true; 

    cv::VideoWriter writer;
    bool writerReady = false;

    int frameId = 0;

    //逐帧处理 
    while (cap.read(frame))
    {
        if (frame.empty())
        {
            break;
        }
