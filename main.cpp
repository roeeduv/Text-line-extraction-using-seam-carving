#include "main.h"

using namespace cv;
using namespace std;


int prevY;

struct {
	bool operator()(ConnectedComponent* a, ConnectedComponent* b) {
		return a->getContour().getCenter().x < b->getContour().getCenter().x;
	}
} comp;


int getMinRow(Mat energy_map, int row, int col) {
	float min_val = energy_map.at<float>(row, col);
	int min_row = row;
	for (int i = -1; i <= 1; i++)
		if ((row + i >= 0) && (row + i < energy_map.rows) && energy_map.at<float>(row + i, col) < min_val) {
			min_row = row + i;
			min_val = energy_map.at<float>(row + i, col);
		}
	return min_row;
}

vector<cv::Point> getNextSeam(Mat energy_map) {
	Point min_pt;
	double min_val, max_val;
	//	cout << "Col: " << energy_map.col(energy_map.cols - 2) << endl ;
	
	minMaxLoc(energy_map.col(energy_map.cols - 2), &min_val, &max_val, &min_pt);
	min_pt.x = energy_map.cols - 2;

	//cout << "Min: " << min_pt << " MinVal: " << min_val << " MaxVal: " << max_val << endl;
	vector<Point> seam;
	int row = min_pt.y;
	seam.push_back(min_pt);
	for (int col = min_pt.x; col > 0; col--) {
		//	cout << "Row: " << row << " Col: " << col << endl;
		row = getMinRow(energy_map, row, col);
		seam.push_back(Point(col, row));
	}
	return seam;
}

//finds the component that intersect with the seam
vector<ConnectedComponent*> getSeamComponents(vector<Point> seam, vector<ConnectedComponent*> &component_list) {
	vector<ConnectedComponent*> seam_components;
	vector<ConnectedComponent*> rect_components;
	Rect seam_rect = boundingRect(seam);

	for each (ConnectedComponent* comp in component_list) {
		if ((seam_rect & comp->getBoundRect()).area() > 0) {
			rect_components.push_back(comp);
		}
	}
	sort(rect_components.begin(), rect_components.end(), comp);

	for (int i = 0; i < seam.size(); i++) {
		vector<ConnectedComponent*>::iterator iter;
		for (iter = rect_components.begin(); iter != rect_components.end(); iter++) {

			if ((*iter)->getContour().getBoundRect().contains(seam[i]) &&
				(*iter)->getContour().inside(seam[i]) >= 0) {
				seam_components.push_back((*iter));
				rect_components.erase(iter);
				break;
			}
		}
	}
	return seam_components;
}

float minValueOfThreeRows(Mat img, int row, int col) {
	return min(img.at<float>(row - 1, col), min(img.at<float>(row, col), img.at<float>(row + 1, col)));
}

/*calculate energy map based on formula from article*/
Mat getEnergyMapFromDistance(Mat dist_map) {
	Mat energy_map = Mat::zeros(dist_map.size(), CV_32FC1);
	dist_map.col(0).copyTo(energy_map.col(0));
	dist_map.row(0).copyTo(energy_map.row(0));

	int row, col;
	for (col = 1; col < energy_map.cols; col++)
		for (row = 1; row < energy_map.rows - 1; row++) {
			float min_val = minValueOfThreeRows(energy_map, row, col - 1);
			energy_map.at<float>(row, col) = 2.0*dist_map.at<float>(row, col) + min_val;
		}

	// Last Row 
	row = energy_map.rows - 1;
	for (int col = 1; col < energy_map.cols; col++) {
		float min_val = min(energy_map.at<float>(row - 1, col - 1), energy_map.at<float>(row, col - 1));
		energy_map.at<float>(row, col) = 2.0*dist_map.at<float>(row, col) + min_val;
	}
	return energy_map;
}

