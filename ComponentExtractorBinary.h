#ifndef __ComponentExtractorBinary_H_
#define __ComponentExtractorBinary_H_

class ComponentExtractorBinary  {

protected:
	DImage* _image;

public:
	ComponentExtractorBinary()  { ; }
	ComponentExtractorBinary(DImage* img) {_image = img;}
	~ComponentExtractorBinary(void) { ; }

	void setImage(DImage* img) { _image = img; }
	void extract(vector<ConnectedComponent*>&) ;
protected:
	void collectComponents(vector<vector<cv::Point>>& contours, vector<cv::Vec4i>& hierarchy, vector<ConnectedComponent*>& component);
};

#endif 
