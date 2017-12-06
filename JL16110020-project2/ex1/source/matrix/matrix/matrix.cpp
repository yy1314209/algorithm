// matrix.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<windows.h>

int number[31];
int m[31][31];
int s[31][31];

void GetNumbersFromFile(FILE* fp, int n)
{
	int i = 0, j = 0;
	char ch;
	ch = fgetc(fp);
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
		number[i] = 10 * number[i] + (ch - 48);;  //we get a right digit
		ch = fgetc(fp);
	}
}

void matrix_chain_order(int n)
{
	int i,j,k,l,q;
	for (i = 1; i <= n; i++)
	{
		m[i][i] = 0;
	}
	for (l = 2; l <= n; l++)
	{
		for (i = 1; i <= n - l + 1; i++)
		{
			j = i + l - 1;
			m[i][j] = INT_MAX;
			for (k = i; k <= j - 1; k++)
			{
				q = m[i][k] + m[k + 1][j] + number[i - 1] * number[k] * number[j];
				if (q < m[i][j])
				{
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
}

void print_optimal_parens(int i,int j,FILE *fp)
{
	char temp[5];
	if (i == j)
	{
		//printf("A%d", i);
		fputs("A", fp);
		sprintf(temp, "%d", i);
		fputs(temp,fp);
	}
	else
	{
		//printf("(");
		fputs("(", fp);
		print_optimal_parens(i,s[i][j],fp);
		print_optimal_parens(s[i][j] + 1, j,fp);
		//printf(")");
		fputs(")", fp);
	}
}

int main(int argc,char *argv[])
{
	int i;
	char split[50] = "-------------------------------------------------";
	char temp[10];
	FILE * fp = fopen("../../../input/input_numbers.txt","r");
	FILE * fp_result;  //we'll write the sorted strings into it
	FILE * fp_time;  //we'll write the time of calculating into it
	char path_result[50] = "../../../output/result.txt";
	char path_time[50] = "../../../output/time.txt";
	if (!fp) //input file doesn't exist
	{
		printf("the file doesn't exist\n");
		exit(1);
	}
	int n = atoi(argv[1]);
	GetNumbersFromFile(fp, n + 1);

	LARGE_INTEGER  timeStart;  //the start time
	LARGE_INTEGER  timeEnd;  //the end time
	LARGE_INTEGER  frequency; //the frequency of CPU
	QueryPerformanceFrequency(&frequency);
	double quadpart = (double)frequency.QuadPart;
    QueryPerformanceCounter(&timeStart);

	matrix_chain_order(n);
	fp_result = fopen(path_result, "a");
	fp_time = fopen(path_time, "a");
	fputs(split, fp_result);
	fputs("\r\n", fp_result);
	fputs("n=", fp_result);
	fputs(argv[1], fp_result);
	fputs(":", fp_result);
	fputs("\r\n", fp_result);
	for (i = 0; i <= n; i++)   //write the numbers into file
	{
		//printf("%d ", number[i]);
		sprintf(temp, "%d", number[i]);
		fputs(temp, fp_result);
		fputs(" ", fp_result);
	}
	fputs("\r\n", fp_result);
	//printf("\n");
	print_optimal_parens(1, n,fp_result);
	fputs("\r\n", fp_result);
	//printf("\n");

	QueryPerformanceCounter(&timeEnd);
	float elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;
	
	fputs(argv[1], fp_time);
	fputs(":", fp_time);
	sprintf(temp, "%f", elapsed);
	fputs(temp,fp_time);
	fputs("s\r\n",fp_time);

	fclose(fp); //close files
	fclose(fp_result);
	fclose(fp_time);
	getchar();
    return 0;
}

