#include <iostream>
#include "Contour.h"

using namespace std;
using namespace cv;


void Contour::setCenter() {
	_center.x = 0;
	_center.y = 0;
	vector<Point>::iterator iter;
	for (iter = _points.begin(); iter != _points.end(); iter++){
		_center.x += (*iter).x;
		_center.y += (*iter).y;
	}
	_center.x /= _points.size();
	_center.y /= _points.size();
}


Mat Contour::getMat(vector<Point>& pts){
	Mat m((int)(pts.size()), 2, CV_64F);
	vector<Point>::iterator iter;
	int row = 0;
	for (iter = pts.begin(); iter != pts.end(); iter++, row++){
		m.at<double>(row, 0) = (*iter).x;
		m.at<double>(row, 1) = (*iter).y;
	}
	return m;
}


vector<Point> Contour::getConvexHull(bool clockwise){
	vector<Point> hull ;
	convexHull(_points, hull, clockwise);
	return hull ;
}

