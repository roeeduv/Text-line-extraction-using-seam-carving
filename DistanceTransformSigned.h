#ifndef _SIGNED_DISTANCE_TRANSFORMATION_ 
#define _SIGNED_DISTANCE_TRANSFORMATION_

#include <vector>
#include "ConnectedComponent.h"
#include "DImage.h"

using namespace cv;

class  DistanceTransformSigned  {
	
protected:
	vector<ConnectedComponent*>* _components;
	DImage *_image;
	void updateSignedDistaceMap(Mat map, int row, int col, int step, int delta);

public:
	DistanceTransformSigned(void) {;}
	DistanceTransformSigned(DImage* img){ setImage(img); }
	~DistanceTransformSigned(void){;}
	void setImage(DImage *img) { _image = img; }
	void setComponents(vector<ConnectedComponent*>* components){ _components = components ; }
	vector<ConnectedComponent*>* getComponents(){return _components; }
	DImage * transform();
};

#endif 