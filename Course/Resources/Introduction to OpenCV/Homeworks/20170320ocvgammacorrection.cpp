// include aia and ucas utility functions
#include "aia/aiaConfig.h"
#include "ucas/ucasConfig.h"

// include my project functions
#include "functions.h"

#include <opencv2/imgproc/imgproc.hpp>

int main() 
{
	try
	{	
		cv::Mat mammo_img = cv::imread(std::string(EXAMPLE_IMAGES_PATH) + "/raw_mammogram.tif", CV_LOAD_IMAGE_UNCHANGED);
		if(!mammo_img.data)
			throw aia::error("Cannot load image");
		printf("Image loaded: dims = %d x %d, channels = %d, bitdepth = %d\n",
			mammo_img.rows, mammo_img.cols, mammo_img.channels(), aia::bitdepth(mammo_img.depth()));
		aia::imshow("Original image", mammo_img, true, 0.3f);

		// log-transform (expands dark values, not tunable)
		cv::Mat mammo_img_copy = mammo_img.clone();		// make a clone of the original mammo, so we keep two different versions
		float L = 16384;			// gray levels = 2^b (b = number of bits = 14 in this case)
		float c = (L - 1)/log(L);	// normalization factor (so that input and output intensity ranges are the same)
		                            // it can be obtained by solving the following system:
		                            // 1) y = c * log(x+1)
		                            // 2) y(0) = 0
		                            // 3) y(L-1) = L-1
		for(int y=0; y<mammo_img_copy.rows; y++)
		{
			unsigned short* ythRow = mammo_img_copy.ptr<unsigned short>(y);
			//        /\
			//        || remember to use the right type. Here we deal with 14 bits images. 14 is the number of 'used' bits
			//           but 16 is the number of 'stored' bits (in CPU/memory, everything is multiple of bytes, 1 byte = 8 bits)

			for(int x=0; x<mammo_img_copy.cols; x++)
				ythRow[x] = unsigned short ( c * log(ythRow[x] + 1) );
		}
		aia::imshow("After log transform", mammo_img_copy, true, 0.3f);
		cv::imwrite(std::string(EXAMPLE_IMAGES_PATH) + "/raw_mammogram.log.tif", mammo_img_copy);
		
		// exp-transform / gamma-correction (expands dark values, tunable by 'gamma')
		float gamma = 0.1f;					// gamma factor, should be < 1 to expand dark levels
											// try with 0.5, 0.1, 0.05, 0.01, 0.001 and plot the image histogram in MATLAB or ImageJ
		mammo_img_copy = mammo_img.clone();	// make a clone of the original mammo, so we keep two different versions
		c = std::pow(L-1, 1-gamma);			// normalization factor (so that input and output intensity ranges are the same)
		                                    // it can be obtained by solving the following system:
		                                    // 1) y = c * x^gamma
		                                    // 2) y(0) = 0
		                                    // 3) y(L-1) = L-1
		for(int y=0; y<mammo_img_copy.rows; y++)
		{
			unsigned short* ythRow = mammo_img_copy.ptr<unsigned short>(y);

			for(int x=0; x<mammo_img_copy.cols; x++)
				ythRow[x] = unsigned short ( c * pow(ythRow[x], gamma) );
		}
		aia::imshow(aia::strprintf("After gamma-correction, gamma = %f", gamma), mammo_img_copy, true, 0.3f);
		//    /\
		//    || imshow can't properly display images that are not 'full' 8/16 bits (in this case, we have a 14-bit image)
		//    || we cannot rely on what we see...alternatively, we have to save the image in a format that supports
		//    || the bitdepth used, and open it with a proper visualization tool (e.g. ImageJ)
		cv::imwrite(std::string(EXAMPLE_IMAGES_PATH) + aia::strprintf("/raw_mammogram.gamma%.3f.tif", gamma), mammo_img_copy);

		
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

