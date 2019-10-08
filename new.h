#ifndef NEW_H
#define NEW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedlist.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


char **str_to_array(char *str);
q *tok_pipes(q *parent_list, char **str);
q *str_to_linkedlist(char **str);
q *pipe_cmd(q *parent_list, char **str);
void print_str(char **str);
void print_q(q *command_list);
void handle(q *pipe_list);
void handle_internal_cmd(q *command);
void in_redirect_cmd(char *FILENAME); 
void out_redirect_cmd(char *FILENAME);
void handle_redirects(q *command);

#endif //NEW_H
