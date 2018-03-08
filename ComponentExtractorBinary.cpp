#include <iostream>
#include "DImage.h"
#include "ImageTools.h"
#include "ComponentExtractorBinary.h"

using namespace cv;
using namespace std;

void ComponentExtractorBinary::collectComponents(vector<vector<Point>>& contours, vector<Vec4i>& hierarchy,
	vector<ConnectedComponent*>& components){

	if ( contours.size() > 0 ) {
		// Generate all the components 
		vector<vector<Point>>::iterator iter; 
		for ( iter = contours.begin(); iter != contours.end(); iter++ ){
			ConnectedComponent* component = new ConnectedComponent(*iter);
			components.push_back(component);
		}

		// Scan the the component and build the hierarchy 
		for ( unsigned int i = 0 ; i < components.size(); i++ ){
			ConnectedComponent* component = components[i];
			// Child
			if (hierarchy[i][2] >= 0 ){
				component->addChild(components[hierarchy[i][2]]);
			}

			// Parent 
			if (hierarchy[i][3] >= 0 )
				component->setParent(components[hierarchy[i][3]]);	
		}
	}
}



void  ComponentExtractorBinary::extract(vector<ConnectedComponent*>& components){
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<ConnectedComponent*> tmp_components;

	Mat mat = _image->getMat().clone();
	//opencv func
	findContours(mat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0,0) );
	collectComponents(contours, hierarchy, tmp_components);
	for each (ConnectedComponent* comp in tmp_components)
		if (!comp->getParent())
			components.push_back(comp);
}

