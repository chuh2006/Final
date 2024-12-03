#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

cv::Mat BGR2YUV_NV12(cv::Mat& bgr_mat) {
    cv::Mat yuv_nv12(bgr_mat.rows * 3 / 2, bgr_mat.cols, CV_8UC1);
    cv::Mat y_plane = yuv_nv12(cv::Rect(0, 0, bgr_mat.cols, bgr_mat.rows));
    cv::Mat uv_plane = yuv_nv12(cv::Rect(0, bgr_mat.rows, bgr_mat.cols, bgr_mat.rows / 2));
    for (int y = 0; y < bgr_mat.rows; y++) {
        for (int x = 0; x < bgr_mat.cols; x++) {
            cv::Vec3b bgr = bgr_mat.at<cv::Vec3b>(y, x);
            uint8_t B = bgr[0];
            uint8_t G = bgr[1];
            uint8_t R = bgr[2];
            uint8_t Y = (uint8_t)(0.299 * R + 0.587 * G + 0.114 * B);
            uint8_t U = (uint8_t)(-0.169 * R - 0.331 * G + 0.5 * B + 128);
            uint8_t V = (uint8_t)(0.5 * R - 0.419 * G - 0.081 * B + 128);
            y_plane.at<uint8_t>(y, x) = Y;
            if (y % 2 == 0 && x % 2 == 0) {
                uv_plane.at<uint8_t>(y / 2, x) = U;
                uv_plane.at<uint8_t>(y / 2, x + 1) = V;
            }
        }
    }
    return yuv_nv12;
}

void imgW(const char* file_name, cv::Mat& mat) {
    FILE* fd = fopen(file_name, "wb");
    size_t mat_size = mat.total() * mat.elemSize();
    fwrite(mat.ptr<uint8_t>(), 1, mat_size, fd);
    fclose(fd);
}

cv::Mat imgR(const char* file_name, int width, int height) {
    FILE* fd = fopen(file_name, "rb");
    cv::Mat mat(height, width, CV_8UC1);
    size_t mat_size = mat.total() * mat.elemSize();
    fread(mat.ptr<uint8_t>(), 1, mat_size, fd);
    fclose(fd);
    return mat;
}

int main(){
    string IMGPTH = "C:/Users/33554/Documents/learn/homew/13321/img/";
    string WPTH = "C:/Users/33554/Documents/learn/homew/13321/";
    string IMG = "pic1.jpg";
    string YUV = "test1_nv12.yuv";
    string BGR = "test1_reconstructed.jpg";
    cv::Mat bgr = cv::imread(IMGPTH + IMG, cv::IMREAD_COLOR);
    auto start = chrono::high_resolution_clock::now();
    cv::Mat yuv = BGR2YUV_NV12(bgr);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Conversion took " << elapsed.count() << " seconds." << endl;
    cout << "FPS: " << 1 / elapsed.count() << endl;
    cv::imshow("yuv", yuv);
    cv::waitKey(0);
    imgW((WPTH + YUV).c_str(), yuv);
    cv::Mat yuv2 = imgR((WPTH + YUV).c_str(), bgr.cols, bgr.rows * 3 / 2);
    cv::Mat bgr2;
    auto start2 = chrono::high_resolution_clock::now();
    cv::cvtColor(yuv2, bgr2, cv::COLOR_YUV2BGR_NV12);
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = end2 - start2;
    cout << "\nConversion took " << elapsed2.count() << " seconds." << endl;
    cout << "FPS: " << 1 / elapsed2.count() << endl;
    cv::imwrite(WPTH + BGR, bgr2);
    return 0;
}