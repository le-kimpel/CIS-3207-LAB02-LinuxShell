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
-still need to stick a batch file in
-give "<<" and ">>" functionality
-...fucking parallel commands???

BUGS:


*/

//function prototypes
char **str_to_array(char *str);
q *tok_pipes(q *parent_list, char **str, int *index);
q *str_to_linkedlist(char **str);
q *pipe_cmd(q *parent_list, char **str, int *index);
void print_str(char **str);
void print_q(q *command_list);
void handle(q *pipe_list);
void handle_internal_cmd(q *command);
void in_redirect_cmd(char *FILENAME); 
void out_redirect_cmd(char *FILENAME);
void handle_redirects(q *command);
int check_IO_redirects(q *command);

char **env;

int main(char *c, char**argv, char **environ){

  //size of an input string
  char *input = (char*)malloc(256*sizeof(char));  
  
  //while true
  while(1){
    
    //working directory
    printf("%s%s%s", "MyShell:", getenv("PWD"), ">" );
    
    int len = 100;
    fgets(input,len,stdin);
    
    //print str for debugging
     printf("%s\n", input);
     
    //forking
    int pid = fork();
     
    //parent process
    if (pid > 0){

      if (strcmp(input, "quit\n") == 0){
	exit(0);
      }
    
      if (input[strlen(input)-2] != '&'){
	
	int status = 0;
	wait(&status);
	printf("%s%d\n", "Child executed with status ID of ", status);

      }

       //child process
    } else if (pid == 0){
       
      //setting environment variable
      env = environ;
       
      // converting input to 2d array
      char **temp = str_to_array(input);
       
      // converts 2d array to linked list
      q *temp2 = str_to_linkedlist(temp);

      //check for valid redirect(s)
      if (check_IO_redirects(temp2) == 0){
	puts("ERROR: Ambiuous redirects");
	return -1;
      }

      int index = 0;
      
      //initiate recursive tokenization (pipes first)
      pipe_cmd(temp2, temp, &index);
       
    } 
    
  }
  
  return 0;
}


int check_IO_redirects(q *command){


  //detecting input/output carrots
  int IN_FLAG = 0;
  int OUT_FLAG = 0;
  int CAT_FLAG = 0;
  
  //check for pipe
  int pipe_temp_count = 0;
 
  //tracker variables
  int was_pipe = 0;
  int was_in = 0;
  int was_out = 0;
  int was_cat = 0;

  
  //counter variables
  int i = 0;
  int j = 0;
  int pipe_count = 0;
  int in_count = 0;
  int out_count = 0;
  int cat_count = 0;
  
  while(get(command, i)!=NULL){

    char *str = get(command, i);


    //sets out flag to 1 upon discovery of output redirect
    if (strcmp(str, ">>") == 0){
      cat_count++;
      CAT_FLAG = 1;
    }
    
    
    //sets out flag to 1 upon discovery of output redirect
    if (strcmp(str, ">") == 0){
      out_count++;
      OUT_FLAG = 1;
    }
    
    //sets in flag to 1 upon discovery of input redirect
    if (strcmp(str, "<") == 0){
      in_count++;
      IN_FLAG = 1;
      
      //cannot have input redirect after an output redirect
      if (OUT_FLAG == 1 || CAT_FLAG == 1){
	return 0;
      }
      
    }
    
    //check for pipes and find the total number of pipes
    if (strcmp(str, "|") == 0){
      pipe_count++;
    }
    
    i++;
    
  }
  
  //bad argument if we have more than one out/in redirect
  if (out_count > 1 || in_count > 1 || cat_count > 1){
    return 0;
  }

  if (cat_count == 1 && out_count == 1){
    return 0;
  }
  
  //no pipes: no ambiguous piping to be checked
  if (pipe_count == 0){
    return 1;
  }
  
  while (get(command, j)!=NULL){

    char *str = get(command, j);
 
    //if we have not seen all pipes 
    if (strcmp(str, ">") == 0 || strcmp(str, ">>") == 0){
      was_out = 1;
      
      if (pipe_temp_count != pipe_count){
	return 0;
      }
      //no arguments between pipe and redirect
      if (was_pipe == 1 || was_in == 1){
	return 0;
      }
    }
    
    //cannot have an input redirect after a pipe
    if (strcmp(str, "<") == 0){
      was_in = 1;
      
      if (pipe_temp_count != 0){
	return 0;
      }
      
      //no arguments between pipe and redirect
      if (was_pipe == 1 || was_out == 1){
	return 0;
      }
    }
    //testing for consecutive pipes
    if (strcmp(str, "|") == 0){    
      pipe_temp_count++;
      if (was_pipe == 1 || was_in == 1 || was_out == 1){
	return 0;
      }else{
	was_pipe = 1;
      }
    }else{
      was_pipe = 0;
    }
    //if we are not an output redirect
    if (strcmp(str, ">") != 0 || strcmp(str, ">>") !=0){
      was_out = 0;
    }
    //cannot have an input redirect after a pipe
    if (strcmp(str, "<") == 0){
      was_in = 0;  
    }
    
    j++; 

  }
 
  return 1;
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
    //print_q(pipe_list);
     handle_redirects(pipe_list);
     handle_internal_cmd(pipe_list);
   }
}

