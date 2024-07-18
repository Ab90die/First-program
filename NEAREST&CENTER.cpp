#include <iostream>
#include <opencv2/opencv.hpp>

int rotateImage(const cv::Mat &src, cv::Mat &dst, const double angle, const int mode)
{
    // mode = 0,Keep the original image size unchanged
    // mode = 1, Change the original image size to fit the rotated scale, padding with zero

    if (src.empty())
    {
        std::cout << "Damn, the input image is empty!\n";
        return -1;
    }

    if (mode == 0)
    {
        cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::warpAffine(src, dst, rot, src.size()); // the original size
    }
    else
    {

        double alpha = -angle * CV_PI / 180.0; // convert angle to radian format

        cv::Point2f srcP[3];
        cv::Point2f dstP[3];
        srcP[0] = cv::Point2f(0, src.rows);
        srcP[1] = cv::Point2f(src.cols, 0);
        srcP[2] = cv::Point2f(src.cols, src.rows);

        // rotate the pixels
        for (int i = 0; i < 3; i++)
            dstP[i] = cv::Point2f(srcP[i].x * cos(alpha) - srcP[i].y * sin(alpha), srcP[i].y * cos(alpha) + srcP[i].x * sin(alpha));
        double minx, miny, maxx, maxy;
        minx = std::min(std::min(std::min(dstP[0].x, dstP[1].x), dstP[2].x), float(0.0));
        miny = std::min(std::min(std::min(dstP[0].y, dstP[1].y), dstP[2].y), float(0.0));
        maxx = std::max(std::max(std::max(dstP[0].x, dstP[1].x), dstP[2].x), float(0.0));
        maxy = std::max(std::max(std::max(dstP[0].y, dstP[1].y), dstP[2].y), float(0.0));

        int w = maxx - minx;
        int h = maxy - miny;

        // translation
        for (int i = 0; i < 3; i++)
        {
            if (minx < 0)
                dstP[i].x -= minx;
            if (miny < 0)
                dstP[i].y -= miny;
        }

        cv::Mat warpMat = cv::getAffineTransform(srcP, dstP);
        cv::warpAffine(src, dst, warpMat, cv::Size(w, h)); // extend size

    } // end else

    return 0;
}

int main()
{
    std::cout << "Hello World!\n";

    std::string filePath = "/home/muhammad/下载/第四次考核/dataset_任务一/5.jpg";
    cv::Mat src = cv::imread(filePath, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cerr << "Error: Image could not be read." << std::endl;
        return -1;
    }

    // 定义图像缩放比例
    double scale = 0.8;

    // 缩放图像（使用 NEAREST 插值法）
    cv::Mat scaledImage;
    cv::resize(src, scaledImage, cv::Size(), scale, scale, cv::INTER_NEAREST);

    // 平移图像实现向左上移动
    cv::Mat translatedImage;
    cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 1, 0, 30, 0, 1, 60);
    cv::warpAffine(scaledImage, translatedImage, translationMatrix, scaledImage.size());

    // 旋转图像
    cv::Mat rotatedImage;
    double angle = 90.0;
    rotateImage(translatedImage, rotatedImage, angle, 1);

    // 保存图像
    std::string savePath = "/home/muhammad/图片/考核 一.jpg";
    bool isSaved = cv::imwrite(savePath, rotatedImage);
    if (!isSaved)
    {
        std::cerr << "Error: Image could not be saved." << std::endl;
        return -1;
    }

    // 显示图像
    cv::imshow("Rotated Image", rotatedImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
