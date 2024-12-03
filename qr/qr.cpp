#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat rec(cv::Mat frame){
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);
    cv::Mat thre;
    cv::threshold(blur, thre, 100, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    cv::Mat canny;
    cv::Canny(thre, canny, 75, 200);
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::findContours(canny, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    vector<vector<cv::Point>> conPoly(contours.size());
    vector<cv::Rect> boundRect(contours.size());
    for(int i = 0; i < contours.size(); i++){
        int area = cv::contourArea(contours[i]);
        if(area > 2500){
            float peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            boundRect[i] = cv::boundingRect(conPoly[i]);
            cv::rectangle(frame, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 2);
        }
    }
    return frame;
}

cv::Mat qr(cv::Mat frame){
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Mat thre;
    cv::threshold(gray, thre, 100, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    cv::QRCodeDetector qrDecoder;
    string data = qrDecoder.detectAndDecode(thre);
    if(data.length() > 0){
        vector<cv::Point2f> pts;
        qrDecoder.detect(thre, pts);
        for(int i = 0; i < 4; i++){
            cv::line(frame, pts[i], pts[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
        }
        return frame;
    }
    return rec(frame);
}

int main(){
    string path = "C:/Users/33554/Documents/learn/homew/qr/video/";
    string file = "20241026_225634.mp4";
    cv::Mat frame;
    cv::VideoCapture cap(path + file);
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    while(1){
        cap >> frame;
        if(frame.empty()){
            break;
        }
        frame = qr(frame);
        cv::imshow("Frame", frame);
        if(cv::waitKey(10) >= 0){
            break;
        }
    }
}