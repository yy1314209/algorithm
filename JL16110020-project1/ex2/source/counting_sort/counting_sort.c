#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
//#include<sys/time.h>

int number[131072];
int sorted_number[131072];

void GetNumbersFromFile(FILE* fp,int n)
{
	int i = 0,j = 0;
	char ch;
	ch = fgetc(fp);
	boolean f = 1; //if f(false) == 1,the number is line number,ignore it
	while(ch != EOF)
	{
	    //printf("%c\n",ch);
		if(ch >= '0' && ch <= '9' && (f == 1))  //in my data file,there is a line num brfore every string,so ignore them
		{
			ch = fgetc(fp);
			continue;
		}
		else if(ch == '.')
		{
			f = 0;  //we find .,thus the next digit is right digit
			ch = fgetc(fp);
			continue;
		}
		else if(ch == '\r')
		{
			ch = fgetc(fp);
			continue;
		}
		else if(ch == '\n')  //now we should get the next string,initial the next
		{
			ch = fgetc(fp);
			f = 1;  //the next digit is line number
			i++;
			if(i == n)
				break;
			continue;
		}
		number[i] = 10*number[i] + (ch - 48);;  //we get a right digit
		ch = fgetc(fp);
	}
}

void CountingSort(int n)
{
	int i,j;
	int c[65536];  //0~65535
	for(i = 0; i < 65536 ; i++)
		c[i] = 0;
	for(j = 0; j < n ; j++)  //now c[i] contains the number of elements equal to i
		c[number[j]]++;
	for(i = 1; i < 65536 ; i++)
		c[i] += c[i-1];
	for(j = n-1; j>=0 ; j--)
	{
		sorted_number[c[number[j]] - 1] = number[j];
		c[number[j]]--;
	}
}


int main(int argc,char *argv[])
{
	int i,j,t = 0;
	char temp[13];
	int temp_len;
	FILE* fp = fopen("../../input/input_numbers.txt","r");
	FILE* fp_out;  //we'll write the sorted strings into it
	FILE* fp_time;  //we'll write the time of calculating into it
	char path[50] = "../../output/counting_sort/result_";
	char path_time[50] = "../../output/counting_sort/time.txt";
	if(fp == NULL)
	{
		printf("the file doesn't exist");
		exit(1);
	}
	int n = atoi(argv[1]);  //convert the number from a to i
	GetNumbersFromFile(fp,n);  //get strings from file
	/*for(i = 0 ; i < n ; i++)
		printf("%s %d\n",string[i].str,string[i].len);
	printf("*********************************\n");*/
	/*struct timeval tStart,tEnd;  //in order to get the executive time
	float timeElapse;
	gettimeofday(&tStart,NULL);  //the start time
	*/

	LARGE_INTEGER  timeStart;  //the start time
	LARGE_INTEGER  timeEnd;  //the end time
	LARGE_INTEGER  frequency; //the frequency of CPU
	QueryPerformanceFrequency(&frequency);
	double quadpart = (double)frequency.QuadPart;

	QueryPerformanceCounter(&timeStart);

	CountingSort(n);

	QueryPerformanceCounter(&timeEnd);
	float elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;
	/*gettimeofday(&tEnd,NULL);  //the end time
	timeElapse = tEnd.tv_usec - tStart.tv_usec + 1000000*(tEnd.tv_sec - tStart.tv_sec);
	*/
	/*for(i = 0 ; i < n ; i++)
		printf("%s %d\n",string[i].str,string[i].len);*/
	strcat(path,argv[1]);
	strcat(path,".txt");
	printf("%s\n",path);
	fp_out = fopen(path,"w");
	for(i = 0 ;i < n ; i++)   //write sorted numbers into file
	{
		t++;
		sprintf(temp,"%d",t);
		temp_len = strlen(temp);
		temp[temp_len++] = '.';
		temp[temp_len] = '\0';
		fputs(temp,fp_out);
		sprintf(temp,"%d",sorted_number[i]);
		fputs(temp,fp_out);
		fputs("\r\n",fp_out);
	}
	fclose(fp);
	fclose(fp_out);

	printf("%fs\n",elapsed);
	//getchar();
	fp_time = fopen(path_time,"a");
	fputs(argv[1],fp_time);
	fputs(" numbers:",fp_time);
	sprintf(temp,"%f",elapsed);
	fputs(temp,fp_time);
	fputs("s\r\n",fp_time);
	fclose(fp_time);
	return 0;
}
