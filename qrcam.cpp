#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include "qrcam.h"

using namespace cv;
using namespace std;


int main(int argc, char* argv[] )
{
	VideoCapture stream1(0);

	if (!stream1.isOpened()) {
		cerr << "\t\033[1;31mcannot open camera!\033[0;30m" << endl;
		return 1;
	}

	Mat inFrame, outFrame, points;
	QRCodeDetector qrDecoder = QRCodeDetector();

	namedWindow("Camera", WINDOW_NORMAL);

	cout << endl;

	while (true) {
		stream1.read(inFrame);

		if(qrDecoder.detect(inFrame, points)) {
			cout << "\r" << qrDecoder.decode(inFrame, points);
			// making sure the line is clean by printing a lot of whitespaces
			cout << "                                                                                                 ";
			drawMatch(inFrame, points);
		}

		flip(inFrame, outFrame, +1);
		imshow("Camera", outFrame);

		if (waitKey(30) >= 0) {
			break;
		}
	}

	cout << endl;
	return 0;
}

void drawMatch(Mat image, Mat points) {
	int n = points.rows;
	for(int i = 0 ; i < n ; i++)
	{
		line(image,
			 Point(points.at<float>(i, 0), points.at<float>(i, 1)),
			 Point(points.at<float>((i + 1) % n, 0), points.at<float>((i + 1) % n, 1)),
			 Scalar(0, 0, 255), 3);
	}
}
