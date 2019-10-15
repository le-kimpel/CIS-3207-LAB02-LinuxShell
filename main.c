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
-implement parallel commands

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
void cat_out_redirect_cmd(char *FILENAME);
void handle_redirects(q *command);
int check_IO_redirects(q *command);
int is_parallel(q *command, int * count);
q** format_parallel(q*command, int * count);
char **env;

int main(char *c, char**argv, char **environ){

  //batch file
  if (argv[1]!=NULL){
    in_redirect_cmd(argv[1]);
  }
  
  //size of an input string: default is 256
  char *input = (char*)malloc(256*sizeof(char));  
  
  //while true
  while(1){
    
    //prints shell prompt with updated working directory if not batch fil
    
    //print the entered command for batch file only
    if (argv[1]!=NULL){    
      int len = 100;
      fgets(input,len,stdin);
     
      printf("%s%s%s", "MyShell:", getenv("PWD"), ">" );
      printf("%s\n", input);
       
    }else{
      printf("%s%s%s", "MyShell:", getenv("PWD"), ">" );
      int len = 100;
      fgets(input,len,stdin);
      
    }

    // check for quit
    if (strcmp(input, "quit\n") == 0){
      exit(0);
    }

    // set wait flag
    int wait_flag = 1;
    if (input[strlen(input)-2] == '&'){
      wait_flag = 0;
      input[strlen(input)-2] = '\n';
      input[strlen(input)-1] = '\0';
    }
    
    //check for valid cd in order to prioritize correct redirection
    char input_copy[strlen(input)+1];
    strcpy(input_copy, input);
   
    //copying into linkedlist for simplicity
    q* cpy = initialize_queue();
    char **args = str_to_array(input_copy);
    cpy = str_to_linkedlist(args);
     
    //if we have an instance of cd()
    if (strcmp(get(cpy, 0), "cd") == 0){
      //cd cannot have more than 1 other arg
      if (cpy->size > 2){
	printf("ERROR: Invalid cd command, cannot have more than one arg\n");
	//cd with 1 arg
      }else if (cpy->size == 2){
	cd(get(cpy,1));
	//cd with no args
      }else{
	cd(".");
      }
      continue;
    }

    int count = 0;
    int should_parallel = is_parallel(cpy, &count);

    printf("should parralel [%d] : count [%d]\n", should_parallel, count);
    if(should_parallel == -1){
      printf("ERROR: Invalid input\n");
      continue;
    }
    
    q** q_array;
    q_array = format_parallel(cpy, &count);

    for(int i = 0; i <= count; i++){
      print_q(q_array[i]);
    }
   
  
    char *input_array[count+1];
    int malloc_flag = 0;
   
    for (int i = 0; i<=count; i++){
      input_array[i] = (char*)malloc(200*sizeof(char));

      if(input_array[i] == NULL){
	puts("Failed to allocate memory for the input string");
	malloc_flag = 1;
	break;
      }

      
      for (int j = 0; j<q_array[i]->size; j++){
	strcat(input_array[i], get(q_array[i], j));
	strcat(input_array[i], " ");

      }
      
      strcat(input_array[i], "\n");
      printf("[%s]\n", input_array[i]);
    }
    
    if (malloc_flag == 1){
      continue;
    }

    for(int i = 0; i <= count; i++){
      //forking
      int pid = fork();
      
      //child process
      if (pid == 0){
	
	//setting environment variable
	env = environ;
	
	//converting input to 2d array
       	char **temp = str_to_array(input_array[i]);
	
	//converts 2d array to linked list
	//q *temp2 = str_to_linkedlist(temp);

	q *temp2 = q_array[i];
	
	//check for valid redirect(s)
	if (check_IO_redirects(temp2) == 0){
	  puts("ERROR: Ambiuous redirects");
	  return -1;
	}
	
	int index = 0;
	
	//initiate recursive tokenization (pipes first)
	pipe_cmd(temp2, temp, &index);
	exit(0);
      }
    }

    if (wait_flag == 1){
      for(int i = 0; i <= count; i++){
	int status = 0;
	wait(&status);
      }
    }
  }
  return 0;
}

q** format_parallel(q*cmds, int *count){
  //assume have at least 1 & (2 commands)
  q** inputs;

  //if no &, return just the command
  if(*count == 0){
    inputs = (q**)malloc(sizeof(q*) * ((*count+1)));
    inputs[0] = cmds;
    return inputs;
  }else{
    //create an array of q inputs 
    inputs = (q**)malloc(sizeof(q*) * ((*count)));    
  }
  
  if(inputs == NULL){
    printf("ERROR: Failed to malloc Q array");
  }
  
  //initialize our queues
  inputs[0] = initialize_queue();
  inputs[1] = initialize_queue();

  //construct command 1
  enqueue(inputs[0], get(cmds, 0));
  enqueue(inputs[0], get(cmds, 3));

  //construct command 2
  enqueue(inputs[1], get(cmds, 2));
  enqueue(inputs[1], get(cmds, 4));

  if (*count == 2){
    //if 2 &, construct a third queues
    inputs[2] = initialize_queue();

    //construct command 3
    enqueue(inputs[2], get(cmds, 6));
    enqueue(inputs[2], get(cmds, 7));
  }

  //return our queue array
  return inputs;
}

//checks for other ampersands aside from a terminating &
int is_parallel(q *cmds, int *count){

  int i = 0;
  int redirect_flag = 0;
  while (get(cmds, i)!=NULL){

    //count the number of ampersands in the command (max 3)
    if (strcmp(get(cmds, i), "&") == 0){
      (*count)++;
    }
    //check for invalid inputs
    if (strcmp(get(cmds, i), "|") == 0 || strcmp(get(cmds, i), ">") == 0 ||
	strcmp(get(cmds, i), "<") == 0 || strcmp(get(cmds, i), ">>") == 0){
      redirect_flag = 1;
    }
      
    i++;
  }
  //valid; no ampersands
  if(*count == 0){
    return 0;
  }
  
  //invalid input
  if (redirect_flag == 1 && (*count) > 0){
    return -1;
  }

  //invalid input
  if ((*count) != 1 && (*count) != 2){
    return -1;
  }

  //checks for valid input length
  if(cmds->size != 5 && cmds->size != 8){
    return -1;
  }

  //checks for correct location of &
  if(strcmp(get(cmds, 1), "&") != 0){
    return -1;
  }

  //checks for correct location of &
  if((*count) == 2 && strcmp(get(cmds,5), "&") != 0){
    return -1;
  }

  //all conditions hold for valid parallelism
  return 1;
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

  //can't begin or end with a pipe (no pipes to nowhere)
  if (strcmp(get(command, command->size-1), "|") == 0 ||
      strcmp(get(command, 0), "|") == 0){
    return 0;
  }
  
  while(get(command, i)!=NULL){

    char *str = get(command, i);

    //bad redirects in general
    if (strcmp(str, "<>") == 0 || strcmp(str,"><") == 0){
    return 0;
  }
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

//handles the current elements of the list(s)
void handle(q *pipe_list){
  
  //if command is preprocessed and valid, redirect as necessary
  if (get(pipe_list, 0)!=NULL){
     handle_redirects(pipe_list);
     handle_internal_cmd(pipe_list);
  }
}

q *pipe_cmd(q *parent_list, char **str, int *index){

  //produce a list that handles up to first end of pipe
  q *pipe_list = (tok_pipes(parent_list, str, index));

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
    return 0;
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
  //checking failure
  int res = menu(args, env);

  if (res == 0){
    exit(0);
  }
  
  //if we quitted:
  if (strcmp(args[0], "quit") == 0){
    quit();
  }

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




