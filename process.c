#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedlist.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "internal.h"

/*
  -----------------
  | UPDATE NOTES: |
  -----------------

-and start writing better error messages
-also, i need to teach it to handle ambiguous input redirects
-still need to have it run in background
-still need to stick a batch file in

BUGS:
-doesn't like to exit when told


*/

//function prototypes
char **str_to_array(char *str);
q *tok_pipes(q *parent_list, char **str);
q *str_to_linkedlist(char **str);
q *pipe_cmd(q *parent_list, char **str);
void print_str(char **str);
void print_q(q *command_list);
void handle(q *pipe_list);
void handle_internal_cmd(q *command);
void in_redirect_cmd(char *FILENAME); 
void out_redirect_cmd(char *FILENAME);
void handle_redirects(q *command);

char **env;

int main(char *c, char**argv, char **environ){

  //set input to args
  char *input = argv[0];
  
  //setting environment variable
  env = environ;
  
  // converting input to 2d array
  char **temp = str_to_array(input);
  
  // converts 2d array to linked list
  q *temp2 = str_to_linkedlist(temp);
  
  //initiate recursive tokenization (pipes first)
  pipe_cmd(temp2, temp);
  
  
  return 0;
}


//sets output to a file
void cat_out_redirect_cmd(char *FILENAME){
  
  int fd = open(FILENAME, O_CREAT|O_WRONLY|O_APPEND, 0644);

  if (fd!=0){
    dup2(fd, 1);
  }else{
    puts("ERROR: cannot form out-redirect!");
  }
  
}

//sets output to a file
void out_redirect_cmd(char *FILENAME){
  
  int fd = open(FILENAME, O_CREAT|O_WRONLY|O_TRUNC, 0644);

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
  
  //if command
  if (get(pipe_list, 0)!=NULL){
     print_q(pipe_list);
     handle_redirects(pipe_list);
     handle_internal_cmd(pipe_list);
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
  if (parent_list->size == 0){
    printf("PARENT\n");
    handle(pipe_list);
    return 0;
  }
  
  //process ID
  int pid = fork();
  
  //child process handles the pipe
  if (pid == 0){
    printf("CHILD\n");
    close(fd[READ]);
    
    dup2(fd[WRITE], 1);
    
    handle(pipe_list);
    
    //parent process takes the parent list
  }else if (pid > 0){
    
    wait(NULL);
    
    close(fd[WRITE]);
    
    dup2(fd[READ], 0);
    
    char *args[] = {NULL};
    
    pipe_cmd(parent_list, str);
    
  }
  
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
  
  char *args[100];
  
  int i = 0;

  //pump command arguments into args
  while(get(command, i)!=NULL){
    
    args[i] = get(command, i);
    i++;
  }

  //insert null-terminating character
  args[i] = NULL;
  
  //if the command is an internal command
  print_str(args);
  
  int res = menu(args, env);

  if (strcmp(get(command,0), "quit") == 0){
      exit(0);
    }
    
  //checks to see if menu for internal commands returns failure
  if(res == 1){
    printf("%d\n", execvp(get(command,0), args));
  }
}

void handle_redirects(q *command){

  //metrics to handle finding redirects within a string in the data structure
  int i = 0;
  int IN_FLAG;
  int OUT_FLAG;

  //while each string is not null
  while (get(command, i)!=NULL){

    //if we find an in-redirect
    if (strcmp(get(command,i), "<") == 0){
      OUT_FLAG = 0;
      IN_FLAG = 1;
      break;
      
      //if we find an out-redirect
    }else if (strcmp(get(command,i),">") == 0){
      IN_FLAG = 0;
      OUT_FLAG = 1;
      break;

    }

    i++;

 }

  //chooses whether to redirect inward or outward depending on which flag is active
  if (IN_FLAG == 0){
    in_redirect_cmd(get(command,i+1));
  }else if (OUT_FLAG == 0){
    out_redirect_cmd(get(command, i+1));
  }    
}
