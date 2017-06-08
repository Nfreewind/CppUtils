#ifndef OPENCVUTILS_H
#define OPENCVUTILS_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

class OpencvUtils
{
public:
	void ShowImage(const cv::Mat img, int delay);
	cv::Mat DrawRect(Mat& img, const cv::Rect rects, const std::string text, const cv::Scalar color);
	void flip_coordinate(std::vector<cv::Rect> &rects, const cv::Mat img);
	void edgeExpansion_coordinate(cv::Mat img, vector<cv::Rect> &rects, vector<float> &scores, int gap);
};
#endif