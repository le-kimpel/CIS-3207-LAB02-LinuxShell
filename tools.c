#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "linkedlist.h"

q *pipe (q *pipe_list);



int main(){








}


q *pipe(q* pipelist, q *parent_list){
  
  int READ = 0;
  int WRITE = 0;
  int pid = fork();
  int fd[1] = {READ, WRITE};
  
  if (pipe(fd) < 0){
    puts("error during pipe");
    exit(0);
  }
  
  if (pid == 0){
    puts("We are in the child!");
    close(fd[WRITE]);
    dup2(fd[READ], stdin);

  }else if (pid > 0){
    wait();
    close(fd[READ]);
    dup2(fd[WRITE]);
  }
}
    
