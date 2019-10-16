#include <stdio.h>
#include <unistd.h>



//test for parallelism
//waits for 10 seconds and then outputs a success statement to a similarly named file
int main(){

  printf("%d\n", 1);

  sleep(10);
  puts("test complete");

  FILE *filePtr = fopen("test2.txt", "w");

  fprintf(filePtr, "test2 success\n"); 
}
