#include <iostream>
#include <math.h>
#include <iomanip> 
#include <Windows.h>
using namespace std;
#define H 0.2 // Step size 
const int N = 250; // N was calculated by (50-0)/0.2
//---------------------------------------Functions------------------------------------------------//
long double predictor(long double e, long double d, long double c, long double b, long double a) //yp formula
{
	long double result = e + (H / 24) * (55 * d - 59 * c + 37 * b - 9 * a);
	return result;
}
long double corrector(long double k, long double j, long double i, long double h, long double g) // yc formula
{
	long double result = k + (H / 24) * (9 * j + 19 * i - 5 * h + g);
	return result;
}
long double ydash(long double x, long double y) // ode given
{
	long double result = 50 * (1 + cos(x)) - (0.5 * y);
	return result;
}
long double approxerror(long double pres, long double prev) //Approximate error formula
{
	long double result = abs((pres - prev) / pres) * 100;
	return result;
}
long double trueerror(long double right, long double approx)// True error formula
{
	long double result = abs((right - approx) / right) * 100;
	return result;
}
long double exactsol(long double x) // Given Exact Solution
{
	long double result = 1000 + 2.494 * cos(x) + 49.88 * sin(x) - 802.5 * exp(-0.05 * x);
	return result;
}
//---------------------------- Given Initial Values --------------------------//
//0	0.2	0.4	0.6
//200 218 235 251
// Start at x=0.8 up to x=50 with h =0.2
int main()
{
	long double exact = 0;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 15);  // Change color of console 
	cout << "Calcualting initial values" << endl;
	SetConsoleTextAttribute(h, 4);
	cout << "X\t" << " Y\t" << " " << "F\t\t" << "Approximate Error\t" << "Exact\t\t" << "True Error" << endl;
	// Setting initial values
	long double* x = new long double[N + 1];
	x[0] = 0;
	x[1] = 0.2;
	x[2] = 0.4;
	x[3] = 0.6;
	long double* y = new long double[N + 1];
	y[0] = 200.0;
	y[1] = 218.0;
	y[2] = 235.0;
	y[3] = 251.0;
	long double* f = new long double[N + 1];
	// Calcualting initial values using ode given and calculating approximate error for initial points
	for (int i = 0; i < 4; i++)
	{
		f[i] = ydash(x[i], y[i]);
	}
	double* e = new double[4];
	{
		e[0] = 0;
		e[1] = approxerror(y[1], y[0]);
		e[2] = approxerror(y[2], y[1]);
		e[3] = approxerror(y[3], y[2]);
	}
	for (int i = 0; i < 4; i++) // Prinitng results,exact solution,true error and approxiamte error
	{
		cout << x[i];
		cout << "\t ";
		cout << y[i];
		cout << "\t ";
		cout << f[i];
		if (f[i] == 0)
			cout << "\t";
		cout << "\t";
		exact = exactsol(x[i]);
		cout << setprecision(10) << e[i] << " %";
		cout << "\t\t";
		if (e[i] == 0)
			cout << "\t";
		cout << setprecision(10) << exact;
		cout << "    \t";
		cout << trueerror(exact, y[i]) << " %";
		cout << endl;
		cout << "-----------------------------------------------------------------------------------------" << endl;
	}
	//-----------------------Solution---------------------------//
	SetConsoleTextAttribute(h, 2);
	cout << "X\t" << " Y\t" << "\t  F\t\t" << "  Approximate Error\t\t" << "Exact\t\t" << "True Error" << endl;
	SetConsoleTextAttribute(h, 11);
	cout << "^^------------------------------------------------Solution--------------------------------------------^^" << endl;
	int i = 3; // at x=0.6
	for (int u = 0; u < N - 3; u++) // N - 3 because we calculated for the first 3 points 
	{
		x[i + 1] = x[i] + H;
		cout << setprecision(10) << x[i + 1]; // Setprecision to print known 10 digits
		y[i + 1] = predictor(y[i], f[i], f[i - 1], f[i - 2], f[i - 3]);
		cout << "\t" << setprecision(10) << y[i + 1];
		long double yp = y[i + 1];
		f[i + 1] = ydash(x[i + 1], y[i + 1]);
		cout << "\t" << setprecision(10) << f[i + 1];
		cout << "\t" << approxerror(y[i + 1], y[i]) << " %";
		cout << endl;
		y[i + 1] = corrector(y[i], f[i + 1], f[i], f[i - 1], f[i - 2]);
		cout << "\t" << setprecision(10) << y[i + 1];
		cout << "\t";
		long double yc = y[i + 1];
		f[i + 1] = ydash(x[i + 1], y[i + 1]);
		cout << setprecision(10) << f[i + 1];
		cout << "\t" << approxerror(yc, yp) << " %" << endl;
		for (int j = 0; j < 4; j++)
		{
			long double prev = y[i + 1];
			y[i + 1] = corrector(y[i], f[i + 1], f[i], f[i - 1], f[i - 2]);
			cout << "\t" << setprecision(10) << y[i + 1];
			f[i + 1] = ydash(x[i + 1], y[i + 1]);
			cout << "\t" << setprecision(10) << f[i + 1];
			cout << "\t" << approxerror(y[i + 1], prev) << " %" << endl;
		}
		long double ex = exactsol(x[i + 1]);
		cout << "\t\t\t\t\t\t\t\t    " << ex;
		long double trueer = trueerror(ex, y[i + 1]);
		cout << "\t\t" << trueer << " %";
		cout << endl;
		i++;
		cout << "---------------------------------------------------------------------------------------------------------" << endl;
	}
	SetConsoleTextAttribute(h, 0);
}

