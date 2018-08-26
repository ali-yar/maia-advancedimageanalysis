void hitormiss(const cv::Mat& src, cv::Mat& dst, cv::Mat element) {
		cv::Mat B1, B2;
		cv::bitwise_and(element+1, 2*cv::Mat::ones(element.size(), CV_8S), B1);
		cv::bitwise_and(-1 * element + 1, 2 * cv::Mat::ones(element.size(), CV_8S), B2);
		
		B1.convertTo(B1, CV_8U);
		B2.convertTo(B2, CV_8U);

		cv::Mat erode1, erode2;
		cv::erode(src, erode1, B1);
		cv::erode(255 - src, erode2, B2);
		
		cv::bitwise_and(erode1, erode1, dst);

		//cv::normalize(B1, B1, 0, 255, CV_MINMAX);
		//cv::normalize(B2, B2, 0, 255, CV_MINMAX);
		//aia::imshow("B1", B1);
		//aia::imshow("B2", B2);
	}