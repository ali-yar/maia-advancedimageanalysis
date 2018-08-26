int main() 
{
	try
	{	
		std::string dataset_path = "V:/EIID/projects/AIA-Pectoral-Muscle-Segmentation/dataset/";
		std::vector <cv::Mat> images = aia::getImagesInFolder(dataset_path + "images", ".png");
		std::vector <cv::Mat> truths = aia::getImagesInFolder(dataset_path + "groundtruth", ".png", true);
		std::vector <cv::Mat> masks  = aia::getImagesInFolder(dataset_path + "mask", ".png", true);

		// dummy pectoral muscle segmentation: fixed triangle
		for(auto & im : images)
		{
			im.convertTo(im, CV_8U);
			im.setTo(cv::Scalar(0));
			std::vector <cv::Point> triangle;
			triangle.push_back(cv::Point(0, 0));
			triangle.push_back(cv::Point(900,0));
			triangle.push_back(cv::Point(0, 1500));
			cv::fillConvexPoly(im, triangle, cv::Scalar(255));
		}

		std::vector <cv::Mat> visual_results;
		double ACC = aia::accuracy(images, truths, masks);
		printf("Accuracy (dummy segmentation) = %.2f%%\n", ACC*100);
		//for(auto & v : visual_results)
			//aia::imshow("Visual result", v);


		// ideal segmentation: we use the groundtruth 
		ACC = aia::accuracy(truths, truths, masks);
		printf("Accuracy (ideal segmentation) = %.2f%%\n", ACC*100);

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