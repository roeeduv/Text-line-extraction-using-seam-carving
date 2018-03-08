#include "DImage.h"

using namespace cv;


DImage::DImage(cv::Mat mat){
	_mat = mat;
	_com = Point(-1, -1);
}

DImage::~DImage(void){
	_mat.deallocate();
}


cv::Mat& DImage::getMat(){
	return _mat;
}


DImage* DImage::negate(){
	DImage *img = new DImage(cv::Scalar::all(255) - _mat);
	return img;
}

void DImage::extractComponents(ComponentExtractorBinary& ex, vector<ConnectedComponent*>& components){
	ex.setImage(this);
	ex.extract(components);
}


DImage*  DImage::convert(int to_type){
	cv::Mat gray;
	if (_mat.type() == to_type)
		return this;

	if (_mat.channels() == 1){
		_mat.convertTo(gray, CV_8U);
		return new DImage(gray);
	}

	if (_mat.channels() == 3 && to_type == CV_8U) {
		cv::Mat gray;
		cvtColor(_mat, gray, CV_BGR2GRAY);
		return new DImage(gray);
	}
	return this;

}


void DImage::display(String win){
	namedWindow(win, cv::WINDOW_NORMAL);
	imshow(win, this->getMat());
}