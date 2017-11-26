#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
//#include<sys/time.h>

struct String{
	char str[33];
	int len;
} string[131072];

void GetStringsFromFile(FILE* fp,int n)
{
	int i = 0,j = 0;
	char ch;
	ch = fgetc(fp);
	while(ch != EOF)
	{
	    //printf("%c\n",ch);
		if(ch >= '0' && ch <= '9')  //in my data file,there is a line num brfore every string,so ignore them
		{
			ch = fgetc(fp);
			continue;
		}
		else if(ch == '.')
		{
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
			string[i].str[j] = '\0';
			string[i].len = j;
			i++;
			if(i == n)
				break;
			j = 0;
			continue;
		}
		string[i].str[j++] = ch;  //we get a right char
		ch = fgetc(fp);
	}
}

int CompareString1(char *str,int i)
{
	int n;
	n = strlen(str);
	if(n > string[i].len)  //we should compare the length first
		return 1;
	else if(n < string[i].len)
		return -1;
	if(strcmp(str,string[i].str) > 0)
		return 1;
	else if(strcmp(str,string[i].str) < 0)
		return -1;
	return 0;
}


int main(int argc,char *argv[])
{
	int i,j,t = 0;
	char temp[33];
	int temp_len;
	FILE* fp = fopen("../../input/input_strings.txt","r");
	FILE* fp_out;  //we'll write the sorted strings into it
	FILE* fp_time;  //we'll write the time of calculating into it
	char path[50] = "../../output/insertion_sort/result_";
	char path_time[50] = "../../output/insertion_sort/time.txt";
	if(fp == NULL)
	{
		printf("the file doesn't exist");
		exit(1);
	}
	int n = atoi(argv[1]);  //convert the number from a to i
	GetStringsFromFile(fp,n);  //get strings from file
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
	for(i = 1; i < n; i++)
	{
		strcpy(temp,string[i].str);
		temp_len = string[i].len;
		j = i - 1;
		while(CompareString1(temp,j) == -1)   //string temp < string sting[j].str
		{
			stpcpy(string[j+1].str,string[j].str);
			string[j+1].len = string[j].len;
			j--;
		}
		strcpy(string[j+1].str,temp);
		string[j+1].len = temp_len;
	}
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
	for(i = 0 ;i < n ; i++)   //write sorted strings into file
	{
		t++;
		sprintf(temp,"%d",t);
		temp_len = strlen(temp);
		temp[temp_len++] = '.';
		temp[temp_len] = '\0';
		fputs(temp,fp_out);
		fputs(string[i].str,fp_out);
		fputs("\r\n",fp_out);
	}
	fclose(fp);
	fclose(fp_out);

	printf("%fs\n",elapsed);
	fp_time = fopen(path_time,"a");
	fputs(argv[1],fp_time);
	fputs(" numbers:",fp_time);
	sprintf(temp,"%f",elapsed);
	fputs(temp,fp_time);
	fputs("s\r\n",fp_time);
	fclose(fp_time);
	return 0;
}
