#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <map>
#include <fstream>
extern "C"
{
#include "sqlite3.h"
};
using namespace std;

#define Random(x) (rand() % x) 
sqlite3 *db;

struct colr{
	string upper;
	string lower;
	cv::Rect body;
};

vector<int> GetImageIds(){
	vector<int> res;
	const char *sql_select = "select image_id from Images where file_path like '11\%'";
	char** pResult;
	int nRow;
	int nCol;
	int rc = sqlite3_get_table(db, sql_select, &pResult, &nRow, &nCol, NULL);
	if (rc == SQLITE_OK)
	{
		if (nRow >= 1)
		{
			for (int i = nCol; i < nCol*(nRow+1); ++i)
			{
				res.push_back(atoi(pResult[i]));
			}
		}
	}
	sqlite3_free_table(pResult);
	//cout << "图片数量为："<<res.size() << endl;
	//std::random_shuffle(res.begin(),res.end());
	return res;
}

vector<int> GetFigureId(int image_id)
{
	std::vector<int> figures;
	stringstream ss;
	string sql_figures;
	ss << image_id;
	sql_figures = "select figure_id from Figures where image_id = " + ss.str();
	ss.str("");
	const char *sql = sql_figures.c_str();

	char** pResult;
	int nRow;
	int nCol;
	int rc = sqlite3_get_table(db, sql, &pResult, &nRow, &nCol, NULL);
	if (rc == SQLITE_OK)
	{
		for (int i = nCol; i < (nRow + 1)*nCol; ++i)
		{
			figures.push_back(atoi(pResult[i]));
		}
	}
	sqlite3_free_table(pResult);
	cout << "人物数量为：" << figures.size() << endl;
	return figures;
}

vector<int> GetBodyRects(int figure_id){
	vector<int> res;
	stringstream ss;
	ss << figure_id;
	string sql = "select body_left,body_top,body_width,body_height from Figures where figure_id = " + ss.str();
	const char *sql_select = sql.c_str();
	char** pResult;
	int nRow;
	int nCol;
	int rc = sqlite3_get_table(db, sql_select, &pResult, &nRow, &nCol, NULL);
	if (rc == SQLITE_OK)
	{
		if (nRow >= 1)
		{
			for (int i = 0; i < nCol; ++i){
				res.push_back(atoi(pResult[nCol+i]));
			}
		}
	}
	sqlite3_free_table(pResult);
	return res;
}

vector<string> split(string str, string separator)
{
	std::vector<string> result;
	int cutAt;
	while ((cutAt = str.find_first_of(separator)) != str.npos)
	{
		if (cutAt > 0)
		{
			result.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if (str.length() > 0)
	{
		result.push_back(str);
	}
	return result;
}

map<int, colr> GetFigures()
{
	map<int, colr> res;
	string sql_figures;
	sql_figures = "select figure_id,upper_color,lower_color,body_left,body_top,body_width,body_height from Figures where is_full=\"yes\"";
	const char *sql = sql_figures.c_str();

	char** pResult;
	int nRow;
	int nCol;
	int rc = sqlite3_get_table(db, sql, &pResult, &nRow, &nCol, NULL);
	if (rc == SQLITE_OK)
	{
		for (int i = nCol; i < (nRow + 1)*nCol - 2; i = i + 7)
		{
			int figure_id = atoi(pResult[i]);
			colr color;
			color.upper = pResult[i + 1];
			color.lower = pResult[i + 2];
			color.body.x = atoi(pResult[i + 3]);
			color.body.y = atoi(pResult[i + 4]);
			color.body.width = atoi(pResult[i + 5]);
			color.body.height = atoi(pResult[i + 6]);
			res[figure_id] = color;
		}
	}
	sqlite3_free_table(pResult);
	cout << "人物数量为：" << res.size() << endl;
	return res;
}

string getImagePath(int figure_id)
{
	string res;
	stringstream ss;
	ss << figure_id;
	string sql = "select file_path from Images,Figures where Figures.image_id=Images.image_id and figure_id=" + ss.str();
	const char *sql_select = sql.c_str();
	char** pResult;
	int nRow;
	int nCol;
	int rc = sqlite3_get_table(db, sql_select, &pResult, &nRow, &nCol, NULL);
	if (rc == SQLITE_OK)
	{
		if (nRow >= 1)
		{
			res = pResult[nCol];
		}
	}
	sqlite3_free_table(pResult);
	cout << "图片路径为：" << res << endl;
	return res;
}

int main()
{
	char *db_path = "../feature_v2.sqlite";
	int rc = sqlite3_open(db_path, &db);
	int red_c = 0, green_c = 0, blue_c = 0, cyan_c = 0, magenta_c = 0, yellow_c = 0, white_c = 0, black_c = 0;

	if (rc == SQLITE_OK){
		map<int, colr> figures = GetFigures();
		map<int, colr>::iterator it;
		int count = 0;
		for (it = figures.begin(); it != figures.end(); ++it){
			string image_path = getImagePath(it->first);
			vector<string> image_pathes = split(image_path, "\\");
			string abs_path = "/raid/zhangzhaofeng/zhangbin/Pedestrian/" + image_pathes[0]+ "/" + image_pathes[1];
			colr color = it->second;

			cv::Mat image = cv::imread(abs_path);
			if (color.body.x < 0 || color.body.x > image.cols || color.body.y > image.rows || color.body.y < 0 
				|| color.body.x + color.body.width > image.cols ||color.body.y + color.body.height > image.rows)
			{
				continue;
			}
			cv::Mat body_image = image(color.body);
			string name = image_pathes[1];
			
			if (color.lower == "red"){
				name = "red-" + name;
				if (++red_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "green"){
				name = "green-" + name;
				if (++green_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "blue"){
				name = "blue-" + name;
				if (++blue_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "cyan"){
				name = "cyan-" + name;
				if (++cyan_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "magenta"){
				name = "magenta-" + name;
				if (++magenta_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "yellow"){
				name = "yellow-" + name;
				if (++yellow_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "white"){
				name = "white-" + name;
				if (++white_c <= 500)
					cv::imwrite(name, body_image);
			}
			else if (color.lower == "black"){
				name = "black-" + name;
				if (++black_c <= 500)
					cv::imwrite(name, body_image);
			}
			if (black_c >= 500 && white_c >= 500 && yellow_c >= 500 && magenta_c >= 500 && cyan_c >= 500 && blue_c >= 500 && green_c >= 500 && red_c >= 500) 
			{
				break;
			}
		}
	}

	sqlite3_close(db);
	return 0;
}
