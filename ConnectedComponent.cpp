#include "ConnectedComponent.h"

using namespace std;
using namespace cv;

ConnectedComponent::ConnectedComponent(vector<cv::Point>& contour){
	_contour.setPoints(contour);
	_parent = 0 ;
}


void ConnectedComponent::addChild(ConnectedComponent* component) { 
	component->setParent(this);
	_children.push_back(component); 
}
