#ifndef _CONNECTEDCOMPONENT_H 
#define _CONNECTEDCOMPONENT_H



#include <vector>
#include <string>
#include "Contour.h" 

class ConnectedComponent {
	const static int ABOVE =  1 ;
	const static int BELOW = -1 ;
	const static int BORDER = 1 ;
	const static int VERTEX = 2 ;
	const static int ON_EDGE = 3 ;

protected:

	cv::Mat                   _image   ;
	ConnectedComponent*       _parent  ;
	Contour                   _contour ;
	string                    _string  ;
	vector<ConnectedComponent*> _children ;
	
public:
	ConnectedComponent(vector<cv::Point>& contour);
	cv::Mat  getImage()              { return _image ; } 
	cv::Rect                          getBoundRect() { return _contour.getBoundRect(); }
	Contour& getContour()        { return _contour ;}
	ConnectedComponent*               getParent()      { return _parent; }
	vector<ConnectedComponent*>&      getChildren()    { return _children; }
	void setImage(cv::Mat img) { _image = img; }
	void setParent(ConnectedComponent* parent)        { _parent   = parent; }
	void addChild(ConnectedComponent* component);
};

#endif 