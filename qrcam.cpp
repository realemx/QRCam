#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <getopt.h>
#include "qrcam.h"


int main(int argc, char* argv[] ) {

	char option;

	// Options set by arguments
	int videoDevice = 0;
	bool onlyReadOne = false;
	std::string winName = "Camera";
	float alpha = 0.5;
	int beta = 70;


	while ((option = getopt(argc, argv, "ohv:w:a:b:")) != EOF) {
		switch(option) {
			case 'v':
				videoDevice = std::stoi(optarg);
				break;
			case 'h':
				printhelp();
				return 0;
			case 'o':
				onlyReadOne = true;
				break;
			case 'w':
				winName = optarg;
				break;
			case 'a':
				alpha = std::stof(optarg);
				break;
			case 'b':
				beta = std::stoi(optarg);
				break;
			default:
				return 1;
		}
	}

	argc -= optind;
	argv += optind;

	cv::VideoCapture stream1(videoDevice);

	if (!stream1.isOpened()) {
		std::cerr << "\t\033[1;31mcannot open camera!\033[0;30m" << std::endl;
		return 1;
	}

	cv::Mat inFrame, outFrame, points;
	cv::QRCodeDetector qrDecoder = cv::QRCodeDetector();

	namedWindow(winName, cv::WINDOW_NORMAL);

	std::cout << std::endl;

	bool readQR = false;
	std::string qrString;
	while (true) {
		stream1.read(inFrame);

		// Copy frame before greyscale conversion
		outFrame = inFrame.clone();

		cv::cvtColor(inFrame, inFrame, cv::COLOR_BGR2GRAY);
		inFrame.convertTo(inFrame, -1, alpha, beta);

		if(qrDecoder.detect(inFrame, points)) {
			qrString = qrDecoder.decode(inFrame, points);
			std::cout << "\r" << qrString;
			// making sure the line is clean by printing a lot of whitespaces
			std::cout << "                                                                                            ";
			//Draw outlines on the colored image
			drawMatch(outFrame, points);
			if (!qrString.empty()) {
				readQR = true;
			}
		}

		//Flip the image for a mirror like video
		flip(outFrame, outFrame, +1);
		imshow(winName, outFrame);

		if (cv::waitKey(30) >= 0 || (readQR && onlyReadOne)) {
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

void printhelp() {
	std::cout << "Usage: qrcam [OPTIONS]" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t-o:\t\tRead one code and exit the program automatically" << std::endl;
	std::cout << "\t-h:\t\tPrint this help site" << std::endl;
	std::cout << "\t-v id\t\tSelect video devide with the id  (Default: 0)" << std::endl;
	std::cout << "\t-w name\t\tSets the Window Name to NAME (Default: Camera)" << std::endl;
	std::cout << "\t-a value\t\tSets a custom alpha value (Default: 0.5)" << std::endl;
	std::cout << "\t-b value\t\tSets a custom beta value (Default: 70)" << std::endl;
	std::cout << "" << std::endl;
}
