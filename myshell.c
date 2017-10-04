// Kenneth Sexton
// CIS 3207
// Project 2

	/* Function Declarations */
void shellEnvir();  // Sets the shell environment variable to the path which it was executed
void prompt();  // Print the prompt
char * relpath(char *path); // prints ~ if relative path is displayed
const char * pathTrim(char *path);  // prints path or relative path on prompt
void exitmsg(); // quit or exit command

	/* Batch File */
char *inputFromFile();

	/* Build Ins */
void cd(int argc, char **argv);  // cd command
void clear(); // clr command
char *dir(int argc, char **argv); // dir command
void environls();  // environ command
void echo(int argc, char **argv); // echo command
char *ls(); // ls command
void help(); // help command
void usrPause(); // Pause command

char *upOne(char **argv); // Handles "cd .." case

	/* Command Choice */
char *cmdChoice(int argc, char **argv);

	/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"  // commands.c  contains built ins

	/* Colors */
#define ANSI_COLOR_CYAN_BOLD  "\033[1m\033[36m"  
#define ANSI_COLOR_YELLOW_BOLD   "\033[1m\033[33m"
#define ANSI_COLOR_BIRGHT_BLUE "\033[1m\033[34m"
#define ANSI_COLOR_BRIGHT_RED "\033[1m\033[31m"
#define ANSI_COLOR_RESET  "\033[0m"


	/* Main Function */
int main() {

	int i;
	int special;
	char **argv;
	argv = malloc(sizeof(char)* 200);

	int argc = 0; // argument counter

	char batchfile[] = {"myshell batchfile\n"}; // batchfile command
	char lineInput[80];
	char fileInput[80];

	shellEnvir();
	system("clear");  // Use System clear to give the shell a clean look on startup

	while( strcmp(lineInput, "quit") != 0 && strcmp(lineInput, "exit") != 0){ // exit the program by typing "quit","exit" or using your escape command ex. ctrl^c

		prompt();	

			/* Read Input */
		fgets(lineInput, 80 ,stdin);

		if(strlen(lineInput) < 3){
			printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);
			printf("type "); 
			printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
			printf(" to view manual\n");
		}
		else {

			char *token;
			char flags[] = {' ', '-', '\n'};

			if(strcmp(lineInput, batchfile) == 0){ // if User types "myshell batchfile"
				strcpy(fileInput,inputFromFile());
				// printf("fileInput: %s", fileInput );
				// printf("hello?\n");
				token = strtok(fileInput, flags);
			}
			else {
				token = strtok(lineInput, flags);
			}

				/* Parse Input */
			argv[argc] = token;
			// printf("%s %s %d\n", token, argv[argc], argc);

			while (token != NULL){		
				token = strtok(NULL, flags);
				argc++;
				argv[argc] = token;
				// printf("%s %s %d\n", token, argv[argc], argc);
			}
			special = 0;
			/* ---------- Special Cases ---------- */   // > >> < | & 
			for (i = 0; i < argc; i++){  // Look at array input
			 	printf("run: %d   %s\n", i, argv[i]);
			 	if (strcmp(argv[i], ">") == 0){
			 		special = 1;
			 	}
			 	else if (strcmp(argv[i], ">>") == 0){
			 		printf(">> input redirection append\n");
			 		special = 2;
			 	}
			 	else if (strcmp(argv[i], "<") == 0){
			 		special = 3;
			 	}
			 	else if (strcmp(argv[i], "|") == 0){
			 		special = 4;
			 	}
			 	else if (strcmp(argv[i], "&") == 0){
			 		special = 5;
			 	}
			}

			switch(special) {
				case 0:  	// no special character
					cmdChoice(argc, argv);
					break;
				case 1:		// >
					pid_t pid=fork();
					if(pid==0){ //In child process
						execlp("./outputReDir", "./outputReDir", NULL);
						printf("This will not be printed if the execlp call succeeds\n");
					}
					else{ // In parent process
						printf("you are in the parent process\n");
						waitpid(pid,NULL, 0);
					}
					break;
				case 2:		// >>
					printf("printf append\n");
					break;
				case 3:		// <
					printf("input redir\n");
					break;
				case 4:		// |
					printf("pipe\n");
					break;
				case 5:		// &
					printf("background\n");
					break;
				default:
					printf("error\n");
					exit(1);
				}

			// cmdChoice(argc, argv);
			
				
				// Rest argc and argv	
			argc = 0;
			for (i = 0; i < argc; i++){
				argv[i] = NULL;
			}
		}
	}
	exitmsg(); // Messege on quit
	free(argv);	
}

