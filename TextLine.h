#ifndef _TEXTLINE_H_ 
#define _TEXTLINE_H_ 

#include <iostream>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <math.h>
#include "ConnectedComponent.h"

using namespace cv ;
using namespace std;

class  TextLine {
protected :
	cv::Mat   _image ;
	cv::Rect  _rect  ;
	vector<cv::Point>   _boundary ;
	vector<cv::Point>* _upperBound ;
	vector<cv::Point>* _lowerBound ;

public:
	TextLine()  {;}
	TextLine(cv::Mat img){	_image = img; }
	~TextLine(void) {;}	
	void setLineBoundary(std::vector<ConnectedComponent*>& components);
	void setImage(cv::Mat& img) { _image = img; }
	void setUpperBound(vector<Point>* bound){ _upperBound = bound; }
	void setLowerBound(vector<Point>* bound){ _lowerBound = bound; }

	vector<cv::Point>& getBoundary(){ return _boundary; }
	Point getCenter(cv::Rect rec) { return Point(rec.x + rec.width / 2, rec.y - rec.height / 2); }
	vector<Point> getConvexHull(vector<Point> a, vector<Point> b);
};

#endif 