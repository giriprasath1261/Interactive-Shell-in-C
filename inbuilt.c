#include<sys/wait.h>
#include<sys/utsname.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

int commandcd(char **arguments,int flag,int length,int k)
{
	if(arguments[1]==NULL)
		fprintf(stderr, "shellmax: no argument given\n" );
	else
	{
		if(arguments[2]!=NULL)
			fprintf(stderr, "shellmax: too many arguments for \"cd\"\n");
		else
		{
			char currdir[256];
			getcwd(currdir,sizeof(currdir));
			char home1[256];
			strcpy(home1,currdir);
			char temp[256]="/";
			strcat(temp,arguments[1]);
			strcat(home1,temp);
			if(chdir(home1)!=0)
			{
				perror("shellmax");
			}
		}
	}
	return 1;
}

int commandpwd(char **arguments,int flag,int length,int k)
{
	if(flag==2||flag==3)
		freopen(arguments[length], "a+", stdout);
	char currdir[256];
	getcwd(currdir,sizeof(currdir));
	printf("%s\n", currdir);
	freopen("/dev/tty", "w", stdout);
	return 1;
}

int commandecho(char **arguments,int flag,int length,int k)
{
	int limit = length;
	if(flag%10 ==2||flag%10 ==3)
	{	
		freopen(arguments[length], "a+", stdout);
	}
	int i=1;
	while(i<limit)
	{
		printf("%s ", arguments[i++]);
	}
	printf("\n");
	freopen("/dev/tty", "w", stdout);
	return 1;
}

int commandexit(char **arguments,int flag,int length,int k)
{
	if(arguments[1]!=0)
		fprintf(stderr, "shellmax: too many arguments\n" );
	else
	{
		return 0;
	}
}

int commandsetenv(char **arguments,int flag,int length,int k)
{
	if(arguments[3]!=NULL)
	{
		fprintf(stderr, "shellmax:too many arguments\n" );
		return 1;
	}
	if(arguments[1]==NULL)
	{
		fprintf(stderr, "shellmax:no variable given\n" );
		return 1;
	}
	if(arguments[2]==NULL)
	{
		setenv(arguments[1]," ",1);
	}
	else
	{
		setenv(arguments[1],arguments[2],1);
	}
	return 1;
}

int commandunsetenv(char **arguments,int flag,int length,int k)
{
	if(arguments[2]!=NULL)
	{
		fprintf(stderr, "shellmax:too many arguments\n" );
		return 1;
	}
	if(arguments[1]==NULL)
	{
		fprintf(stderr, "shellmax: no variable given\n" );
		return 1;
	}
	unsetenv(arguments[1]);
	return 1;
}

int inf()
{
	while(1)
	{
		printf("0\n");
	}
}
