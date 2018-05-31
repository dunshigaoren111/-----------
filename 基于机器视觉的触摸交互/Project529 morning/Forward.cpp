#include"Forward.h"
int INPUT_NODE = 28;
int OUTPUT_NODE = 2;
int LAYER1_NODE = 13;
string w1_record = "./record/w1_record.txt";
string w2_record= "./record/w2_record.txt";
string b1_record= "./record/b1_record.txt";
string b2_record= "./record/b2_record.txt";

Mat_<float> get_weight(int shape[2], string filename)
{
	int rows = shape[0];
	int cols = shape[1];
	int length = rows * cols;
	float* f=new float[length];
	read_record(f, length, filename);
	Mat_<float> m = Mat_<float>(rows, cols,f);
	return m;
}


Mat_<float> get_bias(int shape, string filename)
{
	int length = shape;
	float* f = new float[length];
	read_record(f, length, filename);
	Mat_<float> m = Mat_<float>(1, shape,f);
	return m;
}

Mat_<float> sigmoid(Mat_<float>mat_mul)
{
	float a;
	Mat_<float> result = Mat_<float>(1, mat_mul.cols);
	for (int i = 0; i < mat_mul.cols; i++)
	{
		a = mat_mul(0, i);
		result(0, i) = (float)1 / ((float)1 + exp(-a));
	}
	return result;
}
Mat_<float> relu(Mat_<float>mat_mul)
{
	Mat_<float> result = Mat_<float>(1, mat_mul.cols);
	for (int i = 0; i < mat_mul.cols; i++)
	{
		if (mat_mul(0, i) > 0)
		{
			result(0, i) = mat_mul(0, i);
		}
		else
		{
			result(0, i) = 0;
		}
	}
	return result;
}

Mat_<float> forward(Mat_<float> x)
{
	int w1_shape[2] = { INPUT_NODE, LAYER1_NODE };
	int b1_shape = { LAYER1_NODE };
	Mat_<float > w1= get_weight(w1_shape, w1_record);
	Mat_<float > b1 = get_bias(b1_shape, b1_record);
	
	Mat_<float> y1 = relu(x*w1 + b1);

	int w2_shape[2] = { LAYER1_NODE,OUTPUT_NODE};
	int b2_shape = { OUTPUT_NODE };
	Mat_<float > w2 = get_weight(w2_shape, w2_record);
	Mat_<float > b2 = get_bias(b2_shape, b1_record);
	
	Mat_<float> y = y1*w2 + b2;
	return y;

}


int read_record(float * a, int length, string filename)
{
	ifstream fin(filename);
	int i = 0;
	while (!fin.eof() && i< length)
	{
		fin >> a[i];
		i++;
	}
	fin.close();
	return 0;
}
//int main()
//{
//	int w1_shape[2] = { INPUT_NODE, LAYER1_NODE };
//	int b1_shape = { LAYER1_NODE };
//	Mat_<float > w1 = get_weight(w1_shape, w1_record);
//	cout << w1;
//	system("pause");
//	return 0;
//}