// fft.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <windows.h>



void GetNumbersFromFile(FILE* fp, int n,int number[])
{
	int i=0, j = 0;
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

void MUL(int a[], int b[], int c[],int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			c[i + j] += a[i] * b[j];
		}
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
	GetNumbersFromFile(fp, n,A);
	GetNumbersFromFile(fp, n,B);

	fp_result = fopen(path_result, "a");
	fp_time = fopen(path_time, "a");
	if (n == 4)
	{
		fputs("MUL:\r\n", fp_result);
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

	LARGE_INTEGER  timeStart;  //the start time
	LARGE_INTEGER  timeEnd;  //the end time
	LARGE_INTEGER  frequency; //the frequency of CPU
	QueryPerformanceFrequency(&frequency);
	double quadpart = (double)frequency.QuadPart;
	QueryPerformanceCounter(&timeStart);

	MUL(A, B, C, n);

	QueryPerformanceCounter(&timeEnd);
	float elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;

	fputs("C=", fp_result);
	for (i = 0; i < 2*n-1; i++)
	{
		printf("%d ", C[i]);
		sprintf(temp, "%d", C[i]);
		fputs(temp, fp_result);
		fputs(" ", fp_result);
	}
	fputs("\r\n", fp_result);
	printf("\n");

	if (n == 4)
	{
		fputs("MUL:\r\n", fp_time);
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



