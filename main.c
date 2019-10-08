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

    char *args[] = {NULL};
    int pid = fork();
    
    while(1){





    }



  }
  
