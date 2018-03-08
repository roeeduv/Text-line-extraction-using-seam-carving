#ifndef _IMAGE_TOOLS_H 
#define _IMAGE_TOOLS_H


#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "DImage.h"

using namespace cv;

typedef std::pair<cv::Point2f, cv::Point2f>   Orientation;

namespace ImageTools {
	static int  Index;


	cv::Point2f  centerOfMass(cv::Mat mat);
	cv::Mat      varianceAroundPoint(cv::Mat m, cv::Point2f point);
	void  shift(std::vector<cv::Point>& pts, cv::Point offset);
	void fillComponents(cv::Mat mat, std::vector<ConnectedComponent*>& components, float filler, float boundary);
	cv::Mat  draw(cv::Mat mat, std::vector<ConnectedComponent*>& components, cv::Scalar clr );
	void  display(String win, Mat& img);
	void  display(String win, int idx, Mat& img);
	void  displayFresh(String win, Mat& img);
}

#endif 
