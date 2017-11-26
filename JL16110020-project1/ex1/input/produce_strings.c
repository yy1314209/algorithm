#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<string.h>

int main()
{
	int i,n,l;
	int total =  131072; //2**17
	int t = 0;
	char temp[8];
	char str[33];  //the length of strings is 1 to 32,one remain '\0'
	FILE *fp = fopen("input_strings.txt","w");
	/*if(fp == NULL)
	{
		printf("the file doesn't exist\n");
		exit(1);
	}*/
	srand( (unsigned)time( NULL ) );  //initialize the srand
	while(total--)
	{
		t++;
		sprintf(temp,"%d",t);
		l = strlen(temp);
		temp[l++] = '.';
		temp[l] = '\0';
		fputs(temp,fp);
		n = (rand() % 32) + 1;
		//printf("%d\n",n);
		for(i = 0 ; i < n ; i++)
		{
			str[i] = (rand() % 26) + 97;  //the  ascll values of a~z are 97 to 122
		}
		str[n] = '\0';
		//printf("%s\n",str);
		fputs(str,fp); //write str into fp
		//printf("%ld\n",ftell(fp));
		fputs("\r\n",fp);  //windows: '\r\n' to change line
	}
	//printf("%ld\n",ftell(fp));
	fclose(fp);
	return 0;
}
	