#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

//function prototypes
void run_shell();


int main(){
  
  run_shell();
  
  return 0;
}

//initiates the shell and execs appropriate commands
void run_shell(){
  
  //working directory
  char *pwd = getenv("PWD");
  //size of an input string
  char *input = (char*)malloc(256*sizeof(char));  
  //arguments
  char *argv[] = {input, NULL};
  
  //while true
  while(1){
    
    printf("%s%s%s", "MyShell:", pwd, ">" );
    
    int len = 100;
    fgets(input,len,stdin);
    
    //print str for debugging
    printf("%s\n", input);
     
     //exit command
     if (strcmp(input, "quit\n") == 0){
       exit(0);
     }
     
     //forking
     int pid = fork();
     
     //child process
     if (pid == 0){
       printf("%d\n", execv("./str", argv));
       
       //parent process
     }if (pid > 0){
       
       int status = 0;
       wait(&status);
       printf("%s%d\n", "Child successfully executed with status ID of ", status);
       
     } 
     
   }
   
  }
  
