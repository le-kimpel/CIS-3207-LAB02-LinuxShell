#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
//file to test inbuilt functions


//clears the screen
void cls ();
//exits the shell
int quit();
//pauses the shell until "Enter" is pushed
void pause_a();
//echoes the most recently printed statement
void echo(char *c);
//prints the environment variables
void environ(char ** env);
//prints the files under the current directory
int dir(char *argv);
//changes the current directory
int  cd(char *argv);
//gets the help manual
void help(char *argv);
//main interface for executing internal commands
int menu(char **argv, char **env);

#endif //INTERNAL_H