/*---------------------------------------------------------Functions---------------------------------------------------------*/
void shellEnvir(){  // sets the shell environment variable to the path which it was executed
	setenv("SHELL", getenv("PWD"), 1);
}

	// Prints the prompt.  appears whenever the shell is ready to recieve input
void prompt(){	
		/* Printing */
	printf(ANSI_COLOR_CYAN_BOLD "kenny-shell" ANSI_COLOR_RESET);  // Prints the name of the shell
	printf(":");
	printf(ANSI_COLOR_YELLOW_BOLD "%s%s " ANSI_COLOR_RESET,  relpath(getenv("PWD")),pathTrim(getenv("PWD"))); // Prints relative directory 
	printf("$ ");
}
	 // Will output a "~" to show only a relative path is being printed
char * relpath(char *path){ 
	int length = strlen(path);
	int count = 0;
	int i;
	for (i=0; i < length; i++){  // scans the path for char '/'
		if(path[i] == '/'){
			count++;
		}
	}
	if (count >= 3) {
		return "~";
	}
	return "";
}
	// Displays the current directory and its parent
const char * pathTrim(char *path){
	int i;
	int length = strlen(path);
	int pos2 = 0;
	int pos = 0;
	int count = 0;
	char *trimPath = malloc (sizeof (char) * length);
	
	for (i=0; i < length; i++){  // scans the path for char '/'
		if(path[i] == '/'){
			count++;
			pos2 = pos;  // location of second to last '/'
			pos = i;   // location of last '/'
		}
	}
	if (count >= 3) {
		strncpy(trimPath, path+pos2, length - pos2 + 1);  // shortens the path to include only the last two '/' characters'
	}
	else{
		strncpy(trimPath, path, length); // trimPath just returns the path if there is only 2 or fewer '/'
	}	
	return  trimPath;
}
	// reads batchfile and returns the string contained
char *inputFromFile(){
	FILE *rp; // read pointer
	char *line = malloc(sizeof(char) * 80);
	char c;
	// char line[80];
	int i=0;

	rp = fopen("batchfile", "r"); // open batchfile

	do {
		c = fgetc(rp);  // read file 1 char at a time until '\n' or EOF
		line[i] = c;
		i++;
	}while (c != EOF && c != '\n');

	// printf("%s", line);
	return line;
	fclose(rp);
}

void exitmsg(){	// Prints a message when user quits or exits
	printf(ANSI_COLOR_BIRGHT_BLUE "-- exiting kenny-shell --\n" ANSI_COLOR_RESET);
}

	// uses a command based on input
char *cmdChoice(int argc, char **argv){
	if(strcmp(argv[0], "cd") == 0){  // cd
		cd(argc, argv);
	}
	else if(strcmp(argv[0], "clr") == 0 || strcmp(argv[0], "clear") == 0) {  //clear
		clear();
	}
	else if(strcmp(argv[0], "dir") == 0){ // dir
		dir(argc, argv);	
	}
	else if (strcmp(argv[0], "environ") == 0){ // environ
		environls();
	}
	else if (strcmp(argv[0], "echo") == 0){ // echo 	
		echo(argc, argv);
	}
	else if(strcmp(argv[0], "help") == 0){ // help
		help();
	}
	else if(strcmp(argv[0], "ls") == 0){ // ls
		ls(argc, argv);
	}
	else if(strcmp(argv[0], "pause") == 0){ // pause
		usrPause();
	}
	else if(strcmp(argv[0], "pwd") == 0){  //pwd
		printf("%s\n", getenv("PWD"));
	}
	else{
		printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);
		printf("type "); 
		printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
		printf(" to view manual\n");
	}
}

