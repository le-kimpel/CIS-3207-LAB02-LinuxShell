#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedlist.h"
#include <sys/types.h>
#include <sys/wait.h>

//function prototypes
char **str_to_array(char *str);
q *tok_pipes(char **str);
q *tok_redirects(char **str);
q *str_to_linkedlist(char **str);
q *pipe_cmd(q*pipelist, q*parent_list);
void print_str(char **str);
void print_q(q *command_list);
void handle(q *pipe_list);

//main method for testing
int main(){

  char *input = (char*)malloc(100*sizeof(char));
  printf("%s\n", "Enter a string:");
  int len = 100;
  fgets(input,len,stdin);
  char **temp = str_to_array(input);
  tok_pipes(temp);
  

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

//handles the current elements of the list prior to a pipe
void handle(q *pipe_list){
  
  if (get(pipe_list, 0) != NULL){
    puts("Is command");
  }
  
  if (get(pipe_list, 1) != NULL){
    puts("Is an argument");
  }
  
  if (get(pipe_list, 2) !=NULL){
    puts("Is an argument");
    
  }else{
    puts("idek how to categorize this lmao");
    
  }
  

}
//supposedly handles the pipes

q *pipe_cmd(q* pipe_list, q *parent_list){
  
  int READ = 0;
  int WRITE = 1;
  int pid = fork();
  int fd[2] = {READ, WRITE};

  //Broken pipe
  if (pipe(fd) < 0){

    puts("error during pipe");

    exit(0);
  }

  //child process handles the pipe
  if (pid == 0){

    puts("We are in the child!");

    close(fd[WRITE]);

    dup2(fd[READ], 0);

    handle(pipe_list);

   //parent process takes the parent list
  }else if (pid > 0){

    wait(NULL);

    close(fd[READ]);

    dup2(fd[WRITE], 1);

    handle(parent_list);

  }

}


//tokenizes and sorts in-redirects and out-redirects
q *tok_redirects(char **str){

  





}

//function sort through a **str and return a list of elements to recursively pipe
q *tok_pipes(char **str){

  //counter
  int i = 0;
  //list of currently active pipes
  q *pipe_list = initialize_queue();
  //list for parent process to handle every element after the first pipe
  q *parent_list = str_to_linkedlist(str);

  while (str[i]!=NULL){

    //compares at every node    
   if (strcmp(str[i],"|")==0){     
      
    break;

    }
   
   //otherwise enqueue each preceding element
    enqueue(pipe_list,str[i]);
    //dequeue the preceding elements so we are left with those post-pipe 
    dequeue(parent_list);
   
    i++;
    
  }
  
  pipe_cmd(pipe_list, parent_list);
  
  //prints output for debugging
  print_q(pipe_list);
  print_q(parent_list);
  
  return parent_list;
  

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
  
  return command_list;
}
