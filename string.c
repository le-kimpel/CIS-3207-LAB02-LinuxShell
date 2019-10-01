#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedlist.h"
#include <sys/types.h>
#include <sys/wait.h>

//next i need to handle PIPES. LIKE TODAY
//child -> parent
//parent then goes back and handles itself once more with an updated list
//function prototypes
char **str_to_array(char *str);
q *tok_pipes(q *parent_list, char **str);
q *tok_in_redirects(q *parent_list);
q *str_to_linkedlist(char **str);
q *pipe_cmd(q *parent_list, char **str);
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
  //  q *temp2 = tok_pipes(temp);
  //tok_in_redirects(temp2);
  q *temp3 = str_to_linkedlist(temp);
  pipe_cmd(temp3, temp);
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
//currently using this method to debug the pipe method
void handle(q *pipe_list){

  //if there is a valid command in the list
  if (get(pipe_list, 0) != NULL){
    puts("Is a command");
  }
  
  //if there is a valid in_redirect in the list
  if (get(pipe_list, 1) != NULL){
    puts("Is an in-redirect");
  }
  
  //if there is a valid out_redirect in the list
  if (get(pipe_list, 2) !=NULL){
    puts("Is an out-redirect");
    
  }else{
    puts("idek how to categorize this lmao");
    
  }
    
}
//supposedly handles the pipes

q *pipe_cmd(q *parent_list, char **str){
  
  //produce a list that handles up to first end of pipe
  q *pipe_list = (tok_pipes(parent_list, str));
  
  //ints for file descriptor
  int READ = 0;
  int WRITE = 1;
  
  //file descriptor
  int fd[2];
  
  //Broken pipe
  if (pipe(fd) < 0){
    
    puts("error during pipe");
    
    exit(0);
  }
  
  //base case for recursive condition: no pipes left
  if (parent_list == NULL){

    puts("parent null, no pipes left!");

  }
  
  //process ID
  int pid = fork();

  //child process handles the pipe
  if (pid == 0){

    puts("We are in the child!");

    close(fd[READ]);

    dup2(fd[WRITE], 1);

    handle(pipe_list);

   //parent process takes the parent list
  }else if (pid > 0){

    close(fd[WRITE]);

    dup2(fd[READ], 0);

  }

}


//tokenizes and sorts in-redirects and out-redirects
q *tok_in_redirects(q *parent_list){

  //counter for parent list index
  int i = 0;

  //counter for updating parent list
  int count = 0;

  //list of in redirects
  q *in_redirect_list = initialize_queue();
  
  while (get(parent_list, i)!=NULL){
     
     if (strcmp(get(parent_list, i), ">") == 0){
      
      break;
    }
    
    enqueue(in_redirect_list, get(parent_list, i));
    
    i++;    

   }
    
   //update the parent list
   while(count < i){
     
     dequeue(parent_list);
     
     count++;
   }
     
   return parent_list;
}

//function sort through a **str and return a list of elements to recursively pipe
q *tok_pipes(q *parent_list, char **str){

  //counter
  int i = 0;
  //list of currently active pipes
  q *pipe_list = initialize_queue();
 

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

  //if there are no pipe commands
  if (get(parent_list,i) == NULL){

    parent_list = pipe_list;

  }
  
  //prints output for debugging
  //print_q(pipe_list);
  
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
    
    printf("%s%s%s","[", entry,"]");
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
