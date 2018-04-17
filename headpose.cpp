#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include<time.h>
#include "opencv2/calib3d/calib3d.hpp"


using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	//Mat im, image; //also put back with forloop
	Mat image;
	VideoCapture capture;
	int dev_id = 0; //Device number, not used yet

/*	capture.open(dev_id);
	if (!capture.isOpened()) {
		cerr << "Failed to open video device "
			<< dev_id << " \n" << endl;
		return 1;
	}

	for (;;) {
		capture >> im;
		if (im.empty())
			continue; 
*/ //commented-out forloop i keep mentioning


		// Read input image, to try lena before video
		cv::Mat im = cv::imread("lena_color.bmp");  //comment out when i comment-in forloop

		// 2D image points, different vector needed for diff image
		std::vector<cv::Point2d> image_points;
		image_points.push_back(cv::Point2d(359, 391));    // Nose tip
		image_points.push_back(cv::Point2d(399, 561));    // Chin
		image_points.push_back(cv::Point2d(337, 297));     // Left eye left corner
		image_points.push_back(cv::Point2d(513, 301));    // Right eye right corner
		image_points.push_back(cv::Point2d(345, 465));    // Left Mouth corner
		image_points.push_back(cv::Point2d(453, 469));    // Right mouth corner

		// 3D model points
		std::vector<cv::Point3d> model_points;
		model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
		model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
		model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
		model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
		model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
		model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

																			 // Camera internals
		double focal_length = im.cols; // Approximate focal length.
		Point2d center = cv::Point2d(im.cols / 2, im.rows / 2);
		cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
		cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion

		cout << "Camera Matrix " << endl << camera_matrix << endl;
		// Output rotation and translation
		cv::Mat rotation_vector; // Rotation in axis-angle form
		cv::Mat translation_vector;

		// Solve for pose
		cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);


		// Project a 3D point (0, 0, 1000.0) onto the image plane.
		// We use this to draw a line sticking out of the nose

		vector<Point3d> nose_end_point3D;
		vector<Point2d> nose_end_point2D;
		nose_end_point3D.push_back(Point3d(0, 0, 1000.0));

		cv::projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);


		for (int i = 0; i < image_points.size(); i++)
		{
			circle(im, image_points[i], 3, Scalar(0, 0, 255), -1);
		}

		cv::line(im, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);

		cout << "Rotation Vector " << endl << rotation_vector << endl;
		cout << "Translation Vector" << endl << translation_vector << endl;

		cout << nose_end_point2D << endl;

		// Display image.
		cv::imshow("Output", im);
		cv::waitKey(0);
	}

//} //while trying to read lena. have to put back when i de-comment for loop in beginning