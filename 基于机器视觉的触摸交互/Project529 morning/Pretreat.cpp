#include"Pretreat.h"

float pi = 3.1415;
float lenghth_Max = 350;


vector<float> pretreat(vector<Point> point_vect)
{
	int cout = point_vect.size();
	float direct = 0;
	float length = 0;
	vector<float> direct_vect;
	vector<float> length_vect;
	for (int i = 0; i < cout; i++)
	{
		for (int j = i + 1; j < cout; j++)
		{
			if ((point_vect[j].x - point_vect[i].x) == 0)
			{
				if ((point_vect[j].y - point_vect[i].y) > 0)
					direct = 1;
				else
					direct = 0;
			}
			else
			{
				float a = (point_vect[j].y - point_vect[i].y) / (point_vect[j].x - point_vect[i].x);
				direct = (pi/2+ atan(a))/pi;
			}

			direct_vect.push_back(direct);
			length = sqrt((point_vect[j].y - point_vect[i].y)*(point_vect[j].y - point_vect[i].y) + (point_vect[j].x - point_vect[i].x)*(point_vect[j].x - point_vect[i].x));
			length_vect.push_back(length);

		}
		break;
	}
	int cout_length = length_vect.size();
	for (int k = 0; k < cout_length; k++)
	{
		if (length_vect[k] > lenghth_Max)
			direct_vect.push_back(1);
		else
			direct_vect.push_back(length_vect[k] / lenghth_Max);
	}
	return direct_vect;
}