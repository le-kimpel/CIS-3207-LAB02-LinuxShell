#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedlist.h"

char **str_to_array(char *str);
q *tok_pipes(char **str);
q *tok_redirects(char **str);
q *str_to_linkedlist(char **str);
void print_str(char **str);
void print_q(q *command_list);

int main(){

  char *input = (char*)malloc(100*sizeof(char));
  printf("%s\n", "Enter a string:");

  int len = 100;
  fgets(input,len,stdin);
  char **temp = str_to_array(input);
  q *temp2 = tok_pipes(temp);
  
  

}

//converts a char * into char ** separated by whitespace
char **str_to_array(char *str){

  char **tokenized = (char**)malloc(100*sizeof(char**));
  int str_index = 0;
  char *token = strtok(str, " ");

  while(token!=NULL){

    tokenized[str_index++] = token;
    token = strtok(NULL, " ");

  }
  
   return tokenized;
 
}

//tokenizes and sorts in-redirects and out-redirects
q *tok_redirects(char **str){

  





}

//function sort through a **str and return a list of elements to recursively pipe
q *tok_pipes(char **str){

  int i = 0;
  q *pipe_list = initialize_queue();
  
  while (str[i]!=NULL){
    
    if (strcmp(str[i],"|")==0){

      //enqueue the first end
      enqueue(pipe_list, str[i-1]);
      //enqueue the last end
      enqueue(pipe_list, str[i+1]);

    }
    
    i++;

  }
  print_q(pipe_list);
  return pipe_list;
  

}

//print to string for debugging purposes
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

//function to print the list for debugging
void print_q(q *command_list){
  
  for (int i = command_list->size; i>0; i--){
    
    char *entry = dequeue(command_list);   
    
    printf("%s%s%s","<", entry,">");
  }
  
  puts("");
}


//testing area for a hypothetical conversion to linkedlist
q *str_to_linkedlist(char **str){

  q * command_list = initialize_queue();
  int i = 0;
  while(str[i]!=NULL){
    enqueue(command_list, str[i]);
    i++;
  }
  
  
}
