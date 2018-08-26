#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat gradientImage(const cv::Mat & img) {
	cv::Mat imgX, imgY, imgG;

	cv::Sobel(img, imgX, CV_16S, 1, 0);
	cv::Sobel(img, imgY, CV_16S, 0, 1);

	cv::convertScaleAbs(imgX, imgX);
	cv::convertScaleAbs(imgY, imgY);

	cv::imshow("Sobel X", imgX);
	cv::waitKey(0);
	cv::imshow("Sobel Y", imgY);
	cv::waitKey(0);

	cv::addWeighted(imgX, 0.5, imgY, 0.5, 0, imgG);

	return imgG;
}

int main() {
	std::string file = "C:/Users/hp4540/Desktop/opencv/homework1/lena.jpg";
	cv::Mat image = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);

	cv::imshow("ori", image);
	cv::waitKey(0);

	cv::Mat gradient = gradientImage(image);

	cv::imshow("gradient", gradient);
	cv::waitKey(0);

	return 0;
}
