#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

std::string winname = "output";

void onChange(int k, void* data) {
	cv::Mat im = *(reinterpret_cast<cv::Mat *>(data));
	
	cv::Mat smoothed, sharpened;
	sharpened + im.clone();
	cv::GaussianBlur(im, smoothed, cv::Size(7, 7), 0);

	sharpened = im + k/100 * (im - smoothed);

	cv::imshow(winname, sharpened);
}

int main() {
	std::string file = "C:/Users/hp4540/Desktop/opencv/homework1/lena.jpg";
	cv::Mat image = cv::imread(file, CV_LOAD_IMAGE_UNCHANGED);
	
	cv::namedWindow(winname);

	int strengthInit = 0;
	int strengthMax = 200;

	cv::createTrackbar("stength", winname, &strengthInit, strengthMax, onChange, &image);

	// start showing something
	onChange(10, &image);

	cv::waitKey(0);

	return 0;
}
