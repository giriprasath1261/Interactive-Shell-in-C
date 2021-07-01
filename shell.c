#include<sys/wait.h>
#include<sys/utsname.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include <getopt.h>
#include <time.h>
#include <fcntl.h>
#include "execute2.c"
#include "redirec.c"
#define delim " \t\r\n\a"

char home[256];
int length=20;
int status=1;
int thepid;

void controlc()
{
	int pid = getpid();
	if(pid==thepid)
	{
		return;
	}
	else
		kill(pid,9);
	// signal(SIGINT,controlcandz);
}


char **pipespliting(char* input)
{
	char **words = malloc(128 * sizeof(char*));
	char *word;
	int pos=0;
	word = strtok(input,"|");
	while(word != NULL)
	{
		words[pos]=word;
		pos++;
		word = strtok(NULL, "|");
	}
	words[pos]=NULL;
	length = pos;
	return words;
}

char **readinputsplit(char* input)
{
	char **words = malloc(128 * sizeof(char*));
	char *word;
	int pos=0;
	word = strtok(input, delim);
	while(word != NULL)
	{
		words[pos]=word;
		pos++;
		word = strtok(NULL, delim);
	}
	words[pos]=NULL;
	length = pos;
	return words;
}


int main(int argc,char **argv)
{
	thepid = getpid();
	// printf("%d\n", thepid);
	signal(SIGINT,controlc);
	
	char **arguments;
	char **commands = malloc(128 * sizeof(char*));
	char currdir[256];
	char *p = getenv("USER");
	struct utsname username;
	uname(&username);
	getcwd(currdir,sizeof(currdir));
	strcpy(home,currdir);
	int cancel = strlen(home);
	int flag=0;
	while(status)
	{
		flag=0;
		getcwd(currdir,sizeof(currdir));
		int total = strlen(currdir);
		if(total<cancel)
		{
			flag=1;
		}
		else
		{
			for(int i=0;i<cancel;++i)
			{

				if(home[i]!=currdir[i])
				{
					flag=1;
					break;
				}
			}
		}
		if(flag==0)
		{
			char path[256];
			path[0]='~';
			int i;
			for(i=1;i<=total-cancel;i++)
				path[i]=currdir[cancel+i-1];
			path[i]='\0';
			printf("<%s@%s:%s>", p,username.nodename,path);
		}
		else
		{
			printf("<%s@%s:%s>", p,username.nodename,currdir);
		}
		
		char *input = NULL;
		size_t size = 0;
		getline(&input, &size, stdin);
		char *command;
		int pos=0;
		command = strtok(input,";");
		while(command!=NULL)
		{
			commands[pos]=command;
			pos++;
			command = strtok(NULL,";");
		}
		commands[pos]="NULL";

		for(int i=0;i<pos;i++)
		{
			char **pipesplit = pipespliting(commands[i]);
			int pipe=0;
			while(pipesplit[pipe]!=NULL)
				pipe++;

			for(int k=0;k<pipe;k++)
			{
				if(k!=0)
				{
					remove("tempin.txt");
					rename("tempout.txt","tempin.txt");
				}
				int flagpipe=0;
				for(int j=0;j<strlen(pipesplit[k]);j++)
				{
					if(pipesplit[k][j]=='<')
					{	
						pipesplit[k][j]=' ';
						if(flagpipe==2)
							flagpipe=3;
						else if(flagpipe==4)
							flagpipe=5;
						else
							flagpipe=1;
					}
					if(pipesplit[k][j]=='>')
					{
						pipesplit[k][j]=' ';
						if(pipesplit[k][j+1]=='>')
						{
							pipesplit[k][j+1]=' ';
							if(flagpipe==1)	
								flagpipe=5;
							else
								flagpipe=4;
						}
						else
						{
							if(flagpipe==1)	
								flagpipe=3;
							else
								flagpipe=2;
						}
					}
				}
				if(pipe>1 && k!=pipe-1)
					flagpipe=flagpipe+10;
				arguments = readinputsplit(pipesplit[k]);

				status = execute(arguments,flagpipe,length,k,thepid);
			}
		}

	}
	return EXIT_SUCCESS;
}
