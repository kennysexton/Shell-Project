#ifndef COMMANDS_H_
#define COMMANDS_H_

void cd(int argc, char **argv);  // cd command
void clear(); // clr command
void *dir(int argc, char **argv); // dir command
void environls();  // environ command
void echo(int argc, char **argv); // echo command
void help(); // help command
void usrPause(); // Pause command
char *upOne(char **argv); // Handles "cd .." case
void *cmdChoice(int argc, char **argv); // What command is called
int builtinCheck(char **argv);  // check if user typed a builtin

#endif // COMMANDS_H_
