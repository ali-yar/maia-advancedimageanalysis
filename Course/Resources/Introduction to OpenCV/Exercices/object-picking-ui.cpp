// include aia and ucas utilities
#include "aiaConfig.h"
#include "ucasConfig.h"

// include my project functions
#include "functions.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace
{
	cv::Mat img;
	std::vector < std::vector <cv::Point> > contours;

	void objectPicking(int event, int x, int y, int, void* userdata)
	{
		if(event == cv::EVENT_LBUTTONDOWN)
		{
			int clicked_contour_idx = -1;
			for(int i=0; i<contours.size() && clicked_contour_idx == -1; i++)
				if(cv::pointPolygonTest(contours[i], cv::Point2f(x,y), false) >= 0)
					clicked_contour_idx = i;
			if(clicked_contour_idx != -1)
			{
				cv::Mat img_copy = img.clone();
				cv::drawContours(img_copy, contours, clicked_contour_idx, cv::Scalar(255,255,255), CV_FILLED, CV_AA);
				
				cv::Mat blended;
				cv::addWeighted(img, 0.7, img_copy, 0.3, 0, blended);
				cv::imshow("object picking", blended);
			}
			else
				cv::imshow("object picking", img);
		}
	}
}

int main() 
{
	try
	{
		img = cv::imread(EXAMPLE_IMAGES_PATH + std::string("/pills.jpg"), CV_LOAD_IMAGE_COLOR);
		if(!img.data)
			throw aia::error("Cannot load image");
		printf("channels = %d\n", img.channels());
		aia::imshow("image", img);

		cv::Mat gray_img;
		cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
		std::vector <int> histo = ucas::histogram(gray_img);
		int T = ucas::getTriangleAutoThreshold(histo);
		cv::Mat binarized;
		cv::threshold(gray_img, binarized, T, 255, cv::THRESH_BINARY);
		aia::imshow("binarized image", binarized);

		//cv::Mat SE = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7));
		//cv::morphologyEx(binarized, binarized, cv::MORPH_OPEN, SE);
		//aia::imshow("binarized + opening image", binarized);
		
		cv::medianBlur(binarized, binarized, 9);
		aia::imshow("binarized + median image", binarized);

		
		cv::findContours(binarized, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
		printf("object count = %d\n", contours.size());

		for(int i=0; i<contours.size(); i++)
		{
			cv::Mat img_copy = img.clone();
			cv::drawContours(img_copy, contours, i, cv::Scalar(255,0,0), 3, CV_AA);
			cv::Rect bbox = cv::boundingRect(contours[i]);
			cv::rectangle(img_copy, bbox, cv::Scalar(0,0,255), 3, CV_AA);
			//aia::imshow("ith contour", img_copy);
		}

		cv::namedWindow("object picking");
		cv::setMouseCallback("object picking", objectPicking);
		cv::imshow("object picking", img);
		cv::waitKey(0);

		return 1;
	}
	catch (aia::error &ex)
	{
		std::cout << "EXCEPTION thrown by " << ex.getSource() << "source :\n\t|=> " << ex.what() << std::endl;
	}
	catch (ucas::Error &ex)
	{
		std::cout << "EXCEPTION thrown by unknown source :\n\t|=> " << ex.what() << std::endl;
	}
}

