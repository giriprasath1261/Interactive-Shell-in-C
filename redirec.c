#include<sys/wait.h>
#include<sys/utsname.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

int lookforredir(char *command)
{
	int flag=0;
	for(int i=0;i<strlen(command);i++)
	{
		if(command[i]=='<')
		{
			command[i]==' ';
			if(flag==2)
				flag=3;
			else
				flag=1;
		}
		if(command[i]=='>')
		{
			command[i]==' ';
			if(flag==1)
				flag=3;
			else
				flag=2;
		}
	}
	return flag;
}