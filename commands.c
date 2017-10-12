// Kenneth Sexton
// CIS 3207
// Project 2

char *upOne(char **argv); // Handles "cd .." case


	/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> // for dir() and ls();

	/* Colors */
#define ANSI_COLOR_CYAN_BOLD  "\033[1m\033[36m"  
#define ANSI_COLOR_YELLOW_BOLD   "\033[1m\033[33m"
#define ANSI_COLOR_BIRGHT_BLUE "\033[1m\033[34m"
#define ANSI_COLOR_BRIGHT_RED "\033[1m\033[31m"
#define ANSI_COLOR_RESET  "\033[0m"

	/* Defines */
#define TRUE 1
#define FALSE 0

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

void dir(int argc, char **argv){

	if (chdir(argv[1]) == 0){
		DIR *d = opendir(argv[1]);
		struct dirent *dir;
		if (d){
			while ((dir = readdir(d)) != NULL){
				printf("%s\n", dir->d_name);
			}
		closedir(d);	
		}
	}
	else{
		printf(ANSI_COLOR_BRIGHT_RED "Invalid path: " ANSI_COLOR_RESET);			
		printf("dir <path>\n");
		return;
	}
	return;
}

void environls(){
	extern char **environ;
	int i;

	for(i=0; environ[i] != NULL; i++){
		printf("%s\n", environ[i]);
	}
}

void echo(int argc, char **argv){
	int i;

	if(argv[1] != NULL){
		for (i = 1; i < argc; i++){
			printf("%s ", argv[i]);
		}
		printf("\n");
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

				if (strcmp(space, " \n") == 0 || strcmp(space, "\n") == 0){ // If user entered space, display 25 more
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

void *ls(){ // slightly modified dir command

	DIR *d = opendir(getenv("PWD"));
	struct dirent *dir;
	if (d){
		while ((dir = readdir(d)) != NULL){
			printf("%s ", dir->d_name);
		}
	}
	// printf("\n");
	closedir(d);
	
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

	// uses a command based on input
void cmdChoice(int argc, char **argv){
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
		printf("\n");
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