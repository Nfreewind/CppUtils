#ifndef NMS_H
#define NMS_H
#include <iostream>
#include <vector>
#include <opencv.hpp>
#include <algorithm>
class Nms
{
public:
	bool judge(const pair<cv::Rect, float> a, const pair<cv::Rect, float> b);
	void NmsMax(vector<cv::Rect>& rects, vector<float>& scores, int model, float overlap);
	float IoU(cv::Rect r1, cv::Rect r2, int model);
	vector<int> Nms(vector<Rect>& rects, vector<float>& scores, vector<int>& Srect, float overlap, Mat Img);
	int Partation(Mat predicate, vector<int>& label);
	int Find(vector<int>& parent, int x);
	vector<float> Logistic(vector<float> scores, vector<int> index);
};
#endif