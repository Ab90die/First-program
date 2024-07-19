#include <iostream>
#include <opencv2/opencv.hpp>

// 移动图像的函数
void moveImage(const cv::Mat &src, cv::Mat &dst, const cv::Point2f &translation) {
    cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 1, 0, translation.x, 0, 1, translation.y);
    cv::warpAffine(src, dst, translationMatrix, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
}

// 旋转图像的函数
void rotateImage(const cv::Mat &src, cv::Mat &dst, double angle) {
    cv::Point2f center(src.cols / 2.0, src.rows / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(src, dst, rot, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
}

int main() {
    std::cout << "Hello World!\n";

    // 图片文件路径
    std::string filePath = "/home/muhammad/下载/第四次考核/dataset_任务一/7.jpg";
    cv::Mat src = cv::imread(filePath, cv::IMREAD_COLOR);
    if (src.empty()) {
        std::cerr << "Error: Image could not be read." << std::endl;
        return -1;
    }

    // 定义旋转角度、平移向量
    double angle = 10.0;
    cv::Point2f translation(0, 50);

    // 缩放图像为 (800, 800) 大小
    cv::Mat scaledImage;
    cv::resize(src, scaledImage, cv::Size(800, 800));

    cv::Mat movedImage;
    moveImage(scaledImage, movedImage, translation);

    cv::Mat movedRotatedAndScaledImage;
    rotateImage(movedImage, movedRotatedAndScaledImage, angle);

    // 步骤 1：将图片中的黑色改为紫色
    for (int y = 0; y < movedRotatedAndScaledImage.rows; ++y) {
        for (int x = 0; x < movedRotatedAndScaledImage.cols; ++x) {
            cv::Vec3b &pixel = movedRotatedAndScaledImage.at<cv::Vec3b>(y, x);
            if (pixel == cv::Vec3b(0, 0, 0)) {
                pixel = cv::Vec3b(255, 0, 255);  // 紫色
            }
        }
    }

    // 扩大白色背景
    int borderSize = 620;
    cv::Mat whiteBackground = cv::Mat::zeros(movedRotatedAndScaledImage.rows + 2 * borderSize, movedRotatedAndScaledImage.cols + 2 * borderSize, CV_8UC3);
    whiteBackground.setTo(cv::Scalar(255, 255, 255));

    // 步骤 2：将背景中的黑色改为白色
    for (int y = 0; y < whiteBackground.rows; ++y) {
        for (int x = 0; x < whiteBackground.cols; ++x) {
            cv::Vec3b &pixel = whiteBackground.at<cv::Vec3b>(y, x);
            if (pixel == cv::Vec3b(0, 0, 0)) {
                pixel = cv::Vec3b(255, 255, 255);
            }
        }
    }

    // 步骤 3：将处理后的图片贴到背景上
    int startX = borderSize;
    int startY = borderSize;
    for (int y = 0; y < movedRotatedAndScaledImage.rows; ++y) {
        for (int x = 0; x < movedRotatedAndScaledImage.cols; ++x) {
            cv::Vec3b srcPixel = movedRotatedAndScaledImage.at<cv::Vec3b>(y, x);
            whiteBackground.at<cv::Vec3b>(startY + y, startX + x) = srcPixel;
        }
    }

    // 步骤 4：将紫色改回黑色
    for (int y = 0; y < whiteBackground.rows; ++y) {
        for (int x = 0; x < whiteBackground.cols; ++x) {
            cv::Vec3b &pixel = whiteBackground.at<cv::Vec3b>(y, x);
            if (pixel == cv::Vec3b(255, 0, 255)) {
                pixel = cv::Vec3b(0, 0, 0);
            }
        }
    }

    // 保存图像
    std::string savePath = "/home/muhammad/图片/考核一/考核七_rotated_and_moved.jpg";
    bool isSaved = cv::imwrite(savePath, whiteBackground);
    if (!isSaved) {
        std::cerr << "Error: Image could not be saved." << std::endl;
        return -1;
    }

    // 显示图像
    cv::imshow("Moved, Rotated and Scaled Image", whiteBackground);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