void fixEnergyMap(Mat energy_map,Mat norm_energy_map) {
	Rect rect(0, prevY ,energy_map.cols, energy_map.rows);
	prevY = prevY - 20;
	//cout << rect << endl;
	//cout <<"prevY: " << prevY << endl;
	rectangle(energy_map, rect, 255, CV_FILLED);
	normalize(energy_map, norm_energy_map, 0, 1, NORM_MINMAX, CV_32F);
	//	ImageTools::displayFresh(" Energy Map ", energy_map);
	//ImageTools::displayFresh(" norm_energy_map energy map ", norm_energy_map);
}


void removeTextLineFromEnergeyMap(Mat energy_map, TextLine& text_line) {
	double mx, mn;
	minMaxLoc(energy_map, &mn, &mx);
	cout << "Boundary: " << text_line.getBoundary().size() << endl;

	Rect rect = boundingRect(text_line.getBoundary());
	cout << "height " << rect.height << "  width " << rect.width << "  x " << rect.x << "  y " << rect.y << endl;
	rect.x = 0;
	rect.width = energy_map.cols - 1;
	cout << "Rect: " << rect << endl;
	rectangle(energy_map, rect, (float)mn, CV_FILLED);
}

//draws the seam
void drawDisplay(vector<Point> seam, DImage* _image) {
	vector < vector < Point >> polys;

	polys.push_back(seam);
	Mat img;

	cvtColor(_image->getMat(), img, CV_GRAY2RGB);
	polylines(img, seam, false, cv::Scalar(255, 0, 0));
	ImageTools::displayFresh("Polyline ", img);
}


int main(int argc, char * argv[]) {
	Mat mat = imread("photo6.JPG");
		DImage* source = new DImage(mat);
		DImage* gry = source->convert(CV_8U);
	//	gry->display("Gray Image");

		vector<TextLine*> lines;
		ComponentExtractorBinary component_extractor;
		vector<ConnectedComponent*> component_list;

		// Extarct the connected components from the binary image 
		DImage* negative = gry->negate();
		negative->extractComponents(component_extractor, component_list);
		DistanceTransformSigned dist(gry);

		//find all the component in the image
		dist.setComponents(&component_list);

		//creating the distance map
		DImage* dist_map = dist.transform();
		Mat norm_dist = Mat(dist_map->getMat().size(), CV_32FC1);
		normalize(dist_map->getMat(), norm_dist, 0, 1, NORM_MINMAX, CV_32F);
		imshow(" Distance ", norm_dist);

		//creating the energy map from distance map
		Mat energy_map = getEnergyMapFromDistance(dist_map->getMat());
		Mat norm_energy_map = Mat(energy_map.size(), CV_32FC1);
		normalize(energy_map, norm_energy_map, 0, 1, NORM_MINMAX, CV_32F);
		imshow(" Energy Map vis ", norm_energy_map);

		prevY = energy_map.rows;

		//creating all the seams until there is no component left
		while (component_list.size() > 0 && prevY > 0) {
			vector<cv::Point> seam = getNextSeam(norm_energy_map);
			vector<ConnectedComponent*> seam_components = getSeamComponents(seam, component_list);
			//	cout << "Componet List: " << component_list.size() << " Seam Components: " << seam_components.size() << endl;

			//only if seam going through components draw it, otherwise just update the enetgy map. 
			if (seam_components.size() > 0) {
				drawDisplay(seam, gry);
				waitKey(0);
				TextLine* text_line = new TextLine(gry->getMat());
				text_line->setLineBoundary(seam_components);
				lines.push_back(text_line);
				stdext::subtract(component_list, seam_components);
				removeTextLineFromEnergeyMap(energy_map, *text_line);
				normalize(energy_map, norm_energy_map, 0, 1, NORM_MINMAX, CV_32F);
				//	ImageTools::displayFresh(" Energy Map ", energy_map);
				ImageTools::displayFresh(" norm_energy_map energy map ", norm_energy_map);
			}
			else
				fixEnergyMap(energy_map, norm_energy_map);
		}

		//cout << "Text Lines " << lines.size() << endl;
		waitKey(0);

		return 0;
	
}
