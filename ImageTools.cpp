#include "ImageTools.h"


using namespace cv;
using namespace std;


void ImageTools::shift(vector<Point>& pts, Point offset){
	vector<Point>::iterator iter;
	for (iter = pts.begin(); iter != pts.end(); iter++)
		*iter += offset;
}


Point2f ImageTools::centerOfMass(Mat mat){
	Point2f center(0, 0);
	float sum = 0;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++){
			float s = mat.at<uchar>(i, j);
			float s2 = s*s;
			sum += s2;
			center.y += (i * s2);
			center.x += (j * s2);
		}
	}
	center.x /= sum;
	center.y /= sum;
	return center;
}

Mat ImageTools::varianceAroundPoint(Mat m, Point2f point){
	Mat variance(m.cols * m.rows, 2, CV_32F);
	int idx = 0;
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++){
		float s = m.at<uchar>(i, j);
		variance.at<float>(idx, 0) = (float)(i - point.y) * s;
		variance.at<float>(idx, 1) = (float)(j - point.x) * s;
		idx++;
		}
	return variance;
}

void ImageTools::fillComponents(Mat mat, std::vector<ConnectedComponent*>& components, float filler, float boundary){
	vector < vector < Point >> plys;

	for each (ConnectedComponent* comp in components){
		plys.push_back(comp->getContour().getPoints());
	}

	fillPoly(mat, plys,  filler);
	polylines(mat, plys, true, boundary);
}

Mat ImageTools::draw(Mat mat, std::vector<ConnectedComponent*>& components, Scalar clr){
	vector < vector < Point >> polys;
	for each (ConnectedComponent* comp in components){
		polys.push_back(comp->getContour().getPoints());
	}

	Mat img;
	cvtColor(mat, img, CV_GRAY2RGB);
	polylines(img, polys, true, clr);
	imshow("Polyline ", img);
	return img;
}


void ImageTools::display(String win, int idx, Mat& img){
	char buf[32];
//	sprintf_s(buf, "-[%d]", idx);
	namedWindow(win + String(buf),WINDOW_NORMAL);
	imshow(win + String(buf), img);
}

void ImageTools::displayFresh(String win, Mat& img){
	display(win, Index++, img);
}