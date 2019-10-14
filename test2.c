#include <stdio.h>
#include <unistd.h>




int main(){

  printf("%d\n", 1);


  int j = 0;
  
  /*  for (int i = 0; i<100000000000000; i++){

    //    printf("Hello [%d]\n", i);
    j++;
  }
  */

  sleep(10);
  puts("test complete");

  FILE *filePtr = fopen("test2.txt", "w");

  fprintf(filePtr, "luke is annoying\n;

"); 
}
