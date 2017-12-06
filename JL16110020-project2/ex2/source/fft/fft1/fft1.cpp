// fft.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <complex>  //模板类
#include <cmath>
#include <windows.h>
#include <math.h>
using namespace std;

const double pi = acos(-1.0);
typedef complex<double> Complex;

Complex a[150], b[150];

void GetNumbersFromFile(FILE* fp, int n, int number[])
{
	int i = 0, j = 0;
	char ch;
	ch = fgetc(fp);
	/*if (flag == FIRST)
	i = 0;
	else
	i = n - 1;*/
	bool f = 1; //if f(false) == 1,the number is line number,ignore it
	while (ch != EOF)
	{
		//printf("%c\n",ch);
		if (ch >= '0' && ch <= '9' && (f == 1))  //in my data file,there is a line num brfore every string,so ignore them
		{
			ch = fgetc(fp);
			continue;
		}
		else if (ch == '.')
		{
			f = 0;  //we find .,thus the next digit is right digit
			ch = fgetc(fp);
			continue;
		}
		else if (ch == '\r')
		{
			ch = fgetc(fp);
			continue;
		}
		else if (ch == '\n')  //now we should get the next string,initial the next
		{
			ch = fgetc(fp);
			f = 1;  //the next digit is line number
			i++;
			if (i == n)
				break;
			continue;
		}
		number[i] = 10 * number[i] + (ch - 48);  //we get a right digit
		ch = fgetc(fp);
	}
}

void FFT(Complex *a, int n, int t)
{
	int i;
	if (n == 1) return;
	Complex a0[150], a1[150];
	for (i = 0; i < n; i += 2)
	{
		a0[i >> 1] = a[i];
		a1[i >> 1] = a[i + 1];
	}
	FFT(a0, n >> 1, t);
	FFT(a1, n >> 1, t);
	Complex w(cos(2.0*pi / n), sin(t*2.0*pi / n)), wn(1, 0);
	for (i = 0; i < (n >> 1); i++,wn *= w)
	{
		a[i] = a0[i] + wn*a1[i];
		a[i + (n >> 1)] = a0[i] - wn*a1[i];
	}
}

void DFT(Complex *a, int n)
{
	FFT(a, n, 1);
}

void IDFT(Complex *a, int n)
{
	int i;
	FFT(a, n, -1);
	for (i = 0; i < n; i++)
	{
		a[i] = Complex(a[i].real() / n, a[i].imag());
	}
}

int main(int argc, char *argv[])
{
	int A[64];
	int B[64];
	int C[128];
	memset(A, 0, sizeof(int) * 64);
	memset(B, 0, sizeof(int) * 64);
	memset(C, 0, sizeof(int) * 128);
	int i;
	int x;
	char split[50] = "-------------------------------------------------";
	char temp[20];
	FILE * fp = fopen("../../../input/input_numbers.txt", "r");
	FILE * fp_result;
	FILE * fp_time;
	char path_result[50] = "../../../output/result.txt";
	char path_time[50] = "../../../output/time.txt";
	if (!fp) //input file doesn't exist
	{
		printf("the file doesn't exist\n");
		exit(1);
	}
	int n = atoi(argv[1]);
	GetNumbersFromFile(fp, n, A);
	GetNumbersFromFile(fp, n, B);

	fp_result = fopen(path_result, "a");
	fp_time = fopen(path_time, "a");
	if (n == 4)
	{
		fputs("FFT:\r\n", fp_result);
	}
	fputs(split, fp_result);
	fputs("\r\n", fp_result);
	fputs("n=", fp_result);
	fputs(argv[1], fp_result);
	fputs(":", fp_result);
	fputs("\r\n", fp_result);

	fputs("A=", fp_result);
	for (i = 0; i < n; i++)
	{
		printf("%d ", A[i]);
		sprintf(temp, "%d", A[i]);
		fputs(temp, fp_result);
		fputs(" ", fp_result);
	}
	fputs("\r\n", fp_result);
	printf("\n");
	fputs("B=", fp_result);
	for (i = 0; i < n; i++)
	{
		printf("%d ", B[i]);
		sprintf(temp, "%d", B[i]);
		fputs(temp, fp_result);
		fputs(" ", fp_result);
	}
	fputs("\r\n", fp_result);
	printf("\n");

	for (i = 0; i < n; i++)
	{
		a[i] = Complex(A[i], 0);
	}
	for (i = 0; i < n; i++)
	{
		b[i] = Complex(B[i], 0);
	}
	int t;
	for (t = 1; t <= 2 * (n-1); t <<= 1);

	LARGE_INTEGER  timeStart;  //the start time
	LARGE_INTEGER  timeEnd;  //the end time
	LARGE_INTEGER  frequency; //the frequency of CPU
	QueryPerformanceFrequency(&frequency);
	double quadpart = (double)frequency.QuadPart;
	QueryPerformanceCounter(&timeStart);

	DFT(a, t);  //点值
	DFT(b, t);
	for (i = 0; i <= t; i++)
	{
		a[i] = a[i] * b[i];  //计算
	}
	IDFT(a, t);
	fputs("C=", fp_result);
	for (i = 0; i < 2 * n - 1; i++)
	{
		x = a[i].real() + 0.5;
		//printf("%d ", x);
		sprintf(temp, "%d", x);
		fputs(temp, fp_result);
		fputs(" ", fp_result);
	}
	fputs("\r\n", fp_result);
	//printf("\n");
	QueryPerformanceCounter(&timeEnd);
	float elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;


	if (n == 4)
	{
		fputs("FFT:\r\n", fp_time);
	}
	fputs(argv[1], fp_time);
	fputs(":", fp_time);
	sprintf(temp, "%f", elapsed);
	fputs(temp, fp_time);
	fputs("s\r\n", fp_time);

	fclose(fp_time);  //close files
	fclose(fp);
	fclose(fp_result);
	getchar();
	return 0;
}



