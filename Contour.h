#ifndef _Contour_H 
#define _Contour_H 


#include <vector>
#include <opencv/cv.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

class  Contour {
protected:
	vector<cv::Point> _points;
	cv::Rect          _brect;
	cv::Point         _center;

public:
	Contour()  { ; }
	~Contour() { ; }

	Contour(std::vector<cv::Point>& points)  { 
		assert(points.size() > 2);
		_center = cv::Point(0, 0);
		_brect = cv::Rect(points[0], points[1]);
		_points.push_back(points[0]);
		_center += points[0];
		_points.push_back(points[1]);
		_center += points[1];
		for (int i = 2; i < points.size(); i++){
			_brect += points[i];
			_center += points[i];
			_points.push_back(points[i]);
		}
		_center /= (int)_points.size();
	}

	Contour(const Contour &contour)  {
		_points.insert(_points.end(), contour._points.begin(), contour._points.end());
		_brect  = contour._brect;
		_center = contour._center;
	}

	void set(const Contour &contour)  {
		_points.erase(_points.begin(), _points.end());
		_points.insert(_points.end(), contour._points.begin(), contour._points.end());
		_brect = contour._brect;
		_center = contour._center;
	}

	void setPoints(vector<cv::Point>& p)  {
		_points.erase(_points.begin(), _points.end());
		_points.insert(_points.end(), p.begin(), p.end());
		_brect = boundingRect(p);
		setCenter();
	}

	vector<cv::Point>& getPoints(){
		return _points;
	}

	cv::Rect&  getBoundRect() {
		return _brect;
	}

	float inside(cv::Point p){
		return (float)pointPolygonTest(_points, p, false);
	}

	void        setCenter();
	cv::Point   getCenter()            { return _center; }

	
	cv::Mat  getMat(vector<cv::Point>& pts);
	vector<cv::Point> getConvexHull(bool clockwise);
};

#endif 