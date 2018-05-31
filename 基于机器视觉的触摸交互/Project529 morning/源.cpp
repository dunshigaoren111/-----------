#include<iostream>
#include<opencv2\opencv.hpp>
#include<vector>
#include<cmath>
#include<Windows.h>
#include<opencv2\tracking.hpp>
#include <opencv2\videoio.hpp>  
#include <opencv2\highgui.hpp>
#include <cstdlib>
#include<cstdio>
#include"test.h"

using namespace cv;
using namespace std;

#define LEN_SEQ 15

Point pointFit(Mat fit, Point p)
{
	vector<Point2f> points, points_trans;
	points.emplace_back(Point2f(p.x, p.y));
	perspectiveTransform(points, points_trans, fit);
	Point p1;
	p1.x = points_trans[0].x;
	p1.y = points_trans[0].y;
	return p1;
}

Mat lineFit(Mat img, vector<Point> p)
{
	int width = img.cols;
	int height = img.rows;
	size_t len = p.size();
	vector<Point> point1, point2, point3, point4;
	for (size_t i = 0; i < len; i++)
	{
		int x = p[i].x;
		int y = p[i].y;
		if (x > 120 && x < (width - 170) && y < height / 2)
		{
			point1.push_back(p[i]);//上
		}
		if (x > 120 && x < (width - 170) && y > height / 2)
		{
			point2.push_back(p[i]);//下
		}
		if (x<width / 2 && y>150 && y<(height - 110))
		{
			point3.push_back(p[i]);//左
		}
		if (x>width / 2 && y>150 && y<(height - 110))
		{
			point4.push_back(p[i]);//右
		}
	}
	if (point1.size() && point2.size() && point3.size() && point4.size())
	{
		Vec4f line1;//计算四条直线
		Vec4f line2;
		Vec4f line3;
		Vec4f line4;

		fitLine(point1, line1, CV_DIST_HUBER, 0, 0.01, 0.01);//上框
		fitLine(point2, line2, CV_DIST_HUBER, 0, 0.01, 0.01);//下框
		fitLine(point3, line3, CV_DIST_HUBER, 0, 0.01, 0.01);//左框
		fitLine(point4, line4, CV_DIST_HUBER, 0, 0.01, 0.01);//右框
		double k1, k2, k3, k4;
		double b1, b2, b3, b4;
		if (line1[0])
		{
			double cos_theta = line1[0];
			double sin_theta = line1[1];
			double x0 = line1[2], y0 = line1[3];
			k1 = sin_theta / cos_theta;
			b1 = y0 - k1 * x0;

			double x1 = 5;
			double y1 = k1 * x1 + b1;
			double x = 635;
			double y = k1 * x + b1;

			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}
		else
		{
			double x1 = line1[2], y1 = 5;
			double x = line1[2], y = 450;
			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}

		if (line2[0])
		{
			double cos_theta = line2[0];
			double sin_theta = line2[1];
			double x0 = line2[2], y0 = line2[3];
			k2 = sin_theta / cos_theta;
			b2 = y0 - k2 * x0;

			double x1 = 5;
			double y1 = k2 * x1 + b2;
			double x = 635;
			double y = k2 * x + b2;

			line(img, Point(x1, y1), Point(x, y), Scalar(255, 255, 255), 1);
		}
		else
		{
			double x1 = line2[2], y1 = 5;
			double x = line2[2], y = 450;
			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}

		if (line3[0])
		{
			double cos_theta = line3[0];
			double sin_theta = line3[1];
			double x0 = line3[2], y0 = line3[3];
			k3 = sin_theta / cos_theta;
			b3 = y0 - k3 * x0;

			double y1 = 5;
			double x1 = (y1 - b3) / k3;
			double y = 460;
			double x = (y - b3) / k3;

			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}
		else
		{
			double x1 = line3[2], y1 = 5;
			double x = line3[2], y = 470;
			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}

		if (line4[0])
		{
			double cos_theta = line4[0];
			double sin_theta = line4[1];
			double x0 = line4[2], y0 = line4[3];
			k4 = sin_theta / cos_theta;
			b4 = y0 - k4 * x0;

			double y1 = 5;
			double x1 = (y1 - b4) / k4;
			double y = 460;
			double x = (y - b4) / k4;

			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}
		else
		{
			double x1 = line4[2], y1 = 5;
			double x = line4[2], y = 450;
			line(img, Point(x1, y1), Point(x, y), Scalar(0, 255, 255), 1);
		}

		int x1, y1, x2, y2, x3, y3, x4, y4;//计算四个点
		x1 = (b3 - b1) / (k1 - k3);
		y1 = x1 * k1 + b1;
		x2 = (b4 - b1) / (k1 - k4);
		y2 = x2 * k1 + b1;
		x3 = (b2 - b3) / (k3 - k2);
		y3 = x3 * k2 + b2;
		x4 = (b2 - b4) / (k4 - k2);
		y4 = x4 * k2 + b2;

		vector<Point2f> corners(4);
		corners[0] = Point2f(x1, y1);
		corners[1] = Point2f(x2, y2);
		corners[2] = Point2f(x3, y3);
		corners[3] = Point2f(x4, y4);

		vector<Point2f> corners_trans(4);
		corners_trans[0] = Point2f(0, 0);
		corners_trans[1] = Point2f(0, 766);
		corners_trans[2] = Point2f(1365, 0);
		corners_trans[3] = Point2f(1365, 766);

		Mat transform = getPerspectiveTransform(corners, corners_trans);//计算变换矩阵
		imshow("x", img);
		return transform;
	}
	else
	{
		Mat t;
		cout << "点的个数太少。";
		return t;
	}
}

