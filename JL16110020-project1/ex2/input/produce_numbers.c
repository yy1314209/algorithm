#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int main()
{
	int i,n,l;
	int total =  131072; //2**17
	int t = 0;
	char temp[7];
	int num1,num2; //in order to get a num between 1~65535,do [0~32767] + [0~32767] + 1
	int  num; //1~65535
	FILE *fp = fopen("input_numbers.txt","w");
	/*if(fp == NULL)
	{
		printf("the file doesn't exist\n");
		exit(1);
	}*/
	srand( (unsigned)time( NULL ) );  //initialize the srand
	while(total--)
	{
		t++;
		sprintf(temp,"%d",t);  //line number + .
		l = strlen(temp);
		temp[l++] = '.';
		temp[l] = '\0';
		fputs(temp,fp);
		num1 = rand();
		num2 = rand();
		num = num1 + num2 + 1;
		sprintf(temp,"%d",num);
		//printf("%s\n",str);
		fputs(temp,fp); //write str into fp
		//printf("%ld\n",ftell(fp));
		fputs("\r\n",fp);  //windows: '\r\n' to change line
	}
	//printf("%ld\n",ftell(fp));
	fclose(fp);
	return 0;
}
