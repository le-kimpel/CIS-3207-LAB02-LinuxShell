#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
//test c program to practice using fork(), pipe(), and dup2() functions to redirect I/O

int main(){

  //creating the file descriptor for both ends of the pipe
  int fd[1], pid;

  if (pipe(fd) < 0){
    exit(0);
  }
  fork();
  if (pid == 0){
    printf("%s\n", "We are in the child!");
    printf("%s%d\n", "Child ID: ", pid);
    close(fd[0]); //no reading
    dup2(fd[1],1); //rewrites to page table of the current process
  }
  else if (pid > 0 ){
    printf("%s\n", "We are in the parent!");
    printf("%s%d\n", "Parent ID: ", pid);
    close(fd[1]); //no writing
    dup2(fd[0], 0);
  }
  













  return 0;
}
