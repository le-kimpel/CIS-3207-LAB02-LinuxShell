#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//practice file for looking at fork functionality


int main(){


  int x = 0;
  
  char *input = (char*)malloc(100*sizeof(char));
  
  printf("enter a string: \n");
  scanf("%s", input);
  
  int fd = fork();
  if (fd == 0){
    
    printf("%s%s%s\n", "The CHILD string is: ", input, " + potato");
    printf("%s%d\n", "x = ", x);
    
    
  }else if (fd > 0){
    
    printf("%s%s\n", "The PARENT string is: ", input);
    x++;
    printf("%s%d\n", "x = ", x);
    
  }else{
    puts("error");
  }

 
   return 0;
}
