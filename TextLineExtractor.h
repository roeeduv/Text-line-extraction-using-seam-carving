#ifndef _TEXTLINEEXTRACTOR_H 
#define _TEXTLINEEXTRACTOR_H 

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>

class TextLine ;
class DImage;

class  TextLineExtractor {
protected :
	DImage* _image ;

public:
	TextLineExtractor(void)  {;}
	~TextLineExtractor(void) {;}

	void setImage(DImage* img) { _image = img ;}
	virtual void extract(std::vector<TextLine*>& ) = 0;
};

#endif 