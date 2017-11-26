#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
//#include<sys/time.h>

struct String{
	char str[33];
	int len;
} string[131073];  //this time,the first efficient element is string[1],so add 1

void GetStringsFromFile(FILE* fp,int n)
{
	int i = 1,j = 0;
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
			if(i == n + 1)
				break;
			j = 0;
			continue;
		}
		string[i].str[j++] = ch;  //we get a right char
		ch = fgetc(fp);
	}
}

int CompareString1(int j,int i)
{
	if(string[j].len > string[i].len)  //we should compare the length first
		return 1;
	else if(string[j].len < string[i].len)
		return -1;
	if(strcmp(string[j].str,string[i].str) > 0)
		return 1;
	else if(strcmp(string[j].str,string[i].str) < 0)
		return -1;
	return 0;
}

void Max_Heapify(int n,int i)
{
	char temp[33]; //be used to exchange two strings
	int temp_len;  //be used to exchange two integers
	int l = 2*i;  //left child
	int r = 2*i + 1;  //right child
	int largest;
	if((l <= n) && (CompareString1(l,i) == 1))
		largest = l;
	else largest = i;
	if((r <= n) && (CompareString1(r,largest) == 1))
		largest = r;
	if(largest != i)
	{
		strcpy(temp,string[i].str);
		strcpy(string[i].str,string[largest].str);
		strcpy(string[largest].str,temp);
		temp_len = string[i].len;
		string[i].len = string[largest].len;
		string[largest].len = temp_len;
		Max_Heapify(n,largest);
	}
}


void Build_Max_Heap(int n)
{
	int i;
	for(i = n/2; i >= 1 ; i--)
	{
		Max_Heapify(n,i);
	}
}

void Heap_Sort(int n)
{
	int i;
	char temp[33]; //be used to exchange two strings
	int temp_len;  //be used to exchange two integers
	int heap_size = n;
	Build_Max_Heap(heap_size);
	for(i = n; i >=2; i--)
	{
		strcpy(temp,string[1].str);
		strcpy(string[1].str,string[i].str);
		strcpy(string[i].str,temp);
		temp_len = string[1].len;
		string[1].len = string[i].len;
		string[i].len = temp_len;
		heap_size--;
		Max_Heapify(heap_size,1);
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
	char path[50] = "../../output/heap_sort/result_";
	char path_time[50] = "../../output/heap_sort/time.txt";
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

	Heap_Sort(n);

	QueryPerformanceCounter(&timeEnd);
	float elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;
	/*gettimeofday(&tEnd,NULL);  //the end time
	timeElapse = tEnd.tv_usec - tStart.tv_usec + 1000000*(tEnd.tv_sec - tStart.tv_sec);
	*/
	/*for(i = 1 ; i <= n ; i++)
		printf("%s %d\n",string[i].str,string[i].len);*/
	strcat(path,argv[1]);
	strcat(path,".txt");
	printf("%s\n",path);
	fp_out = fopen(path,"w");
	for(i = 1 ;i <= n ; i++)  //write sorted strings into file
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
