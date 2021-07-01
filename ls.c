#include<sys/wait.h>
#include<sys/utsname.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include <dirent.h>
#include<time.h>
#include <pwd.h>
#include <grp.h>
// #include "execute.c"
# define st_mtime st_mtim.tv_sec

int functioner (char **arguments,char currdir[256],struct dirent *file,struct stat mystat)
{


	char *perms = malloc(100 * sizeof(char*));
	char path[128];
	strcpy(path,currdir);
	strcat(path,"/");
	strcat(path,file->d_name);
	stat(path,&mystat);
	time_t modt = mystat.st_mtime;
	struct passwd *pass;
	struct group *pass2;
	if (mystat.st_mode & S_IRUSR)
	{	
		perms[0]='r';
	}
	else
	{
		perms[0]='-';
	}
	if (mystat.st_mode & S_IWUSR)
	{
		perms[1]='w';
	}
	else
	{
		perms[1]='-';
	}
	if (mystat.st_mode & S_IXUSR)
	{
		perms[2]='x';
	}
	else
	{
		perms[2]='-';
	}

	if (mystat.st_mode & S_IRGRP)
	{
		perms[3]='r';
	}
	else
	{
		perms[3]='-';
	}
	if (mystat.st_mode & S_IWGRP)
	{
		perms[4]='w';
	}
	else
	{
		perms[4]='-';
	}
	if (mystat.st_mode & S_IXGRP)
	{
		perms[5]='x';
	}
	else
	{
		perms[5]='-';
	}
	if (mystat.st_mode & S_IROTH)
	{
		perms[6]='r';
	}
	else
	{
		perms[6]='-';
	}
	if (mystat.st_mode & S_IWOTH)
	{
		perms[7]='w';
	}
	else
	{
		perms[7]='-';
	}

	if (mystat.st_mode & S_IXOTH)
	{
		perms[8]='x';
	}
	else
	{
		perms[8]='-';
	}
	perms[9]='\0';
	printf("%s ", perms);
	char *timed = ctime(&modt);
	int tempo = strlen(timed);
	timed[24]=' ';
	printf(" %ld",mystat.st_nlink);
	pass=getpwuid(mystat.st_uid);
	pass2=getgrgid(mystat.st_gid);
	printf(" %s ", pass->pw_name);
	printf(" %s ", pass2->gr_name);
	printf(" %s ", timed  );
	printf("%d  ",mystat.st_size );
	
	// printf("%s\n",file->d_name);
	return 1;
}

