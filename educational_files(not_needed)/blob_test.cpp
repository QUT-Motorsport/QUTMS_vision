#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

	// Read image
	Mat im = imread( "cones_sim.png", IMREAD_COLOR );

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 10000;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 200;
    params.maxArea = 10000;
    //params.maxArea

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.4;
    params.maxCircularity = 0.7;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.2;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.1;
    params.maxInertiaRatio = 1;


	// Storage for blobs
	vector<KeyPoint> keypoints;


#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2

	// Set up detector with params
	SimpleBlobDetector detector(params);

	// Detect blobs
	detector.detect( im, keypoints);
#else 

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);   

	// Detect blobs
	detector->detect( im, keypoints);
#endif 

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	Mat im_with_keypoints;
	drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	// Show blobs
	imshow("keypoints", im_with_keypoints );
	waitKey(0);

}