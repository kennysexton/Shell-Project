// Kenneth Sexton
// CIS 3207
// Project 2

	/* Functions */
void printError();
void systemcommand(char **argv, int background);

	/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
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

#define BUF_SIZE 64

int fd[2];

void outputReDir(char **left, char **right, int leftSize, int builtin, int background){  // if the '>' character is used correctly then this function will execute

	char *str;
		
	if (right[0] != NULL){
		str = right[0];
		if (strstr(str, ".txt") == NULL){ // adds a .txt ending if none is present
			strcat(str, ".txt");
		}

		int file = open(str, O_WRONLY | O_CREAT | O_EXCL, 0777);

		if (builtin == FALSE){  // If the user is not using one of my built in commands

				int saved_stdout; // save state
				saved_stdout = dup (1);

				dup2(file, 1);
				systemcommand(left, background);	// where fork and exec are done

				dup2(saved_stdout, 1);  // returns back to normal stdout
				close(saved_stdout);
		} 
		else {  // One of my builtin commands is being used
			
				/******* Background Operation ******/
			if (background == TRUE){  // if there is an &
				pid_t pid=fork();
				
				if (pid==0){ // in child

					dup2(file, 1);  // output redirection

					cmdChoice(leftSize, left);
					_exit(0);  // child exits when done
				}
				else {
					; // don't wait
				}
				/************* end *************/
			}
			else {  // No & symbol
				int saved_stdout;
				saved_stdout = dup (1);

				dup2(file, 1);  // output redirection

				cmdChoice(leftSize, left);

				dup2(saved_stdout, 1);  // returns back to normal stdout
				close(saved_stdout);
			}	
		}
	}	
}

void append(char **left, char **right, int leftSize, int builtin, int background){	// if the '>>' character is used correctly then this function will execute

	char *str;
		
	if (right[0] != NULL){
		str = right[0];
		if (strstr(str, ".txt") == NULL){ // adds a .txt ending if none is present
			strcat(str, ".txt");
		}

		int file = open(str, O_WRONLY|O_APPEND | O_CREAT, 0777);

		if (builtin == FALSE){  // If the user is not using one of my built in commands
			
			int saved_stdout;
			saved_stdout = dup (1);

			dup2(file, 1);
			systemcommand(left, background);  // where fork and exec are done

			dup2(saved_stdout, 1);  // returns back to normal stdout
			close(saved_stdout);
		} 
		else {  // One of my built in commands is being used
				
				/******* Background Operation ******/
			if (background == TRUE){  // if there is an &
				pid_t pid=fork();
				
				if (pid==0){ // in child

					dup2(file, 1);  // output redirection

					cmdChoice(leftSize, left);
					_exit(0);  // child exits when done
				}
				else {
					; // don't wait
				}
			}	/************* end *************/
			else {
				int saved_stdout;
				saved_stdout = dup (1);

				dup2(file, 1);  // output redirection

				cmdChoice(leftSize, left);

				dup2(saved_stdout, 1);  // returns back to normal stdout
				close(saved_stdout);	
			}
		}
	}		
}

void inputReDir(char **left, char **right, int leftSize, int builtin, int background){

	char *str;
	char buffer[100];
	int pos = 2;
	str = right[0];

	if(leftSize == 0){ // no left hand argument
		printError();
	}
	else {
		int file = open(str, O_RDONLY);

		if (file == -1){  // if open call fails
			printf(ANSI_COLOR_BRIGHT_RED "File not found: " ANSI_COLOR_RESET);  
			printf("type "); 
			printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
			printf(" to view manual\n");
		}
		else {  // file was found
			read(file, buffer, 100);  // read to a buffer
			//write(1,buffer , 100);

			char *token;
			char flags[] = {' ', '-', '\n'};

			token = strtok(buffer, flags);
			left[1] = token;

			while (token != NULL){
					token = strtok(NULL, flags);
					left[pos] = token;
					pos++;
			}
			if (builtin == FALSE){
				systemcommand(left, background);
			}
			else {
					/******* Background Operation ******/
				if (background == TRUE){  // if there is an &
					pid_t pid=fork();
					
					if (pid==0){ // in child

						cmdChoice(pos -1, left);
						_exit(0);  // child exits when done
					}
					else {
						; // don't wait
					}
				}	/************* end *************/
				else {
					cmdChoice(pos -1 , left);
				}
			}
		}
	}
}
/* 	4 Cases
*  1.) Left = False & Right = False
*  2.)
*/
void mypipe(char **left, char **right, int leftSize, int builtin, int background){

	pid_t pid;
	int fd[2];
	char buf[BUF_SIZE];
	int bytes_read;

	pid_t pid1;
	if (builtin == FALSE){
		pid1=fork();
		if (pid1==0){ // in child
	
			if(pipe(fd)==-1){
				printf("piper error\n");
				return;
			}

			if((pid=fork()) == -1) {
				printf("fork Error\n");
				return;
			}
			else if (pid==0){

				close(1);
				dup2(fd[WRITE], 1);
				close(fd[READ]);
				execvp(left[0], left);

			}
			else {
				close(0);
				dup2(fd[READ], STDIN_FILENO);
				close(fd[WRITE]);
				execvp(right[0], right);
			}		
		}
		else { // in parent
			if (background == TRUE){ // if & symbol DO NOT WAIT
				; // No waiting
			}
			else {
				waitpid(pid,NULL, 0);
			}
		}
	}
	else {
		pid1=fork();
		if (pid1==0){ // in child

			if(pipe(fd)==-1){
				printf("piper error\n");
				return;
			}

			if((pid=fork()) == -1) {
				printf("fork Error\n");
				return;
			}
			else if (pid==0){

				close(1);
				dup2(fd[WRITE], 1);
				close(fd[READ]);
				cmdChoice(leftSize, left);
				_exit(0);  // child exits when done
			}
			else {
				close(0);
				dup2(fd[READ], STDIN_FILENO);
				close(fd[WRITE]);
				execvp(right[0], right);
			}
		}
		else { // in parent
			if (background == TRUE){ // if & symbol DO NOT WAIT
				; // No waiting
			}
			else {
				waitpid(pid,NULL, 0);
			}
		}
	}
} // end function

	// Non builtins get executed form /bin
void systemcommand(char **argv, int background){
	pid_t pid=fork();
	if(pid==0){ //In child process

		execvp(argv[0], argv);  // execs from /bin
			// Printed only if exec fails
		printError();
	}
	else{ // In parent process
		if (background == TRUE){ // if & symbol DO NOT WAIT
			; // No waiting
		}
		else {
			waitpid(pid,NULL, 0);
		}
	}
}

void printError(){
	printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);  
	printf("type "); 
	printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
	printf(" to view manual\n");	
}