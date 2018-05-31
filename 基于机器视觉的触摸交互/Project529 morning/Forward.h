#pragma once
#include<iostream>
#include <opencv2/opencv.hpp>
#include <list>
using namespace std;
using namespace cv;
Mat_<float> get_weight(int shape[2]);
Mat_<float> get_bias(int shape);
Mat_<float> sigmoid(Mat_<float>mat_mul);
Mat_<float> forward(Mat_<float> x);
int read_record(float * a, int length, string filename);
