#include "stdext.h"
#include "ImageTools.h"
#include "ComponentExtractorBinary.h"
#include "TextLine.h"

using namespace cv;
using namespace std;



void TextLine::setLineBoundary(vector<ConnectedComponent*>& components) {
	if (components.size() > 0) {
		Rect brect = components[0]->getBoundRect();
		for (int i = 0; i < components.size(); i++){
			brect |= components[i]->getBoundRect();
		}

		Mat mask = Mat::zeros(brect.size(), CV_8UC1);
		vector<ConnectedComponent*>::iterator iter = components.begin();
		vector<Point> previousPlygon = (*(iter++))->getContour().getConvexHull(false);
		ImageTools::shift(previousPlygon, Point(-brect.x, -brect.y));
		cv::fillConvexPoly(mask, previousPlygon, 255);

		while (iter != components.end()){
			vector<Point> plygon = (*(iter++))->getContour().getConvexHull(false);
			ImageTools::shift(plygon, Point(-brect.x, -brect.y));
			vector<Point> ply = getConvexHull(plygon, previousPlygon);
			cv::fillConvexPoly(mask, ply, 255);
			stdext::copy(plygon, previousPlygon);
		}

		DImage dimg(mask);
		ComponentExtractorBinary extractor;
		vector<ConnectedComponent*> comps;
		dimg.extractComponents(extractor, comps);
		stdext::copy(comps[0]->getContour().getPoints(), _boundary) ;
		ImageTools::shift(_boundary, Point(brect.x, brect.y));
	}
}

vector<Point> TextLine::getConvexHull(vector<Point> a, vector<Point> b){
	vector<Point> contour;
	vector<Point> hull;

	contour.insert(contour.end(), a.begin(), a.end());
	contour.insert(contour.end(), b.begin(), b.end());
	convexHull(Mat(contour), hull, false);

	return hull;
}



