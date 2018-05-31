#include"test.h"

int argmax(Mat_<float> m)
{
	float max = m(0, 1);
	int argmax = 1;
	for (int i = 0; i < m.cols; i++)
	{
		//cout << m(0, i)<<endl;
		if (m(0, i) > (max+2.5))
		{
			
			max = m(0, i);
			argmax = i;

		}
	}
	return argmax;
}

int text(vector<Point> point_vetc)
{
	vector<float> data_vector = pretreat(point_vetc);
	int count = data_vector.size();
	float* f = new float[count];
	for (int i = 0; i < count; i++)
	{
		f[i] = data_vector[i];
	}
	Mat_<float> x = Mat_<float>(1, count, f);
	Mat_<float> y = forward(x);
	return argmax(y);

}
/*
int main() 
{
	vector<Point> point_vetc;
	Point a, b;
	a.x = 0;
	a.y = 1;
	b.x = 3;
	b.y = 6;
	for (int i = 0; i < 15; i++)
	{
		if(i%4==0)
			point_vetc.push_back(a);
		else
			point_vetc.push_back(b);

	}
	int result=text(point_vetc);
	cout<< result;
	system("pause");
}
*/