int commandls(char **arguments,int flag ,int length,int k)
{
	// printf("%d\n", length);
	if(length==1)
	{
		char currdir[256];
		getcwd(currdir,sizeof(currdir));
		DIR *directory;
		struct dirent *file;
		directory = opendir(currdir);
		char temp[256];
		
		if(flag==10)
			freopen("tempout.txt","a+",stdout);

		if(flag ==2||flag ==3)
		{
			freopen(arguments[length], "a+", stdout);
		}		
		
		while((file = readdir(directory)) != NULL)
		{
			if(file->d_name[0]!='.')
				printf("%s\n",file->d_name);
		}
		
		freopen("/dev/tty", "w", stdout);

		closedir(directory);
		return 1;
	}
	else
	{
		if(arguments[1][0]!='-')
		{
			DIR *directory;
			struct dirent *file;
			directory = opendir(arguments[1]);
			if(directory==NULL)
			{
				perror("shellmax");
				return 1;
			}
			char temp[256];
		
			if(flag==10)
				freopen("tempout.txt","a+",stdout);

			if(flag ==2||flag ==3)
			{
				freopen(arguments[length], "a+", stdout);
			}		

			while((file = readdir(directory)) != NULL)
			{
				if(file->d_name[0]!='.')
					printf("%s\n",file->d_name);
			}

			freopen("/dev/tty", "w", stdout);
			closedir(directory);
			return 1;
		}
		else
		{
			if(length==2)
			{
				if(arguments[1][1]=='a')
				{
					char currdir[256];
					getcwd(currdir,sizeof(currdir));
					DIR *directory;
					struct dirent *file;
					struct stat mystat;
					directory = opendir(currdir);
					char temp[256];
		
					if(flag==10)
						freopen("tempout.txt","a+",stdout);

					if(flag ==2||flag ==3)
					{
						freopen(arguments[length], "a+", stdout);
					}	
		
					while((file = readdir(directory)) != NULL)
					{
						if(arguments[1][2]=='l')
						{
							functioner(arguments,currdir,file,mystat);
						}
						printf("%s\n",file->d_name);
					}
					freopen("/dev/tty", "w", stdout);
					closedir(directory);
					return 1;
				}
				else if(arguments[1][1]=='l')
				{
					char currdir[256];
					getcwd(currdir,sizeof(currdir));
					DIR *directory;
					struct dirent *file;
					struct stat mystat;
					directory = opendir(currdir);
					char temp[256];
	
					if(flag==10)
						freopen("tempout.txt","a+",stdout);
	
					if(flag ==2||flag ==3)
					{
						freopen(arguments[length], "a+", stdout);
					}
					while((file = readdir(directory)) != NULL)
					{
						if(file->d_name[0]!='.' || arguments[1][2]=='a')
						{
							functioner(arguments,currdir,file,mystat);
							printf("%s\n",file->d_name);
						}
					}
					freopen("/dev/tty", "w", stdout);
					closedir(directory);
					return 1;
				}
				else
				{
					fprintf(stderr, "undefined flag  input\n" );
					return 1;
				}
			}
			else if(arguments[2][0]!='-')
			{
				if(arguments[1][1]=='a')
				{
					char currdir[256];
					getcwd(currdir,sizeof(currdir));
					DIR *directory;
					struct dirent *file;
					struct stat mystat;
					directory = opendir(arguments[2]);
					if(directory==NULL)
					{
						perror("shellmax");
						return 1;
					}
					char temp[256];

					while((file = readdir(directory)) != NULL)
					{
						if(arguments[1][2]=='l')
						{
							char path[256];
							strcpy(path,currdir);
							strcat(path,"/");
							strcat(path,arguments[2]);
							functioner(arguments,path,file,mystat);
						}

						if(flag==10)
							freopen("tempout.txt","a+",stdout);

						if(flag ==2||flag ==3)
						{
							freopen(arguments[length], "a+", stdout);
						}

						printf("%s\n",file->d_name);
						freopen("/dev/tty", "w", stdout);
					}
					closedir(directory);
					return 1;
				}
				else if(arguments[1][1]=='l')
				{
					char currdir[256];
					getcwd(currdir,sizeof(currdir));
					DIR *directory;
					struct dirent *file;
					struct stat mystat;
					directory = opendir(arguments[2]);
					char temp[256];
					
					if(flag==10)
						freopen("tempout.txt","a+",stdout);

					if(flag ==2||flag ==3)
					{
						freopen(arguments[length], "a+", stdout);
					}

					while((file = readdir(directory)) != NULL)
					{
						if(file->d_name[0]!='.' || arguments[1][2]=='a')
						{
							char path[256];
							strcpy(path,currdir);
							strcat(path,"/");
							strcat(path,arguments[2]);
							functioner(arguments,path,file,mystat);
							printf("%s\n",file->d_name);
						}
					}
					freopen("/dev/tty", "w", stdout);
					closedir(directory);
					return 1;
				}
				else
				{
					fprintf(stderr, "undefined flag  input\n" );
					return 1;
				}
			}
			else if(arguments[2][0]=='-')
			{
				char currdir[256];
				getcwd(currdir,sizeof(currdir));
				char checkst[2];
				checkst[0]=arguments[1][1];
				checkst[1]=arguments[2][1];
				checkst[2]='\0';
				if(strcmp(checkst,"la")!=0 && strcmp(checkst,"al")!=0)
				{
					fprintf(stderr, "invalid flag  given\n");
					return 1;
				}
				if(length==3)
				{
					DIR *directory;
					struct dirent *file;
					struct stat mystat;
					directory = opendir(currdir);
					char temp[256];
					
					if(flag==10)
						freopen("tempout.txt","a+",stdout);

					if(flag ==2||flag ==3)
					{
						freopen(arguments[length], "a+", stdout);
					}

					while((file = readdir(directory)) != NULL)
					{
						functioner(arguments,currdir,file,mystat);
						printf("%s\n",file->d_name);
					}
					freopen("/dev/tty", "w", stdout);
					fprintf(stderr, "undefined flag  input\n" );
					closedir(directory);
					return 1;		
				}
				else
				{
					DIR *directory;
					struct dirent *file;
					struct stat mystat;
					directory = opendir(arguments[3]);
					if(directory==NULL)
					{
						perror("shellmax");
						return 1;
					}
					char temp[256];

					if(flag==10)
						freopen("tempout.txt","a+",stdout);

					if(flag ==2||flag ==3)
					{
						freopen(arguments[length], "a+", stdout);
					}


					while((file = readdir(directory)) != NULL)
					{
						char path[256];
						strcpy(path,currdir);
						strcat(path,"/");
						strcat(path,arguments[3]);
						functioner(arguments,path,file,mystat);
						printf("%s\n",file->d_name);
					}
					freopen("/dev/tty", "w", stdout);
					fprintf(stderr, "undefined flag  input\n" );
					closedir(directory);
					return 1;	
				}
			}
		}
	}
}