#include <iostream>
#include <opencv2/opencv.hpp>

int rotateImage(const cv::Mat &src, cv::Mat &dst, const double angle, const int mode)
{
    if (src.empty())
    {
        std::cout << "Damn, the input image is empty!\n";
        return -1;
    }

    if (mode == 0)
    {
        cv::Point2f center(0, 0);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::warpAffine(src, dst, rot, src.size(), cv::INTER_LINEAR);  // 使用 cv::INTER_LINEAR 插值法
    }
    else
    {
        cv::Point2f center(0, 0);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);

        double cos = std::abs(rot.at<double>(0, 0));
        double sin = std::abs(rot.at<double>(0, 1));
        int newWidth = static_cast<int>(src.cols * cos + src.rows * sin);
        int newHeight = static_cast<int>(src.cols * sin + src.rows * cos);

        cv::warpAffine(src, dst, rot, cv::Size(newWidth, newHeight), cv::INTER_LINEAR);  // 使用 cv::INTER_LINEAR 插值法
    }

    return 0;
}

int main() {
    std::cout << "Hello World!\n";

    std::string filePath = "/home/muhammad/下载/第四次考核/dataset_任务一/19.jpg";
    cv::Mat src = cv::imread(filePath, cv::IMREAD_COLOR);
    if (src.empty()) {
        std::cerr << "Error: Image could not be read." << std::endl;
        return -1;
    }

    // 定义图像缩放比例
    double scale = 1;

    // 缩放图像（使用 LINEAR 插值法）
    cv::Mat scaledImage;
    cv::resize(src, scaledImage, cv::Size(800, 800), scale, scale, cv::INTER_LINEAR);  // 使用 cv::INTER_LINEAR 插值法

    // 平移图像实现向左上移动
    cv::Mat translatedImage;
    cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 1, 0, 0, 0, 1, 50);
    cv::warpAffine(scaledImage, translatedImage, translationMatrix, scaledImage.size());

    // 旋转图像
    cv::Mat rotatedImage;
    double angle = 10.0;
    rotateImage(translatedImage, rotatedImage, angle, 1);

    // 保存图像
    std::string savePath = "/home/muhammad/图片/考核十九.jpg";
    bool isSaved = cv::imwrite(savePath, rotatedImage);
    if (!isSaved) {
        std::cerr << "Error: Image could not be saved." << std::endl;
        return -1;
    }

    // 显示图像
    cv::imshow("Rotated Image", rotatedImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
