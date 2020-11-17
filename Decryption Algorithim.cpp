#include <iostream>
#include <math.h>
#include <cmath>
using namespace std;

int check(double* m)
{
	if (abs(*m - *(m + 3)) < .00001 && abs(*(m + 1) - *(m + 4)) < .00001 && abs(*(m + 2) - *(m + 5)) < .00001)
	{
		cout << abs(*m - *(m + 3)) << " " << abs(*(m + 1) - *(m + 4)) << " " << abs(*(m + 2) - *(m + 5)) << endl;
		return 1;
	}

	return 0;
}


double* mandelbrot(double* pxl, double* m)
{
	int i = 0;
	int j = 0;

	while (i < 10 && j < 252)
	{
		*(m + 3) = *m * *m - *(m + 1) * *(m + 1) + *pxl;
		*(m + 4) = 2 * *m * *(m + 1) + *(pxl + 1);
		*(m + 5) = sqrt((*(m + 3) - *pxl + *(pxl + 2)) * (*(m + 3) - *pxl + *(pxl + 2)) + *(m + 4) * *(m + 4));

		cout << *m << " " << *(m + 1) << " " << *(m + 2) << " " << *(m + 3) << " " << *(m + 4) << " " << *(m + 5) << " " << endl;

		int c = check(m);
		cout << c << endl;
		if (c == 1)
			i++;
		else
			i = 0;

		for (int k = 0; k < 3; k++)
		{
			*(m + k) = *(m + k + 3);
		}

		j++;

		cout << j << endl << endl;
	}

	return m;
}

int main()
{
	int IntPart[3]  = { 34,112,118 };
	int FrctPart[3] = { 207,237,193 };
	double *IP;
	double *FP;
	IP = IntPart;
	FP = FrctPart;

	return 0;
}