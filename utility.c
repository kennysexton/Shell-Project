// Kenneth Sexton
// CIS 3207
// Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int outputReDir(char **left, char **right){
	FILE *fp;

	printf("test\n");
	printf("%s\n", right[0]);
	printf("test\n");

	fp = fopen(right[0], "w");
	printf("did we make it?\n");
		

}