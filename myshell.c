// Kenneth Sexton
// CIS 3207
// Project 2

	/* Function Declarations */
void shellEnvir();  // Sets the shell environment variable to the path which it was executed
void prompt();  // Print the prompt
char * relpath(char *path); // prints ~ if relative path is displayed
const char * pathTrim(char *path);  // prints path or relative path on prompt
int checkToken(char* token);

	/* Batch File */
char *inputFromFile();

	/* Check */
int builtinCheck(char **argv);  // check if user typed a builtin


	/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"  // commands.c  contains built ins
#include "utility.h"  // utiliy.c contains io modifier code and & code

	/* Colors */
#define ANSI_COLOR_CYAN_BOLD  "\033[1m\033[36m"  
#define ANSI_COLOR_YELLOW_BOLD   "\033[1m\033[33m"
#define ANSI_COLOR_BIRGHT_BLUE "\033[1m\033[34m"
#define ANSI_COLOR_BRIGHT_RED "\033[1m\033[31m"
#define ANSI_COLOR_RESET  "\033[0m"

	/* Defines */
#define TRUE 1
#define FALSE 0

	/* Globals */

	/* Main Function */
int main() {

	int i, j;
	int special;
	int background = 0;
	int builtin = 0;
	int specialpos;
	char **argv;
	char **left;
	char **right;
	int ioredirect = FALSE;
	argv = malloc(sizeof(char)* 200);
	left = malloc(sizeof(char)* 100);
	right = malloc(sizeof(char)* 100);

	int argc = 0; // argument counter

	char batchfile[] = {"myshell batchfile\n"}; // batchfile command
	char lineInput[80];
	char fileInput[80];

	shellEnvir();
	system("clear");  // Use System clear to give the shell a clean look on startup

	while( strcmp(lineInput, "quit") != 0 && strcmp(lineInput, "exit") != 0){ // exit the program by typing "quit","exit" or using your escape command ex. ctrl^c

		prompt();	
		special = -1;
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
				token = strtok(fileInput, flags);
			}
			else {
				token = strtok(lineInput, flags);
				
			}

				/* ----------------------------------- Parse Input ----------------------------------- */
			argv[argc] = token;
			// printf("%s %s %d\n", token, argv[argc], argc);

			while (token != NULL){		
				token = strtok(NULL, flags);
				argc++;
				argv[argc] = token;
				// printf("%s %s %d\n", token, argv[argc], argc);
			}

			builtin = builtinCheck(argv);

			if (builtin == FALSE){
				special = 4;
			}
			
			/* ----------------------------------- & ------------------------------------ */
			if (strcmp(argv[argc - 1], "&") == 0){  // test for timinating background char &
			 		background = TRUE;
			 		argv[argc -1] = " ";  // Remove the & character, Don't need it anymore
			 }


			/* ----------------------------------- Special Cases ----------------------------------- */   // > >> < | & 
			specialpos = 0;

			for (i = 0; i < argc ; i++){  // Look at array input
			 	if (strcmp(argv[i], ">") == 0){
			 		special = 0;
			 		specialpos = i;
			 		ioredirect = TRUE;
			 	}
			 	else if (strcmp(argv[i], ">>") == 0){
			 		special = 1;
			 		specialpos = i;
			 		ioredirect = TRUE;
			 	}
			 	else if (strcmp(argv[i], "<") == 0){
			 		special = 2;
			 		specialpos = i;
			 		ioredirect = TRUE;
			 	}
			 	else if (strcmp(argv[i], "|") == 0){
			 		special = 3;
			 		specialpos = i;
			 		ioredirect = TRUE;
			 	}
			}

			/* ----------------------------------- Left Right Split ------------------------------------ */
			if (ioredirect == TRUE){

					/* Checks */
				if (argc - 1 == specialpos){  // modifiers > >> < | require arguments on both sides of modifier
					special = 5;  // set to error
				}
				else{
					for(i=0; i< specialpos; i++){ // fill left
						left[i] = argv[i];
					}
					i=0;

					for(j = specialpos + 1; j < argc; j++){ // fill right
						right[i] = argv[j];
						i++;
					}
				}
			}

			switch(special) {
				case 0:		// >
					outputReDir(left, right, specialpos, builtin, background);
					break;
				case 1:		// >>
					append(left, right, specialpos, builtin, background);
					break;
				case 2:		// <
					inputReDir(left, right, specialpos, builtin, background);
					break;
				case 3:		// |
					printf("pipe\n");
					break;
				case 4:
					systemcommand(argv, background);
					break;
				case 5: // error
					printf(ANSI_COLOR_BRIGHT_RED "Misuse of modifier: " ANSI_COLOR_RESET);
					printf("type "); 
					printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
					printf(" to view manual\n");
					break;
				default:  // No speical character
					cmdChoice(argc, argv);
				}

				// Rest argc and argv	
			argc = 0;
			ioredirect = FALSE;
			background = FALSE;
			for (i = 0; i < argc; i++){
				argv[i] = NULL;
			}
		}
	}
	free(argv);
	free(left);
	free(right);
}

/*---------------------------------------------------------Functions---------------------------------------------------------*/
void shellEnvir(){  // sets the shell environment variable to the path which it was executed
	setenv("SHELL", getenv("PWD"), 1);
}

	// Prints the prompt.  appears whenever the shell is ready to recieve input
void prompt(){	
		/* Printing */
	printf(ANSI_COLOR_CYAN_BOLD "%s@kshell" ANSI_COLOR_RESET, getenv("USER"));  // Prints the name of the shell
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

int builtinCheck(char **argv){
	if(strcmp(argv[0], "cd") == 0){  // cd
		return TRUE;
	}
	else if(strcmp(argv[0], "clr") == 0 || strcmp(argv[0], "clear") == 0) {  //clear
		return TRUE;
	}
	else if(strcmp(argv[0], "dir") == 0){ // dir
		return TRUE;	
	}
	else if (strcmp(argv[0], "environ") == 0){ // environ
		return TRUE;
	}
	else if (strcmp(argv[0], "echo") == 0){ // echo 	
		return TRUE;
	}
	else if (strcmp(argv[0], "exit") == 0){ // exit 	
		return TRUE;
	}
	else if(strcmp(argv[0], "help") == 0){ // help
		return TRUE;
	}
	else if(strcmp(argv[0], "ls") == 0){ // ls
		return TRUE;
	}
	else if(strcmp(argv[0], "pause") == 0){ // pause
		return TRUE;
	}
	else if(strcmp(argv[0], "pwd") == 0){  //pwd
		return TRUE;
	}
	else if (strcmp(argv[0], "quit") == 0){ // quit	
		return TRUE;
	}
	else {
		return FALSE;
	}
}