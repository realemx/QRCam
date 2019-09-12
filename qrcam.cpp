#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include "qrcam.h"


int main(int argc, char* argv[] ) {
	cv::VideoCapture stream1(0);

	if (!stream1.isOpened()) {
		std::cerr << "\t\033[1;31mcannot open camera!\033[0;30m" << std::endl;
		return 1;
	}

	cv::Mat inFrame, outFrame, points;
	cv::QRCodeDetector qrDecoder = cv::QRCodeDetector();

	namedWindow("Camera", cv::WINDOW_NORMAL);

	std::cout << std::endl;

	while (true) {
		stream1.read(inFrame);

		if(qrDecoder.detect(inFrame, points)) {
			std::cout << "\r" << qrDecoder.decode(inFrame, points);
			// making sure the line is clean by printing a lot of whitespaces
			std::cout << "                                                                                                 ";
			drawMatch(inFrame, points);
		}

		flip(inFrame, outFrame, +1);
		imshow("Camera", outFrame);

		if (cv::waitKey(30) >= 0) {
			break;
		}
	}

	std::cout << std::endl;
	return 0;
}

void drawMatch(cv::Mat image, cv::Mat points) {
	int n = points.rows;
	for(int i = 0 ; i < n ; i++)
	{
		line(image,
			 cv::Point(points.at<float>(i, 0), points.at<float>(i, 1)),
			 cv::Point(points.at<float>((i + 1) % n, 0), points.at<float>((i + 1) % n, 1)),
			 cv::Scalar(0, 0, 255), 3);
	}
}
