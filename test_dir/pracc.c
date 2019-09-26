#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
//test c program to practice using fork(), pipe(), and dup2() functions to redirect I/O

int main(){

  //creating the file descriptor for both ends of the pipe
  int fd[1], fd2[1], pid;
  int READ = 0;
  int WRITE = 1;

 
  fd[0]= READ;
  fd[1]= WRITE;
  fd2[0] = READ;
  fd2[1] = WRITE;

  puts("Enter a string [100 characters maximum]: ");
  char *input_str = (char*)(malloc(100*sizeof(char)));
  scanf("%s", input_str);
  
  if (pipe(fd) < 0){
    puts("error during pipe");
    exit(0);
  }else if (pipe(fd2) < 0){
    puts("error during pipe");
    exit(0);
  }
  
  pid = fork();

  if (pid == 0){
    printf("%s\n", "We are in the child!");
    printf("%s%d\n", "Child ID: ", pid);
    close(fd[1]); //no writing
    
    char *cat_str = (char*)malloc(100*sizeof(char));
    
    read(fd[0], cat_str, 100);
    strcat(cat_str,"...and thanks for coming to my TED talk.");
    close(fd[0]);
    close(fd2[0]);

    write(fd2[1], cat_str, strlen(cat_str)+1);
    close(fd2[1]);
     

  }

  else if (pid > 0 ){

    char *cat_str = (char*)malloc(100*sizeof(char));

    printf("%s\n", "We are in the parent!");
    printf("%s%d\n", "Parent ID: ", pid);
    close(fd[0]); //close reading end
    write(fd[1], input_str, strlen(input_str)+1);
    close(fd[1]);

    wait(NULL);
    close(fd2[1]);
    read(fd2[0], cat_str, 100);

    printf("%s%s\n", "STRING IS: ", cat_str);
    close(fd2[0]);
    
  }

  return 0;
}
