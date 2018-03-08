#ifndef _DImage_H_ 
#define _DImage_H_



#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "Contour.h"
#include "TextLineExtractor.h"
#include <string>
#include "ConnectedComponent.h"
#include "ComponentExtractorBinary.h"


#define atuchar(r,c)    at<unsigned char>(r,c)
#define atuint(r,c)     at<unsigned int>(r,c)
#define atint(r,c)      at<int>(r,c)
#define atfloat(r,c)    at<float>(r,c)
#define atdouble(r,c)   at<double>(r,c) 

using namespace cv;

class  DImage {
	cv::Mat      _mat;
	cv::Mat      _mask;
	Contour      _boundary;
	Point2f      _com;   // Center Of Mass ;

public:
	const static int LAYOUT_UNSET = 0;
	const static int LAYOUT_TEXT = 1;
	const static int LAYOUT_DRAW = 2;
	const static int LAYOUT_IMAGE = 3;
	const static int LAYOUT_PARAGRAPH = 4;
	const static int LAYOUT_PAGE_MAIN = 5;
	const static int LAYOUT_PAGE_MARGIN = 6;

public:

	DImage(cv::Mat mat);
	~DImage(void);
	cv::Mat& getMat();
	DImage* negate();
	DImage* convert(int to_type);
	void extractComponents(ComponentExtractorBinary& ex, vector<ConnectedComponent*>& components);
	void display(String win);
};


#endif 