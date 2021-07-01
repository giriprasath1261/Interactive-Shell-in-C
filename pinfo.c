#include<sys/wait.h>
#include<sys/utsname.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include <getopt.h>
#include <time.h>
#include <fcntl.h>
int commandpinfo(char **arguments,int flagin,int length,int k)
{	
	int pid;
	char *hi;
	char t[256];
	if(length==1)
	{
		pid=getpid();
		int q=pid;
		int j=0;
		while(q>0)
		{
			q=q/10;
			j++;
		}
		sprintf(t,"%d",pid);
		t[j]='\0';
	}
	else
	{
		printf("elsecase\n");
		hi=arguments[1];
		pid = atoi(hi);
	}



	// printf("out\n");
	char propath[256]="/proc/";
	if(length==1)
		strcat(propath,t);
	else
		strcat(propath,hi);

//status
	char stringy1[256];
	strcpy(stringy1,propath);
	strcat(stringy1,"/stat");
	FILE *fp = fopen(stringy1,"r");
	int pid2;
	char name[256];
	char stat[256];
	if(fp==NULL)
	{
		fprintf(stderr, "no process with given pid\n");
		return 1;
	}
	fscanf(fp,"%d %s %s",&pid2,name,stat);

//memory
	char stringy2[256];
	strcpy(stringy2,propath);
	strcat(stringy2,"/statm");
	fp=fopen(stringy2,"r");
	int fil;
	if(fp==NULL)
	{
		fprintf(stderr, "no process with given pid\n");
		return 1;
	}
	fscanf(fp,"%d",&fil);

//exe
	char stringy3[1024],finder[1024];
	strcpy(stringy3,propath);
	strcat(stringy3,"/exe");
	if(fp==NULL)
	{
		fprintf(stderr, "no process with given pid\n");
		return 1;
	}
	int str_size = readlink(stringy3,finder,sizeof(finder));
	finder[str_size]='\0';

//printing
	if(flagin==2||flagin==3)
		freopen(arguments[length], "a+", stdout);
	printf("pid -- %d\n", pid);
	printf("Process Status -- %s\n", stat);
	printf("Memory -- %d\n", fil);
	freopen("/dev/tty", "w", stdout);

//absolute to relative:
	char home[256];
	char currdir[256];
	getcwd(currdir,sizeof(currdir));
	strcpy(home,currdir);
	int cancel = strlen(home);
	int flag=0;
	// currdir=finder;
	int total = strlen(finder);
	if(total<cancel)
	{
		flag=1;
	}
	else
	{
		for(int i=0;i<cancel;++i)
		{

			if(home[i]!=finder[i])
			{
				flag=1;
				break;
			}
		}
	}
	if(flagin==2||flagin==3)
		freopen(arguments[length], "a+", stdout);
	if(flag==0)
	{
		char path[256];
		path[0]='~';
		int i;
		for(i=1;i<=total-cancel;i++)
			path[i]=finder[cancel+i-1];
		path[i]='\0';
		printf("Executable Path -- %s\n",path );
	}
	else
	{
		printf("Executable Path -- %s\n",finder );
	}
	freopen("/dev/tty", "w", stdout);
	return 1;	
}