#include "vesseldetector.h"

VesselDetector::VesselDetector()
{

}

VesselDetector::VesselDetector(std::vector<cv::Mat> images, std::vector<cv::Mat> masks, std::vector<cv::Mat> truths)
{
    this->images = images;
    this->masks = masks;
    this->truths = truths;

    // give default values
    numKernels = 12;
    medianSize = 21;
    threshOffset = 15;
    gamma = 1.0;
    nlMeansDnoising_h = 3;
    nlMeansDnoising_templateWinSize = 3;
    nlMeansDnoising_searchWinSize = 13;
    clahe_limit = 2.0;
    clahe_size = cv::Size(5,5);
    contourTrimSize = cv::Size(5,5);
}

void VesselDetector::setDataset(std::vector<cv::Mat> images, std::vector<cv::Mat> masks, std::vector<cv::Mat> truths)
{
    this->images = images;
    this->masks = masks;
    this->truths = truths;
}

void VesselDetector::setParams(int numKernels, int medianSize, int threshOffset, float gamma, float nlMeansDnoising_h,
                               int nlMeansDnoising_templateWinSize, int nlMeansDnoising_searchWinSize,
                               double clahe_limit, cv::Size clahe_size, cv::Size contourTrimSize)
{
    this->numKernels = numKernels;
    this->medianSize = medianSize;
    this->threshOffset = threshOffset;
    this->gamma = gamma;
    this->nlMeansDnoising_h = nlMeansDnoising_h;
    this->nlMeansDnoising_templateWinSize = nlMeansDnoising_templateWinSize;
    this->nlMeansDnoising_searchWinSize = nlMeansDnoising_searchWinSize;
    this->clahe_limit = clahe_limit;
    this->clahe_size = clahe_size;
    this->contourTrimSize = contourTrimSize;
}

void VesselDetector::operator()()
{
    process();
}

std::vector<cv::Mat> VesselDetector::getImages()
{
    return images;
}

std::vector<cv::Mat> VesselDetector::getMasks()
{
    return masks;
}

std::vector<cv::Mat> VesselDetector::getTruths()
{
    return truths;
}

std::vector<cv::Mat> VesselDetector::getSegmentedList()
{
    return segmented;
}

cv::Mat VesselDetector::getImage(int i)
{
    return images[i].clone();
}

cv::Mat VesselDetector::getTruth(int i)
{
    return truths[i].clone();
}

cv::Mat VesselDetector::getMask(int i)
{
    return masks[i].clone();
}

cv::Point VesselDetector::getSeed(const cv::Mat &image, const cv::Mat &mask)
{
    cv::Mat im;
    cv::Mat mask2(im.size(),CV_8U,cv::Scalar(255));
    cv::Point pt;

    if (!mask.empty()) {
        cv::Mat se = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(50,50));
        cv::erode(mask,mask2,se);
        image.copyTo(im,mask2);
    }

    cv::erode(im,im,cv::Mat(),cv::Point(-1,-1),2);

    cv::minMaxLoc(im,0,0,0,&pt,mask2);

    return pt;
}

cv::Rect VesselDetector::getEnclosedBox(const cv::Mat &image)
{
    cv::Mat im;
    cv::Mat se = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(170,170));
    cv::erode(image,im,se,cv::Point(-1,-1),1);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(im, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    cv::Rect rect = cv::boundingRect(cv::Mat(contours[0]));

    return rect;
}

void VesselDetector::process()
{
    segmented.clear();

    for(int i=0; i<images.size(); i++)
    {
        cv::Mat result;
        segment(images[i], masks[i], result);
        segmented.push_back(result);
    }
}

void VesselDetector::segment(const cv::Mat &image, const cv::Mat &mask, cv::Mat &result)
{
    int key = -1;

    // vars & objects declaration
    cv::Mat im, grey, gammaCorrected, inverted, bg, bgSubstracted,
           nlMeansDenoised, equalized, tophat, tophatSum, thresholded, structElem;
    std::vector<cv::Mat> imrgb, kernel;

    // apply mask on the image to ensure non FOV pixels are set to 0
    image.copyTo(im, mask);
    Utils::imshow("original", im, key);

    // get greyscale image from green channel
    cv::split(im,imrgb);
    grey = imrgb[1];
    Utils::imshow("grayscale from green channel", grey, key);

    // apply gamma correction
    Utils::gammaCorrect(grey,gammaCorrected,gamma);
    Utils::imshow("gamma correct", gammaCorrected, key);

    // invert intensities
    inverted = 255 - gammaCorrected;
    Utils::imshow("intensities inverted", inverted, key);

    // substract background (median blur with big kernel) from image
    cv::medianBlur(inverted,bg,medianSize);
    bgSubstracted = inverted - bg;
    Utils::imshow("background substaction", bgSubstracted, key);

    // denoise with non-local means filtering
    cv::fastNlMeansDenoising(bgSubstracted,nlMeansDenoised,nlMeansDnoising_h,nlMeansDnoising_templateWinSize,nlMeansDnoising_searchWinSize);
    Utils::imshow("non-local means denoising", nlMeansDenoised, key);

    // enhance contrast with CLAHE
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clahe_limit, clahe_size);
    clahe->apply(nlMeansDenoised, equalized);
    Utils::imshow("hist equalized", equalized, key);

    // apply multiple tophats with rotating structuring element
    tophatSum.create(image.size(), CV_32F);
    tophatSum.setTo(cv::Scalar(0));
    kernel = Utils::createTiltedStructuringElements(17,1,numKernels);
    for (int i=0; i<numKernels; i++) {
        cv::morphologyEx(equalized,tophat,CV_MOP_TOPHAT,kernel[i]);
        Utils::imshow("tophat", tophat, key);
        tophat.convertTo(tophat,CV_32F);
        tophatSum += tophat;
    }
    cv::normalize(tophatSum, tophatSum, 0, 255, cv::NORM_MINMAX, CV_8U);
    Utils::imshow("total tophats", tophatSum, key);

    // threshold
    if (threshOffset > 0) {
        cv::threshold(tophatSum, thresholded, threshOffset, 255, CV_THRESH_BINARY);
    } else { // apply OTSU thresholding
        cv::threshold(tophatSum, thresholded, threshOffset, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    }

    Utils::imshow("total tophats threholded", thresholded, key);

    // remove round edge
    structElem = cv::getStructuringElement(cv::MORPH_ELLIPSE,contourTrimSize);
    cv::erode(mask,mask,structElem);
    thresholded.copyTo(result,mask);
    Utils::imshow("edge trim", result, key);

    cv::destroyAllWindows();
}
