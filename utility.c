// Kenneth Sexton
// CIS 3207
// Project 2

	/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "commands.h"

	/* Colors */
#define ANSI_COLOR_CYAN_BOLD  "\033[1m\033[36m"  
#define ANSI_COLOR_YELLOW_BOLD   "\033[1m\033[33m"
#define ANSI_COLOR_BIRGHT_BLUE "\033[1m\033[34m"
#define ANSI_COLOR_BRIGHT_RED "\033[1m\033[31m"
#define ANSI_COLOR_RESET  "\033[0m"

	/* Defines */
#define READ 0
#define WRITE 1

#define TRUE 1
#define FALSE 0

int fd[2];

void outputReDir(char **left, char **right, int leftSize, int builtin){  // if the '>' character is used correctly then this function will execute
	FILE *fp;
	char *str;
	int status;
		
	if (right[0] != NULL){
		str = right[0];
		if (strstr(str, ".txt") == NULL){ // adds a .txt ending if none is present
			strcat(str, ".txt");
		}

		int file = open(str, O_WRONLY | O_CREAT | O_EXCL, 0777);

		if (builtin == FALSE){  // If the user is not using one of my built in commands
			pid_t pid=fork();
			if(pid==0){ //In child process
				
				dup2(file, 1);
				execvp(left[0], left);  // execs from /bin

					// Printed only if exec fails
				printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);  
				printf("type "); 
				printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
				printf(" to view manual\n");
			}

			else{ // In parent process
				waitpid(pid,NULL, 0);
			}
		} 
		else {  // One of my built in commands is being used
			int saved_stdout;
			saved_stdout = dup (1);

			dup2(file, 1);  // output redirection

			cmdChoice(leftSize, left);

			dup2(saved_stdout, 1);  // returns back to normal stdout
			close(saved_stdout);	
		}
	}	
}

void append(char **left, char **right, int leftSize, int builtin){

	FILE *fp;
	char *str;
	int status;
		
	if (right[0] != NULL){
		str = right[0];
		if (strstr(str, ".txt") == NULL){ // adds a .txt ending if none is present
			strcat(str, ".txt");
		}

		int file = open(str, O_WRONLY|O_APPEND | O_CREAT, 0777);

		if (builtin == FALSE){  // If the user is not using one of my built in commands
			pid_t pid=fork();
			if(pid==0){ //In child process
				
				dup2(file, 1);
				execvp(left[0], left);  // execs from /bin

					// Printed only if exec fails
				printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);  
				printf("type "); 
				printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
				printf(" to view manual\n");
			}

			else{ // In parent process
				waitpid(pid,NULL, 0);
			}
		} 
		else {  // One of my built in commands is being used
			int saved_stdout;
			saved_stdout = dup (1);

			dup2(file, 1);  // output redirection

			cmdChoice(leftSize, left);

			dup2(saved_stdout, 1);  // returns back to normal stdout
			close(saved_stdout);	
		}
	}	
}