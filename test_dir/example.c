#include <stdio.h>
#include <unistd.h>

int global = 0;
int main(){

  pid_t childPID;
  int var_lcl = 0;
  childPID = fork();

  //successful fork
  if (childPID >=0){
    //child process
    if (childPID ==0){
      var_lcl++;
      global++;
      printf("\n Child Process ::
var_lcl = [%d], global[\%d]\n", var_lcl, global);
    }else{ //parent process
      var_lcl = 10;
      global = 20;
      printf(\n" Parent process :: var_lcl = global[\%d]\n", var_lcl, global);
    }
    else{
      //fork failed
      printf("\n Fork faled, quitting now\n");
      return 1;
    }
  return 0;
}
