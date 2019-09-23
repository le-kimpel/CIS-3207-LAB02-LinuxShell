#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
//file to test inbuilt functions

/* 
TO DO: 
help
learn how to fork, exec, pipe

*/

//easiest goddamn function to write
void cls ();
//exits the shell
void quit();
//pauses the shell until "Enter" is pushed
void pause_a();
//echoes the most recently printed statement
void echo(char *c);
//prints the environment variables
void environ(char ** env);
//prints the files under the current directory
void dir(char *argv);
//changes the current directory
int  cd(char *argv);
//gets the help manual
void help(char *argv);

int main(char * argc, char ** argv, char **env){
 
  environ(env);
  
  
  return 0;
}


void cls(){
  for (int i = 0; i< 100; i++){
    puts("");
  }
}

  
void quit(){
  exit(0);
}

//not even sure if this works atm
void pause_a(){
  char ENTER = '\n';
  char input;
  
  while(1){
    scanf("%c", &input);
    if (input == ENTER){
      break;
    }
  }
}
  
void echo(char *input){
  printf("%s\n", input);
}

void environ(char ** env){

  int index = 0;
  
  while(env[index]!= NULL){
    printf("%s\n", env[index]);
    index++;
    
  }
    
}
  
void dir(char *argv){
  
  DIR *d;
  struct dirent *dir;
  
  //if no commands attached
  if (argv == ""){
    d = opendir(".");
    
  }else{
    d = opendir(argv);
  }

  //success
  if (d){
    while((dir=readdir(d))!=NULL){
      printf("[%s]\n", dir->d_name);
    }
    
    closedir(d);

    //failure
  }else{  
    puts("An error occurred.");
  }
  
}

int cd(char *argv){

  //buffer set to maximum path size
  char *buf = (char*)(malloc(PATH_MAX));
  //the absolute path for the cd arg
  char *resolved_path = realpath(argv, buf);  
  //working directory
  char *pwd = getenv("PWD");
  
  if (resolved_path){
    //copies the resolved path into the pwd string
    strcpy(pwd, resolved_path);
   
  }else{
    puts("An error occurred. Realpath could not be performed.");
  }

  //value to be returned by chdir(pwd)
  int new_dir = chdir(pwd);

  //on failure
  if (chdir(resolved_path)!=0){
    puts("ERROR: Directory may not exist");
  }
  
  return new_dir;
  
}

void help(char *argv){





  }
