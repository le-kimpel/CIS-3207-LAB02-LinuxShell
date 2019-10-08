#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


void run_shell();

//oof, here goes
int main(){
 
  run_shell();
  //char *args[] = {NULL};
  // printf("%d\n",execv("./str", args));
    return 0;
  }
  
  
  void run_shell(){


    //   int pid = fork();
   
    char *input = (char*)malloc(256*sizeof(char));  
    char *argv[] = {input, NULL};
    
    //testing the while true loop!
   
   while(1){

     printf("%s", "MyShell>");
     int len = 100;
     fgets(input,len,stdin);

     printf("%s\n", input);

     if (strcmp(input, "exit") == 0){
       exit(0);
     }

     int pid = fork();

     if (pid == 0){
       printf("%d\n", execv("./str", argv));

     }if (pid > 0){

       int status = 0;
       wait(&status);
       printf("%s%d\n", "Child successfully executed with status ID of ", status);
              
     } 
     
   }

  }
  