Point getFingerPos(Mat img)
{
	Point p(0, 0);
	vector<vector<Point>> contours;
	findContours(img, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	double maxarea = 0;
	int maxAreaIdx = 0;
	if (contours.size() > 0)
	{
		for (int index = contours.size() - 1; index >= 0; index--)
		{
			double tmparea = contourArea(contours[index]);
			if (tmparea > maxarea)
			{
				maxarea = tmparea;
				maxAreaIdx = index;
			}
		}
		if (maxarea < 200)
			return p;
		vector<Point> hull;
		vector<int> hullsI;
		convexHull(Mat(contours[maxAreaIdx]), hull);

		int leftPos = 1366;
		int posIdx = 0;
		for (int i = hull.size() - 1; i >= 0; i--)
		{
			if (leftPos > hull[i].x)
			{
				leftPos = hull[i].x;
				posIdx = i;
			}
		}
		return hull[posIdx];
	}
	else
	{
		return p;
	}
}

Mat getHand(Mat img)
{
	Mat result, tmp;
	Mat Y, Cr, Cb;
	vector<Mat> channels;
	img.copyTo(tmp);
	float gamma = 0.9;
	for (int r = 0; r < tmp.rows; r++)
	{
		for (int c = 0; c < tmp.cols; c++)
		{
			tmp.at<Vec3b>(r, c)[0] = int(pow(tmp.at<Vec3b>(r, c)[0], gamma));
			tmp.at<Vec3b>(r, c)[1] = int(pow(tmp.at<Vec3b>(r, c)[1], gamma));
			tmp.at<Vec3b>(r, c)[2] = int(pow(tmp.at<Vec3b>(r, c)[2], gamma));
		}
	}
	cvtColor(tmp, result, COLOR_BGR2GRAY);
	cvtColor(tmp, tmp, COLOR_BGR2YCrCb);
	split(tmp, channels);
	Y = channels.at(0);
	Cr = channels.at(1);
	Cb = channels.at(2);
	float WCr = 0, WCb = 0, CbCenter = 0, CrCenter = 0;
	float Wcb = 46.97, Wcr = 38.76, WHCb = 14, WHCr = 10, WLCb = 23, WLCr = 20, Ymin = 16, Ymax = 235, Kl = 125, Kh = 188;

	for (int i = 0; i < Y.rows; i++)
	{
		uchar* currentY = Y.ptr<uchar>(i);
		uchar* currentCr = Cr.ptr<uchar>(i);
		uchar* currentCb = Cb.ptr<uchar>(i);
		uchar* current = result.ptr<uchar>(i);
		for (int j = 0; j < Y.cols; j++)
		{
			if (currentY[j] < Kl)
			{
				WCr = WLCr + (currentY[j] - Ymin) * (Wcr - WLCr) / (Kl - Ymin);
				WCb = WLCb + (currentY[j] - Ymin) * (Wcb - WLCb) / (Kl - Ymin);
				CrCenter = 154 - (Kl - currentY[j]) * (154 - 144) / (Kl - Ymin);
				CbCenter = 108 + (Kl - currentY[j]) * (118 - 108) / (Kl - Ymin);
			}
			else
			{
				if (currentY[j] > Kh)
				{
					WCr = WHCr + (currentY[j] - Ymax) * (Wcr - WHCr) / (Ymax - Kh);
					WCb = WHCb + (currentY[j] - Ymax) * (Wcb - WHCb) / (Ymax - Kh);
					CrCenter = 154 + (currentY[j] - Kh) * (154 - 132) / (Ymax - Kh);
					CbCenter = 108 + (currentY[j] - Kh) * (118 - 108) / (Ymax - Kh);
				}
			}
			if ((currentY[j] < Kl) || (currentY[j] > Kh))
			{
				currentCr[j] = (currentCr[j] - CrCenter) * Wcr / WCr + 154;
				currentCb[j] = (currentCb[j] - CbCenter) * Wcb / WCb + 108;
			}
			if (!((currentCr[j] > 130) && (currentCr[j] < 176) && (currentCb[j] > 70) && (currentCb[j] < 127)))
			{
				current[j] = 0;
			}
		}
	}
	//imshow("gray", result);
	threshold(result, result, 70, 255, THRESH_BINARY + THRESH_OTSU);
	//imshow("threshold", result);
	return result;
}
 
void  initBoundingBox(Point pos, Rect2d& boundingBox)
{
	if (pos.x >= 50)
		boundingBox.x = pos.x  - 50;
	else
		boundingBox.x = 0;
	if (pos.y  >= 50)
		boundingBox.y = pos.y - 50;
	else
		boundingBox.y = 0;
	if (boundingBox.x + 130 <= 640) {
		boundingBox.width = 130;
	}
	else {
		boundingBox.width = 640 - boundingBox.x;
	}
	if (boundingBox.y + 130 <= 480) {
		boundingBox.height = 130;
	}
	else {
		boundingBox.height = 480 - boundingBox.y;
	}
}

void imgArea(int& x,int& y, int& w, int& h, Rect2d fingerArea)
{
	if (fingerArea.x - 50 >= 0)
		x = fingerArea.x - 50;
	else
		x = 0;
	if (fingerArea.y - 50 >= 0)
		y = fingerArea.y - 50;
	else
		y = 0;
	if (fingerArea.x + fingerArea.width >= 680)
		w = 680 - fingerArea.x;
	else
		w = fingerArea.width;
	if (fingerArea.y + fingerArea.height >= 480)
		h = 480 - fingerArea.y;
	else
		h = fingerArea.height;
}

double distance(Point p1, Point p2) 
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void mouseMove(int& mousex, int& mousey, Point p1,Point p2)
{
	if (distance(p1, p2) >= 10 && distance(p1, p2) < 20)
	{
		mousex += int((p2.x - p1.x) * 2);
		mousey += int((p2.y - p1.y) * 2);
	}
	else if (distance(p1, p2)>=20&& distance(p1, p2)<30)
	{
		mousex += int((p2.x - p1.x)*3);
		mousey += int((p2.y - p1.y)*3);
	}
	else if(distance(p1, p2)>=30 && distance(p1, p2)<40)
	{
		mousex += int((p2.x - p1.x)*4);
		mousey += int((p2.y - p1.y)*4);
	}
	else if (distance(p1, p2) >= 40 && distance(p1, p2)<50)
	{
		mousex += int((p2.x - p1.x) * 5);
		mousey += int((p2.y - p1.y) * 5);
	}
	else
	{
		mousex += int((p2.x - p1.x) * 8);
		mousey += int((p2.y - p1.y) * 8);
	}
}

vector<Point> screenInit(Mat img)
{
	Mat imgHsv, tmp;
	vector<Mat> channels;
	cvtColor(img, tmp, COLOR_BGR2GRAY);
	cvtColor(img, imgHsv, COLOR_BGR2HSV);
	split(imgHsv, channels);
	Mat H, S, V;
	V = channels.at(2);
	for (int i = 0; i < img.rows; i++)
	{
		uchar* currentV = V.ptr<uchar>(i);
		uchar* current = tmp.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (currentV[j] > 0.8 * 255)
			{
				current[j] = 255;
			}
			else
			{
				current[j] = 0;
			}
		}
	}
	vector<vector<Point>> contours;
	findContours(tmp, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	double maxarea = 0;
	int maxAreaIdx = 0;
	for (int index = contours.size() - 1; index >= 0; index--)
	{
		double tmparea = contourArea(contours[index]);
		if (tmparea > maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = index;
		}
	}
	return  contours[maxAreaIdx];
}

int motionRecognize(vector<Point> seq)
{
	return text(seq);	
}

int main(void)
{
	int select = 1;

	int nScreenWidth, nScreenHeight;//获得电脑屏幕的参数
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	POINT p;
	GetCursorPos(&p);
	int mousex, mousey;
	mousex = p.x;
	mousey = p.y;

	TrackerKCF::Params param;//初始化KCF和参数
	param.desc_pca = TrackerKCF::GRAY;
	param.desc_npca = 0;
	param.compress_feature = true;
	param.compressed_size = 1;
	Ptr<Tracker> tracker = TrackerKCF::create(param);
	Rect2d boundingBox;//跟踪范围

	VideoCapture capture(1);//open camera
	Mat frame, result, fingerImg, tmp, fitMat;
	int count_frame = 0, x, y, w, h,movex=0,movey=0,move_frame=0;
	Vec4f line;//拟合运动直线

	//bool first_frame = true;
	vector<Point> pos;
	Point new_point;
	vector<Point> posSeq;
	vector<Point> screenPos;
	Rect2d fingerArea;//截取范围
	Size fingerSize;
	int k=1;
	int flagFit = 0;

	if (select == 0)
	{
		printf("请调整相机，保持屏幕处于合适位置\n");
		while (true)
		{
			capture >> frame;
			imshow("Init", frame);
			k = waitKey(30);
			if (k == 13)
			{
				screenPos = screenInit(frame);
				fitMat = lineFit(frame, screenPos);
				break;
			}
		}
	}

	while (capture.isOpened())
	{
		capture >> frame;
		if (count_frame == 5)//每隔六帧进行计数
			count_frame = 0;
		if (select == 0)
		{
			if (count_frame == 0)
			{
				resize(frame, tmp, Size(320, 240));//resize
				result = getHand(tmp);//get a bi-image of hand
				pos[2] = getFingerPos(result);//get a point of finger
				if (pos[2].x == 0 && pos[2].y == 0)
				{
					posSeq.clear();
					continue;
				}
				pos[2] *= 2;
				pos[1] = pos[2]; pos[0] = pos[1];
				pos[2] = pointFit(fitMat, pos[2]);
				initBoundingBox(pos[2], boundingBox);//确保不越界
				tracker->init(frame, boundingBox);
				circle(frame, pos[2], 5, CV_RGB(255, 0, 0), -1);//画追踪的点
				count_frame++;

				if (posSeq.size() < LEN_SEQ) {
					posSeq.push_back(pos[2]);
				}
				else
				{
					int ans = motionRecognize(posSeq);
					if (ans == 1)
					{
						vector<Point>::iterator k = posSeq.begin();
						posSeq.erase(k);
					}
					else
					{
						mousex = posSeq[0].x;
						mousey = posSeq[0].y;
						SetCursorPos(mousex, mousey);
						mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						posSeq.clear();
					}
				}
			}
			else
			{
				pos[0] = pos[1]; pos[1] = pos[2];
				bool ok = tracker->update(frame, boundingBox);//tracking
				rectangle(frame, boundingBox, Scalar(255, 0, 0), 2, 1);

				fingerArea = boundingBox;
				imgArea(x, y, w, h, fingerArea);
				Rect rect(x, y, w, h);
				fingerImg = frame(rect);
				//cout << x<<" " << y<<" " << w<< " " << h << endl;

				result = getHand(fingerImg);//get a bi-image of hand
				pos[2] = getFingerPos(result);//get a point of finger
				pos[2].x = pos[2].x + x;//计算原始点
				pos[2].y = pos[2].y + y;
				pos[2] = pointFit(fitMat, pos[2]);

				if (distance(pos[2], pos[1]) > 60 || distance(pos[2], pos[1])<20)//判断是否是正常点，是就discard
					pos[2] = pos[1];
				circle(frame, pos[2], 5, CV_RGB(255, 0, 0), -1);
				count_frame++;
				if (posSeq.size() < LEN_SEQ) {
					posSeq.push_back(pos[2]);
				}
				else
				{
					int ans = motionRecognize(posSeq);
					if (ans == 1)
					{
						vector<Point>::iterator k = posSeq.begin();
						posSeq.erase(k);
					}
					else
					{
						if (ans == 0)
						{
							mousex = posSeq[0].x;
							mousey = posSeq[0].y;
							SetCursorPos(mousex, mousey);
							mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
							posSeq.clear();
						}
					}
				}
			}
		}
		else
		{
			if (count_frame == 0)
			{
				
				resize(frame, tmp, Size(320, 240));//resize
				result = getHand(tmp);//get a bi-image of hand
				new_point = getFingerPos(result);//get a point of finger
				if (posSeq.size() < LEN_SEQ) {
					posSeq.push_back(new_point);
	 			}
				else
				{
					int ans = motionRecognize(posSeq);
					if (ans == 1)
					{
						vector<Point>::iterator k = posSeq.begin();
						posSeq.erase(k);
					}
					else
					{
						//printf("dddd");
						//mousex = posSeq[0].x;
						//mousey = posSeq[0].y;
						//SetCursorPos(mousex, mousey);
						//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						//posSeq.clear();
					}
				}


				if (flagFit==1)
				{
					fitLine(pos, line, CV_DIST_HUBER, 0, 0.01, 0.01);//拟合直线
					double k, b;
					Point pt;
					if (line[0])
					{
						double cos_theta = line[0];
						double sin_theta = line[1];
						double x0 = line[2], y0 = line[3];
						k = sin_theta / cos_theta;
						b = y0 - k * x0;
						int x1, y1, x2, y2;
						x1 = pos[0].x;
						x2 = (pos[5].x + (pos[5].y - b) / k) / 2;
						y1 = pos[0].y;
						y2 = (k*pos[5].x + b + pos[5].y) / 2;
						movex = (x2 - x1) / 5;
						movey = (y2 - y1) / 5;
						pt.x = x2;
						pt.y = y2;
					}
					else
					{
						movex = 0;
						movey = (pos[5].y - pos[0].y) / 5;
					}
					pos.clear();
					pos.push_back(pt);
				}
			
				if (new_point.x == 0 || new_point.y == 0|| new_point.x == 319 || new_point.y == 239)//没有手的话
				{
					pos.clear();
					movex = 0;
					movey = 0;
					flagFit = 0;
					posSeq.clear();
				}
				else {
					new_point *= 2;
					if (flagFit == 0)
					{
						pos.emplace_back(new_point);
						flagFit = 1;
					}
					pos.emplace_back(new_point);
					if (distance(pos[pos.size() - 1], new_point) > 70)
					{
						pos.clear();
						flagFit = 0;
					}
					initBoundingBox(new_point, boundingBox);//确保不越界
					tracker->init(frame, boundingBox);
					//rectangle(frame, boundingBox, Scalar(255, 0, 0), 2, 1);
					circle(frame, new_point, 5, CV_RGB(255, 0, 0), -1);//画追踪的点
					count_frame++;
				}

			}
			else
			{
				bool ok = tracker->update(frame, boundingBox);//tracking
				rectangle(frame, boundingBox, Scalar(255, 0, 0), 2, 1);
				fingerArea = boundingBox;
				imgArea(x, y, w, h, fingerArea);
				Rect rect(x, y, w, h);
				fingerImg = frame(rect);
				result = getHand(fingerImg);//get a bi-image of hand
				new_point = getFingerPos(result);//get a point of finger
				new_point.x = new_point.x + x;//计算原始点
				new_point.y = new_point.y + y;

				
			
				if (distance(pos[pos.size() - 1], new_point) > 70)
				{
					pos.clear();
					flagFit = 0;
					count_frame = -1;
				}
				else if (distance(pos[pos.size() - 1], new_point) < 10)
				{
					pos.emplace_back(pos[pos.size() - 1]);
					new_point = pos[pos.size() - 1];
				}
				else {
					pos.emplace_back(new_point);//加入新的点
				}
				circle(frame, new_point, 5, CV_RGB(255, 0, 0), -1);
				count_frame++;

				if (posSeq.size() < LEN_SEQ) {
					posSeq.push_back(new_point);
				}
				else
				{
					int ans = motionRecognize(posSeq);
					if (ans == 1)
					{
						vector<Point>::iterator k = posSeq.begin();
						posSeq.erase(k);
					}
					else
					{
						//printf("dddd");
						//mousex = posSeq[0].x;
						//mousey = posSeq[0].y;
						//SetCursorPos(mousex, mousey);
						//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						//posSeq.clear();
					}
				}
			}
		}
		
		GetCursorPos(&p);
		//if (mousex*mousex+mousey*mousey>=100&& mousex*mousex + mousey*mousey<400)
		{
			mousex = p.x + movex * 2;
			mousey = p.y + movey * 2;
		}/*
		else if (mousex*mousex + mousey*mousey >= 400 && mousex*mousex + mousey*mousey<900)
		{
			mousex = p.x + movex * 3;
			mousey = p.y + movey * 3;
		}
		else if (mousex*mousex + mousey*mousey >= 900 && mousex*mousex + mousey*mousey<1600)
		{
			mousex = p.x + movex * 4;
			mousey = p.y + movey * 4;
		}
		else if (mousex*mousex + mousey*mousey >= 1600 && mousex*mousex + mousey*mousey<2500)
		{
			mousex = p.x + movex * 5;
			mousey = p.y + movey * 5;
		}
		else
		{
			mousex = p.x + movex * 7;
			mousey = p.y + movey * 7;
		}*/
		if (mousex < 0)
			mousex = 0;
		if (mousey < 0)
			mousey = 0;
		if (mousex > nScreenWidth)
			mousex = nScreenWidth;
		if (mousey > nScreenHeight)
			mousey = nScreenHeight;
		SetCursorPos(mousex, mousey);

		imshow("input", frame);
		k = waitKey(10);
		if (k == 27)
			break;
		if (k == 32)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}
	return 0;
}