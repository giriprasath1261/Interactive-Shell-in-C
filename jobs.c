#include<sys/wait.h>
#include<sys/utsname.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

int list[100];
int listsize;

int commandjobs(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount)
{
	int sno=1;
	for(int i=0;i<bgcount;i++)
	{
		char t[256];
		char propath[256]="/proc/";
		int q=bgpid[i];
		int j=0;
		while(q>0)
		{
			q=q/10;
			j++;
		}
		sprintf(t,"%d",bgpid[i]);
		t[j]='\0';
		strcat(propath,t);
		char stringy1[256];
		strcpy(stringy1,propath);
		strcat(stringy1,"/stat");
		FILE *fp = fopen(stringy1,"r");
		if(fp==NULL)
		{
			fprintf(stderr, "");
		}
		else
		{
			int pid2;
			char name[256];
			char stat[10];
			fscanf(fp,"%d %s %s",&pid2,name,stat);
			if(strcmp(stat,"R")==0)
			{
				printf("[%d] Running %s  [%d]\n",sno++,bgname[i],bgpid[i] );
			}
			if(strcmp(stat,"Z")==0)
			{
				printf("[%d] is zombie %s  [%d]\n",sno++,bgname[i],bgpid[i] );
			}
			if(strcmp(stat,"T")==0)
			{
				printf("[%d] stopped %s [%d]\n",sno++,bgname[i],bgpid[i] );
			}
			if(strcmp(stat,"S")==0)
			{
				printf("[%d] sleeping %s [%d]\n",sno++,bgname[i],bgpid[i] );
			}
		}
	}
	return 1;
}

int looper(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount)
{
	int sno=1;
	for(int i=0;i<bgcount;i++)
	{
		char t[256];
		char propath[256]="/proc/";
		int q=bgpid[i];
		int j=0;
		while(q>0)
		{
			q=q/10;
			j++;
		}
		sprintf(t,"%d",bgpid[i]);
		t[j]='\0';
		strcat(propath,t);
		char stringy1[256];
		strcpy(stringy1,propath);
		strcat(stringy1,"/stat");
		FILE *fp = fopen(stringy1,"r");
		if(fp==NULL)
		{
			fprintf(stderr, "");
		}
		else
		{
			int pid2;
			char name[256];
			char stat[10];
			fscanf(fp,"%d %s %s",&pid2,name,stat);
			list[sno++]=bgpid[i];
		}
	}
	listsize=sno;
	return 1;
}

int commandkjob(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount)
{
	looper(arguments,length,bgname,bgpid,bgflag,bgcount);
	// printf("%s %s\n",arguments[1],arguments[2]);
	int ind = atoi(arguments[1]);
	int arg1 = list[ind];
	kill(arg1,atoi(arguments[2]));
	return 1;
}

int commandoverkill(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount)
{
	looper(arguments,length,bgname,bgpid,bgflag,bgcount);
	for(int i=1;i<listsize;i++)
	{
		kill(list[i],9);
	}
}

int commandfg(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount)
{
	pid_t wait_pid;
	int check=0;
	int status;
	looper(arguments,length,bgname,bgpid,bgflag,bgcount);
	if(atoi(arguments[1])>=listsize)
		fprintf(stderr, "process not present\n" );
	else
	{
		do {
			wait_pid=waitpid(list[atoi(arguments[1])],&status,WUNTRACED);
		} while(wait_pid!=list[atoi(arguments[1])]);
		// int pid = list[atoi(arguments[1])];
		// printf("%d\n", pid);
		// signal(SIGTTOU,SIG_IGN);
		// setpgid(pid,0);
		// tcsetpgrp(0,pid);
		// tcsetpgrp(1,pid);
	}

	return 1;
}

int commandbg(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount)
{
	looper(arguments,length,bgname,bgpid,bgflag,bgcount);
	int check=0;
	if(atoi(arguments[1])>=listsize)
		fprintf(stderr, "process not present\n" );
	else
		{
			kill(list[atoi(arguments[1])],18);
		}
	return 1;
}
