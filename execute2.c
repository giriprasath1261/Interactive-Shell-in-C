#include<sys/wait.h>
#include<sys/utsname.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include "ls.c"
#include "inbuilt.c"
#include "pinfo.c"
#include "jobs.c"
#define builtinnum 13

int commandcd(char **arguments,int flag,int length,int k);
int commandpwd(char **arguments,int flag,int length,int k);
int commandecho(char **arguments,int flag,int length,int k);
int commandexit(char **arguments,int flag,int length,int k);
int commandsetenv(char **arguments,int flag,int length,int k);
int commandunsetenv(char **arguments,int flag,int length,int k);
int commandjobs(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount);
int commandkjob(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount);
int commandoverkill(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount);
int commandfg(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount);
int commandbg(char **arguments,int length,char bgname[100][100],int bgpid[100],int bgflag[100],int bgcount);

char bgname[100][100];
char bgarg[100][100];
int bgpid[100];
int bgflag[100];
int bgcount=0;
int ctrl =0;


void controlz()
{
	ctrl = 1;
}

char *builtin[]=
{
	"cd","pwd","echo","quit","ls","pinfo","setenv","unsetenv","jobs","kjob","overkill","fg","bg"
};

int builtinfunc(int i,char **arguments,int flag,int length,int k)
{
	int ret;
	switch(i)
	{
		case 0: ret=commandcd(arguments,flag,length,k);
		break;
		case 1: ret=commandpwd(arguments,flag,length,k);
		break;
		case 2: ret=commandecho(arguments,flag,length,k);
		break;
		case 3: ret=commandexit(arguments,flag,length,k);
		break;
		case 4: ret=commandls(arguments,flag,length,k);
		break;
		case 5: ret=commandpinfo(arguments,flag,length,k);
		break;
		case 6: ret=commandsetenv(arguments,flag,length,k);
		break;
		case 7: ret=commandunsetenv(arguments,flag,length,k);
		break;
		case 8: ret=commandjobs(arguments,length,bgname,bgpid,bgflag,bgcount);
		break;
		case 9: ret=commandkjob(arguments,length,bgname,bgpid,bgflag,bgcount);
		break;
		case 10: ret=commandoverkill(arguments,length,bgname,bgpid,bgflag,bgcount);
		break;
		case 11: ret=commandfg(arguments,length,bgname,bgpid,bgflag,bgcount);
		break;
		case 12: ret=commandbg(arguments,length,bgname,bgpid,bgflag,bgcount);
		break;
	}
	return ret;
}

int normalfunc(char **arguments,int flag,int length,int k,int thepid)
{
	char** altarguments = malloc(128 * sizeof(char*));
	int i =0;
	while(i<=length)
	{
		char* tm = arguments[i];
		altarguments[i] = tm;
		i++;
	}
	pid_t pid,wait_pid;
	int bg=0;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		int i=0;
		while(arguments[i]!=NULL)
		{
			if(strcmp(arguments[i],"&")==0)
			{
				bg=1;
				arguments[i]=NULL;
				break;
			}
			i++;
		}

		if(bg==1)
		{
			setpgid(pid,pid);
			pid_t pid2 = fork();
			if(pid2==0)
			{
				if(flag<10)
				{
					if(flag==4||flag==5)
					{
						freopen(arguments[length-1], "a+", stdout);
						altarguments[length-1]=NULL;
					}
					if(flag==2||flag==3)
					{
						freopen(arguments[length-1], "w", stdout);
						altarguments[length-1]=NULL;
					}
					if(flag==1)
						freopen(arguments[length-1], "r", stdin);
					if(flag==3 || flag==5)
						freopen(arguments[length-2],"r",stdin);
				}
				else
				{
					if(flag%10==1)
					{
						freopen(arguments[length-1], "r", stdin);
					}
					freopen("tempout.txt","w",stdout);
				}
				if(k>0)
				{
					freopen("tempin.txt","r",stdin);
				}
				int retval = execvp(arguments[0],altarguments);
				freopen("/dev/tty", "w", stdout);
				freopen("/dev/tty", "r", stdin);
				if(retval<0)
				{
					fprintf(stderr,"cannot proceed command not found\n" );
					return 0;
				}
				return 0;
			}
			waitpid(pid,&status,0);
			printf("%s:[%d] exited normally\n",arguments[0],pid2);
			for(int i=0;i<bgcount;i++)
			{
				if(bgpid[i]==pid2)
					bgflag[i]=1;
			}
			return 0;
		}
		else
		{
			if(flag<10)
			{
				if(flag==4||flag==5)
				{
					freopen(arguments[length-1], "a+", stdout);
					altarguments[length-1]=NULL;
				}
				if(flag==2||flag==3)
				{
					freopen(arguments[length-1], "w", stdout);
					altarguments[length-1]=NULL;
				}
				if(flag==1)
					freopen(arguments[length-1], "r", stdin);
				if(flag==3 || flag==5)
					freopen(arguments[length-2],"r",stdin);
			}
			else
			{
				if(flag%10==1)
					freopen(arguments[length-1], "r", stdin);
				freopen("tempout.txt","w",stdout);
			}
			if(k>0)
			{
				freopen("tempin.txt","r",stdin);
			}
			int retval = execvp(arguments[0],altarguments);
			freopen("/dev/tty", "w", stdout);
			freopen("/dev/tty", "r", stdin);
			if(retval<0)
			{
				fprintf(stderr, "cannot proceed command not found\n" );
				return 0;
			}
		}
	}
	else
	{
		signal(SIGTSTP,controlz);
		// setpgid(pid,pid);
		int i=0;
		while(arguments[i]!=NULL)
		{
			if(strcmp(arguments[i],"&")==0)
			{
				bg=1;
				printf("[%d]\n",pid+1);
				arguments[i]=NULL;
				strcpy(bgname[bgcount],arguments[0]);
				// strcpy(bgarg[bgcount],arguments[1]);
				bgpid[bgcount]=pid+1;
				bgcount++;
				break;
			}
			i++;
		}

		if(bg==0)
		{
			wait_pid=waitpid(pid, &status, WUNTRACED);
		}
		if(ctrl)
		{
			if(pid+1==thepid)
			{}
			else
			{
				kill(pid,19);
				strcpy(bgname[bgcount],arguments[0]);
				bgpid[bgcount]=pid+1;
				bgcount++;
			}
		}
	}
	return 1;
}

int execute(char **arguments,int flag, int length,int k,int thepid)
{

	// printf("%d\n", flag);
	if(arguments[0]==NULL)
		return 1;
	for (int i=0;i<builtinnum;i++)
	{
		if(strcmp(arguments[0],builtin[i])==0)
		{
			int ret;
			if(flag==2)
				ret = builtinfunc(i,arguments,flag,length-1,k);
			else
				ret = builtinfunc(i,arguments,flag,length,k);
			return ret;
		}
	}
	int ret = normalfunc(arguments,flag,length,k,thepid);
	return ret;
}
