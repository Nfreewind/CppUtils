#include "OpencvUtils.h"
using namespace std;
using namespace cv;

void OpencvUtils::ShowImage(const cv::Mat img, int delay)
{
	cv::namedWindow("show", 0);
	cv::imshow("show", img);
	cv::waitKey(delay);
}

cv::Mat OpencvUtils::DrawRect(Mat& img, const cv::Rect rects, const std::string text, const cv::Scalar color)
{
	cv::Mat img_ = img.clone();
	cv::rectangle(img_, rects, color, 2);
	cv::Point coordinate;
	coordinate.x = (rects.x + rects.width) / 2;
	coordinate.y = (rects.y + rects.height) / 2;
	cv::putText(img_, text, coordinate, 1, 1, color);
	return img_;
}

void OpencvUtils::flip_coordinate(std::vector<cv::Rect> &rects, const cv::Mat img)
{
	for (int i = 0; i < rects.size(); ++i) {
		rects[i].x = img.cols - rects[i].x - rects[i].width;
	}
}

void OpencvUtils::edgeExpansion_coordinate(cv::Mat img, vector<cv::Rect> &rects, vector<float> &scores, int gap) 
{
	vector<int> index;
	int W = img.cols;
	int H = img.rows;
	for (int i = 0; i < rects.size(); ++i) {
		int left = rects[i].x;
		int top = rects[i].y;
		int right = rects[i].width + left;
		int bottom = rects[i].height + top;
		int l = left - gap, t = top - gap, r = right - gap, b = bottom - gap;
		if (left >= gap && top >= gap && right <= W + gap
				&& bottom <= H + gap) {
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left < gap && top >= gap && right <= W + gap
				&& bottom <= H + gap) {
			l = 0;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left >= gap && top < gap && right <= W + gap
				&& bottom <= H + gap) {
			t = 0;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left >= gap && top >= gap && right > W + gap
				&& bottom <= H + gap) {
			r = W;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left >= gap && top >= gap && right <= W + gap
				&& bottom > H + gap) {
			b = H;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left < gap && top < gap && right <= W + gap
				&& bottom <= H + gap) {
			l = 0;
			t = 0;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left >= gap && top < gap && right > W + gap
				&& bottom <= H + gap) {
			t = 0;
			r = W;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left < gap && top >= gap && right <= W + gap
				&& bottom > H + gap) {
			l = 0;
			b = H;
			rects[i] = Rect(l, t, r - l, b - t);
		} else if (left >= gap && top >= gap && right > W + gap
				&& bottom > H + gap) {
			r = W;
			b = H;
			rects[i] = Rect(l, t, r - l, b - t);
		} else {
			index.push_back(i);
		}
	}
	for (int i = index.size() - 1; i >= 0; --i) {
		rects.erase(rects.begin() + index[i]);
		scores.erase(scores.begin() + index[i]);
	}
}