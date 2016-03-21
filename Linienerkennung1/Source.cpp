#include "opencv2\imgproc.hpp"
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "iostream"

using namespace std;
using namespace cv;


int main(int argc, char** argv){

	Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	if (img.empty()) // make sure a image i loaded
	{
		cout << "!!! Failed imread(): image not found" << endl;
	}
	else {

		Mat img_erode;
		Mat img_smooth;
		Mat thresh;
		Mat lap_raw;
		Mat finish;

		erode(img, img_erode, getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(0, 0)));

		blur(img_erode, img_smooth, Size(9, 9), Point(-1, -1));

		double min, max;
		minMaxLoc(img_smooth, &min, &max);

		threshold(img_smooth, thresh, (max - min) / 2, 255, 0);

		Laplacian(thresh, lap_raw, CV_16S, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(lap_raw, finish);

		for (int y = 0; y < finish.rows; y++){
			int left = 0;
			int right = 0;
			int i = 0;

			while (i < finish.cols){
				if (finish.at<uchar>(y, i) == 255){
					left = i;
					break;
				}
				i++;
			}

			i = finish.cols - 1;
			while (i > 0){
				if (finish.at<uchar>(y, i) == 255){
					right = i;
					break;
				}
				i--;
			}

			finish.at<uchar>(y, ((right - left) / 2) + left) = 255; //Center of the Line
		}

		// displaying the Images

		namedWindow("Raw", WINDOW_AUTOSIZE);
		imshow("Raw", img);

		namedWindow("Eroded", WINDOW_AUTOSIZE);
		imshow("Eroded", img_erode);

		namedWindow("Smooth", WINDOW_AUTOSIZE);
		imshow("Smooth", img_smooth);

		namedWindow("Thresh", WINDOW_AUTOSIZE);
		imshow("Thresh", thresh);

		namedWindow("Final", WINDOW_AUTOSIZE);
		imshow("Final", finish);

		waitKey(0);
	}
	return 0;
}

