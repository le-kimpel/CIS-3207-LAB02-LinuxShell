#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>




int main(){

  //reading from a file  
  int fd1 = open("farts.txt", O_RDONLY);
  
  if (fd1!= 0){
    dup2(fd1, 0);
    
  }else{
    
    puts("ERROR: cannot form in-redirect!");

  }

  //writing to a file
  int fd = open("file.txt", O_WRONLY);
  char *args[] = {"dogs are cute!", NULL};
  
  if (fd!=0){
    dup2(fd, 1);
  }else{
    puts("ERROR: cannot form out-redirect!");
  }

  return 0;

}
