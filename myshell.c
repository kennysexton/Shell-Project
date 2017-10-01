// Kenneth Sexton
// CIS 3207
// Project 2

	/* Function Declarations */
void prompt();
char * relpath(char *path); // prints ~ if relative path is displayed
const char * pathTrim(char *path);  // prints path or relative path on prompt
void exitmsg(); // quit or exit command

	/* Build Ins */
void cd(int argc, char **argv);  // cd command
void clear(); // clr command
char *dir(int argc, char **argv); // dir command
void environls();  // environ command
void echo(int argc, char **argv); // echo command
void help(); // help command
void usrPause(); // Pause command

char *upOne(char **argv); // Handles "cd .." case


	/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> // for dir();

	/* Colors */
#define ANSI_COLOR_CYAN_BOLD  "\033[1m\033[36m"  
#define ANSI_COLOR_YELLOW_BOLD   "\033[1m\033[33m"
#define ANSI_COLOR_BIRGHT_BLUE "\033[1m\033[34m"
#define ANSI_COLOR_BRIGHT_RED "\033[1m\033[31m"
#define ANSI_COLOR_RESET  "\033[0m"


	/* Main Function */
int main() {

	int i;
	char **argv;
	argv = malloc(sizeof(char)* 100);

	int argc; // argument counter

	char lineInput[80];
	system("clear");  // Use System clear to give the shell a clean look on startup

	while( strcmp(lineInput, "quit") != 0 && strcmp(lineInput, "exit") != 0){ // exit the program by typing "quit","exit" or using your escape command ex. ctrl^c
		
		prompt();	
		
			/* Read Input */
		fgets(lineInput, 80 ,stdin);
		//printf("%s", lineInput);
		if(strlen(lineInput) < 3){
			printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);
			printf("type "); 
			printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
			printf(" to view manual\n");
		}
		else {


				/* Parse Input */
			char *token;
			char flags[] = {' ', '-', '\n'};
			
			token = strtok(lineInput, flags);
			argv[argc] = token;
			// printf("%s %s %d\n", token, argv[argc], argc);
			
			while (token != NULL){		
				token = strtok(NULL, flags);
				argc++;
				argv[argc] = token;
				// printf("%s %s %d\n", token, argv[argc], argc);
			}

			for (i = 0; i < argc; i++){
			 	printf("run: %d   %s\n", i, argv[i]);
			 }

			 printf("%s\n", argv[0] ); // TEMP Debugging

		
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
			else if(strcmp(argv[0], "pwd") == 0){  //pwd
				printf("%s\n", getenv("PWD"));
			}
			else if(strcmp(argv[0], "pause") == 0){ // pause
				usrPause();
			}
			else{
				printf(ANSI_COLOR_BRIGHT_RED "Invalid Command: " ANSI_COLOR_RESET);
				printf("type "); 
				printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
				printf(" to view manual\n");
			}
		
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

	// Prints the prompt.  appears whenever the shell is ready to recieve input
void prompt(){	
		/* Printing */
	printf(ANSI_COLOR_CYAN_BOLD "kenny-shell" ANSI_COLOR_RESET);  // Prints the name of the shell
	printf(":");
	printf(ANSI_COLOR_YELLOW_BOLD "%s%s " ANSI_COLOR_RESET,  relpath(getenv("PWD")),pathTrim(getenv("PWD")) ); // Prints relative directory 
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
		strncpy(trimPath, path+pos2, length - pos2);  // shortens the path to include only the last two '/' characters'
	}
	else{
		strncpy(trimPath, path, length); // trimPath just returns the path if there is only 2 or fewer '/'
	}	
	return  trimPath;
}
	// Prints a message when user quits or exits
void exitmsg(){
	printf(ANSI_COLOR_BIRGHT_BLUE "-- exiting kenny-shell --\n" ANSI_COLOR_RESET);
}

/*------------------------------ Built ins ------------------------------*/
void cd(int argc, char **argv){
	if (argv[1] != NULL){  // print pwd if no argv[1]
		if (chdir(argv[1]) == 0){  // check for valid path
			if((strcmp(argv[1], "..")) == 0){ // if user put cd..
				setenv("PWD", upOne(argv), 1);
			} else {
				setenv("PWD", argv[1], 1);
			}
		}
		else {
			printf(ANSI_COLOR_BRIGHT_RED "Invalid path: " ANSI_COLOR_RESET);			
			printf("cd <path>\n");
		}	
	}
	else {
		printf("%s\n", getenv("PWD"));
	}
}
void clear(){
	system("clear");
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
char *dir(int argc, char **argv){

	if (chdir(argv[1]) == 0){
		DIR *d = opendir(argv[1]);
		struct dirent *dir;
		if (d){
			while ((dir = readdir(d)) != NULL){
				printf("%s\n", dir->d_name);

			}
		}
		closedir(d);
	}
	else{
		printf(ANSI_COLOR_BRIGHT_RED "Invalid path: " ANSI_COLOR_RESET);			
		printf("dir <path>\n");
		return 0 ;
	}
	return 0;
}

void environls(){
	extern char **environ;
	int i;

	for(i=0; environ[i] != NULL; i++){
		printf("%s\n", environ[i]);
	}
}

void echo(int argc, char **argv){
	if(argv[1] != NULL){
		printf("%s\n", argv[1]);
	}
	else {
		printf(ANSI_COLOR_BRIGHT_RED "Invalid format: " ANSI_COLOR_RESET);			
		printf("echo <comment>\n");
	}
}

void help(){
	FILE *rp; // read pointer
	char line[200];
	int numLines = 0;
	char space[10]; 

	rp = fopen("readme", "r");
	while ((fgets(line, 200, rp) != NULL) && numLines < 25){
		
		printf("%s", line );
		
		if(numLines == 24){ // More
			printf("\t--Press space to view more-- ");
			fgets(space, sizeof(space), stdin);

				// printf("Space result: %s\n", space);

				if (strcmp(space, " \n") == 0){ // If user entered space, display 25 more
					numLines = 0;
				}
				else {
					fclose(rp);
					return;
				}
		}
		else {
			numLines++;
		}
	}
	fclose(rp);	
}

void usrPause(){
	printf(ANSI_COLOR_BIRGHT_BLUE "--paused--\n" ANSI_COLOR_RESET);
	printf("Press any key to continue ");
	char endPause[10];

	fgets(endPause, sizeof(endPause), stdin);

}

	
	// For: cd ..
char *upOne(char **argv){ //used in CD
	
	char *path = getenv("PWD");
	int length = strlen(path);
	int pos = 0;
	int i;

	char *parentPath = malloc (sizeof (char) * length);

	for (i=0; i < length; i++){  // scans the path for char '/'
		if(path[i] == '/'){
			pos = i;   // location of last '/'
		}
	}
	int parentLength = length - (length - pos);

	if (pos == 0){ // return root
		strncpy(parentPath, path, parentLength + 1);
		return parentPath;
	}
	
	strncpy(parentPath, path, parentLength); // cuts of the last / and beyond
	return  parentPath;	
}