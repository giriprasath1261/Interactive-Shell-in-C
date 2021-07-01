file included :
	1. shell.c - takes input splits command and prints the terminal line(name@system~>)
	2. execute2.c - finds whether command is inbuilt or not and calls respective command
					also does the background and foreground differenciation
	3. inbuilt.c - contains the inbuilt functions except ls ans pinfo
	4. ls.c - contains command for "ls"
	5. pinfo.c - contains command for "pinfo"
	6.jobs.c - contains jobs,fg,bg etc
	7. redirec.c - checks for redirection
commands:
   	cd ,ls ,echo ,pinfo ,cd ,exit and other system commands with appropriate flags and inputs

use make to compile and ./shell to run
