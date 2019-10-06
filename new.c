#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedlist.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
  -----------------
  | UPDATE NOTES: |
  -----------------
-now i need to handle redirects accordingly
-and start writing better error messages
*/

//function prototypes
char **str_to_array(char *str);
q *tok_pipes(q *parent_list, char **str);
q *tok_in_redirects(q *parent_list);
q *str_to_linkedlist(char **str);
q *pipe_cmd(q *parent_list, char **str);
void print_str(char **str);
void print_q(q *command_list);
void handle(q *pipe_list);
void handle_internal_cmd(q *command);
void in_redirect_cmd(char *FILENAME); 
void out_redirect_cmd(char *FILENAME);

//main method for testing
int main(){
  
  char *input = (char*)malloc(100*sizeof(char));
  printf("%s\n", "Enter a string:");
  int len = 100;
  fgets(input,len,stdin);
  char **temp = str_to_array(input);  
  q *temp2 = str_to_linkedlist(temp);
  pipe_cmd(temp2, temp);
 
}

//sets output to a file
void out_redirect_cmd(char *FILENAME){

  int fd = open(FILENAME, O_WRONLY);

  if (fd!=0){
    dup2(fd, 1);
  }else{
    puts("ERROR: cannot form out-redirect!");
  }
  
}

//sets input from a file
void in_redirect_cmd(char *FILENAME){

  int fd = open(FILENAME, O_RDONLY);
  
  if (fd!= 0){
    dup2(fd, 0);
    
  }else{
    puts("ERROR: cannot form in-redirect!");
  }
  
}

//converts a char * into char ** separated by whitespace
char **str_to_array(char *str){
  
  char **tokenized = (char**)malloc(100*sizeof(char**));
  int str_index = 0;
  int i = 0;
  char *token = strtok(str, " ");

  while(token!=NULL){
    
    tokenized[str_index++] = token;
    i++;
    token = strtok(NULL, " ");
    
  }

  //ascribing a null teriminating character 
  int len = strlen(tokenized[str_index-1]);
  
  tokenized[str_index-1][len-1] = '\0';
  
  return tokenized;
  
}

//handles the current elements of the list prior to a pipe
//currently using this method to debug the pipe method
void handle(q *pipe_list){

  char *args[] = {NULL};

  //if command
   if (get(pipe_list, 0)!=NULL){
     handle_internal_cmd(pipe_list);
   }
  // }else if (get(pipe_list, 1)!=NULL){
    // in_redirect_cmd(get(pipe_list,1));

  // }else if (get(pipe_list, 2)!=NULL){
    // out_redirect_cmd(get(pipe_list,2));


 
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
  if (parent_list->size == 0){
    
    handle(pipe_list);
  }
  
  //process ID
  int pid = fork();
  
  //child process handles the pipe
  if (pid == 0){
    
    close(fd[READ]);
    
    dup2(fd[WRITE], 1);
    
    handle(pipe_list);
    
   //parent process takes the parent list
  }else if (pid > 0){
    
    wait(NULL);
    
    close(fd[WRITE]);
    
    dup2(fd[READ], 0);
    
    char *args[] = {NULL};
    
    handle(parent_list);
    
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

  //dequeue to get rid of extra pipe character
   dequeue(parent_list);

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
  
  int i = 0;
  
  while (get(command_list, i)!=NULL){
    char * entry = get(command_list, i);
    printf("%s%s%s", "[", entry, "]");
    i++;
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

//function to handle and verify internal commands
//as well as  execute them
void handle_internal_cmd(q *command){
  
  //exec in internal.c
  
  char *args[100];
  
  int i = 0;
 
  int pid = fork();
  
  while(get(command, i)!=NULL){
    
    args[i] = get(command, i);
    i++;
  }

  args[i] = NULL;
  
  //if the command is an internal command
  if (pid == 0){
    
    printf("%d\n", execv("./internal", args));
    
    //if the command is not an internal command
  }if (pid > 0){
   
    wait(NULL);
    
    char *args1[] = {NULL};
   
    printf("%d\n",(get(command,0), args1));

  }

}