//supposedly handles the pipes

q *pipe_cmd(q *parent_list, char **str, int *index){

  // print_q(parent_list);
  //produce a list that handles up to first end of pipe
  q *pipe_list = (tok_pipes(parent_list, str, index));

  print_q(pipe_list);
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
    // puts("parent");
    // print_q(pipe_list);
    // print_q(parent_list);
    handle(pipe_list);
    return 0;
  }
  
  //process ID
  int pid = fork();
  
  //child process handles the pipe
  if (pid == 0){

    //puts("child");
    //    print_q(pipe_list);
    
    close(fd[READ]);
    
    dup2(fd[WRITE], 1);
    
    handle(pipe_list);

    //parent process takes the parent list
  }else if (pid > 0){

    wait(NULL);
    
    close(fd[WRITE]);
    
    dup2(fd[READ], 0);
    
    char *args[] = {NULL};

    // puts("parent check");
    // print_q(parent_list);
    pipe_cmd(parent_list, str, index);
    
  }
  
}


//function sort through a **str and return a list of elements to recursively pipe
q *tok_pipes(q *parent_list, char **str, int *index){
  
  //counter
  int i = *index;
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

  *index = i+1;
  //dequeue to get rid of extra pipe character
   dequeue(parent_list);
  
   // print_q(pipe_list);
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
  //print_str(args);

  //checking failure
  int res = menu(args, env);
    
  //checks to see if menu for internal commands returns failure
  if(res == 1){
     execvp(get(command,0), args);
  }
}

void handle_redirects(q *command){

  //metrics to handle finding redirects within a string in the data structure
  int i = 0;
  int IN_FLAG = 0;
  int OUT_FLAG = 0;
  int CAT_FLAG = 0;
  
  //tracker variables
  int in_index = 0;
  int out_index = 0;
  int cat_out_index = 0;
  
  //while each string is not null
  while (get(command, i)!=NULL){

    //if we find an in-redirect
    if (strcmp(get(command,i), "<") == 0){
      in_index = i+1;
      IN_FLAG = 1;
     
      
      //if we find an out-redirect
    }else if (strcmp(get(command,i),">") == 0){
      out_index = i+1;
      OUT_FLAG = 1;
     

    }else if (strcmp(get(command, i), ">>") == 0){
      cat_out_index = i+1;
      CAT_FLAG = 1;
    }
      

    i++;

 }

  //chooses whether to redirect inward or outward depending on which flag is active
  if (IN_FLAG == 1){
    in_redirect_cmd(get(command,in_index));
  }
  
  if (OUT_FLAG == 1){
    out_redirect_cmd(get(command,out_index));
  }    

  if (CAT_FLAG == 1){
    cat_out_redirect_cmd(get(command, cat_out_index));
  }
}




