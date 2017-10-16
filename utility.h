#ifndef UTILITY_H_
#define UTILITY_H_

void outputReDir(char **left, char **right, int leftSize, int builtin, int background); // >
void append(char **left, char **right, int leftSize, int builtin, int background); // >>
void inputReDir(char **left, char **right, int leftSize, int builtin, int background); // <
void mypipe(char **left, char **right, int leftSize, int builtin, int background); // pipe
void systemcommand(char **argv, int background); //For system commands


#endif // UTILITY_H_
