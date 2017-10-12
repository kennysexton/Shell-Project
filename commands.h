#ifndef COMMANDS_H_
#define COMMANDS_H_

void cd(int argc, char **argv);  // cd command
void clear(); // clr command
char *dir(int argc, char **argv); // dir command
void environls();  // environ command
void echo(int argc, char **argv); // echo command
char *ls();
void help(); // help command
void usrPause(); // Pause command
char *upOne(char **argv); // Handles "cd .." case
char *cmdChoice(int argc, char **argv); // What command is called

#endif // COMMANDS_H_
