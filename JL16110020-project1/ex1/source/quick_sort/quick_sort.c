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

int CompareString1(char *str1,char *str2)
{
	int n1,n2;
	n1 = strlen(str1);
	n2 = strlen(str2);
	if(n1 > n2)  //we should compare the length first
		return 1;
	else if(n1 < n2)
		return -1;
	if(strcmp(str1,str2) > 0)
		return 1;
	else if(strcmp(str1,str2) < 0)
		return -1;
	return 0;
}

int Partition(int p,int r)
{
	int i,j;
	struct String x = string[r];
	struct String temp;
	i = p - 1;
	for(j = p; j < r; j++)
	{
		if(CompareString1(string[j].str,x.str) != 1)
		{
			i++;
			temp = string[i];
			string[i] = string[j];
			string[j] = temp;
		}
	}
	temp = string[i+1];
	string[i+1] = string[r];
	string[r] = temp;
	return i+1;
}

void QuickSort(int p,int r)
{
	int q;
	if(p < r)
	{
		q = Partition(p,r);
		QuickSort(p,q-1);
		QuickSort(q+1,r);
	}
}

int main(int argc,char *argv[])
{
	int i,j,t = 0;
	char temp[33];
	int temp_len;
	FILE* fp = fopen("../../input/input_strings.txt","r");
	FILE* fp_out;  //we'll write the sorted strings into it
	FILE* fp_time;  //we'll write the time of calculating into it
	char path[50] = "../../output/quick_sort/result_";
	char path_time[50] = "../../output/quick_sort/time.txt";
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
	
	QuickSort(0,n-1);
	
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
