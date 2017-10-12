// Kenneth Sexton
// CIS 3207
// Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "commands.h"

#define READ 0
#define WRITE 1

#define TRUE 1
#define FALSE 0

int fd[2];

int outputReDir(char **left, char **right, int leftSize, int builtin){
	FILE *fp;
	char *str;
		
	if (right[0] != NULL){
		str = right[0];
		if (strstr(str, ".txt") == NULL){ // adds a .txt ending if none is present
			strcat(str, ".txt");
		}
		

		printf("%s\n", str);
		int file = open(str, O_WRONLY | O_CREAT | O_EXCL, 0666);

		if (builtin == FALSE){
			pid_t pid=fork();
			if(pid==0){ //In child process
				
				printf("command: %s", left[0]);
				dup2(file, 1);
				execvp(left[0], left);
				
				printf("This will not be printed if the execlp call succeeds\n");
			}
			else{ // In parent process
				waitpid(pid,NULL, 0);
			}
		} 
		else {
			int saved_stdout;

			saved_stdout = dup (1);
			dup2(file, 1);
			cmdChoice(leftSize, left);

			dup2(saved_stdout, 1);
			close(saved_stdout);
		}
	}
		//fclose(fp);	
}