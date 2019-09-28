#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


char *tok_whitespace(char *str);
char *tok_redirect(char *str);
char **str_to_array(char *str);
void print_str(char **str);

int main(){

  char *input = (char*)malloc(100*sizeof(char));
  printf("%s\n", "Enter a string:");

  int len = 100;
  fgets(input,len,stdin);
  str_to_array(input);
  
  

}

char **str_to_array(char *str){

  char **tokenized = (char**)malloc(100*sizeof(char**));
  int str_index = 0;
  char *token = strtok(str, " ");

  while(token!=NULL){

    tokenized[str_index++] = token;
    token = strtok(NULL, " ");

  }
  
  print_str(tokenized);
  return tokenized;
 
}

char *tok_redirect(char *str){




}

void print_str(char **str){
  
  printf("%s", "{");
  int i = 0;

  while(str[i]!=NULL){
    if (str[i+1]==NULL){
      printf("%s%s", str[i], "}");
      
    }else{
      printf("%s%s", str[i], ",");
    }
    i++;
     
  }
  

}

