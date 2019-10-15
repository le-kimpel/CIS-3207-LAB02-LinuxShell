CIS 3207 LAB02: INTRODUCTION TO A LINUX SHELL

NAME: Lauren Kimpel
tuID: tug60017 / 915354707
Dr. Tamer Aldwairi

-----------------------------------------------------------------------
				README
-----------------------------------------------------------------------

TABLE OF CONTENTS:

(1) Navigating The Shell Environment
(2) I/O Redirect Management
(3) Parallel and Background Execution
(4) Internal Commands Menu

-----------------------------------------------------------------------

(1) - NAVIGATING THE SHELL ENVIRONENT

Welcome to MyShell. We can launch this program by typing the following
commands into an ordinary Linux terminal:

(1) make 
(2) ./str

This will generate, from a makefile (entitled makefile) with the associated binaries,
an executable version of the shell.
Upon success, the shell should prompt the user as follows:

MyShell>[current working directory]>

So, if my current working directory is /home/TU/Lauren's_Stuff/parent_dir, then the
shell will output

MyShell:/home/TU/Lauren's_Stuff/parent_dir>

if startup is successful.

Alternatively, we can also run program commands in the shell using a batch file.
Only one batch file is permitted. More than one batch file being executed will result
in failure. To execute a batch file, we should perform the following commands:

(1) make
(2) ./str batch.txt
This will then reproduce all of the commands and their output from this batch file.

To exit the shell, enter "quit" into the terminal.

-----------------------------------------------------------------------
HOW TO FORMAT PROGRAM INPUT
-----------------------------------------------------------------------

!WARNING!
This shell determines command types by WHITESPACE. This includes command characters
such as '|', '>', '<', '>>', etc. ANYTHING THAT HAS NOT BEEN SEPARATED BY WHITESPACE
WILL NOT BE RECOGNIZED!

EXAMPLES OF VALID INPUT:

cd ../

./test | ./test | ./test | ./test

echo 1 > in.txt

cmd1 & cmd2 args1 & args2 cmd3 & args3

cat out.txt

EXAMPLES OF INVALID INPUT:

cmd1&cmd2

./test| ./test

echo 1>in.txt

cmd1|cmd2

out.txt>>in.txt

-----------------------------------------------------------------------
(2) I/O REDIRECT MANAGEMENT

-----------------------------------------------------------------------
COMMAND MANUAL
-----------------------------------------------------------------------
"<" MANUAL
    
NAME

"<" - An input redirect operator

SYNOPSIS

someProgram.c < someFile.txt

DESCRIPTION

Redirects input from stdin. This is done without alerting the program to this
redirection. To redirect input, we alter the page table of a program to read from
a file using its FILENAME as a char*. We use the system call dup2 to overwrite stdin.
Prints an error message upon failure, but does not exit MyShell.

">" MANUAL

NAME

">" - An output redirect operator

SYNOPSIS

someProgram.c > someFile.txt

DESCRIPTION
Redirects output from stdout. This is done without alerting the program to this
redirection. To redirect output, we alter the page table of a program to write to a
file using its FILENAME as a char*. We use the system call dup2 to overwrite stdout.
As a result, any calls to printf() will be directed to this file and not the
terminal. Prints an error message on failure, but does not exit MyShell.

"|" MANUAL

NAME

"|" - pipes the input of one program to the output of another

SYNOPSIS

program1 | program2

program1 | program2 | program3 | .... programN

DESCRIPTION

The "|" operator is used to pipe the input of one program to the output of another.
This means that whatever is outputted by program1 will be read in by program2's
calls to read() or scanf(). 
This is accomplished by calling fork() and having the resulting child handle its
assigned input. We call pipe() on the correct file descriptor to initiate the pipe.
The parent waits for its child to finish before managing the rest of
the input, post-pipe. Multiprocessing is accomplished using this strategy, if we wish
to share input and output data among programs. You cannot pipe to a text file in this
implementation of MyShell, because it is not executable. 
It is possible for a user to pipe indefinitely.

">>" MANUAL

NAME

">>" - concatenated output redirect

SYNOPSIS

someProgram.c >> someProgram.txt

DESCRIPTION

The ">>" command operates similarly to the ">" command. If a file does not exist,
executing this command will create a file and write the output of someProgram.c to
this file. In this implementation, ">>" will concatenate to a file if it already
exists, but will not overwrite it.


-----------------------------------------------------------------------
(3) PARALLEL AND BACKGROUND EXECUTION
-----------------------------------------------------------------------
COMMAND MANUAL
-----------------------------------------------------------------------

"&" MANUAL

NAME

"&" - Signifies for a program to run in the background

SYNOPSIS

cmd1 &

./test | ./test &

DESCRIPTION

Allows for a program execution to run in the backround inside of the shell.
This means that, once a program is forked, the parent process, instead of waiting
for its child to complete (to ensure deterministic output), will continue running.


"&" MANUAL - (2)

NAME

"&" - Parallel processing operator

SYNOPSIS

[cmd1] & [cmd2] [args1] & [args2] [cmd3] & [args3]

DESCRIPTION

"&", when placed between two programs and formatted as above (with a maximum of
3 commands and 3 args), will force all child processes to run without having the
parent wait until all processes are started. Thus, all commands will be executed in
parallel, and will run simultaneously in the shell until finished.

-----------------------------------------------------------------------
(4) INTERNAL COMMANDS MENU
-----------------------------------------------------------------------
COMMAND MANUAL
-----------------------------------------------------------------------

CD(PATH) USER MANUAL
NAME
cd - changes the current working directory within the shell by using either
an absolute or relative path.

SYNOPSIS

cd [PATH]

EXAMPLES

Let the current working directory be parent_dir. 
The following can occur:

MyShell~/parent_dir>: cd child_dir
MyShell~/parent_dir/child_dir>: cd ../ (relative path)
MyShell~parent_dir>:

DESCRIPTION

The "cd" command will change the current working directory of the shell to a
user-specified path. This path can be an absolute or relative path. 
Relies on chdir() as well as realpath(). 

DIR MANUAL

NAME

dir [PATH] - Gives a list of all files under the current or specified directory

SYNOPSIS

dir [PATH] 

DESCRIPTION

Calls to dir will provide a list of all files under the current directory if null.
If PATH has an argument, then, if this PATH is deemed by the shell to be legitimate,
dir will print all files under the specified directory (but will not change
directories.) Relies on the realpath() system call.
 

ECHO MANUAL

NAME

echo [statement]

SYNOPSIS

echo [statement]  - prints the statement to the terminal (if not redirected)

DESCRIPTION

echo will print a user specified string or value to the terminal.


QUIT MANUAL

NAME

quit - Quits the shell

SYNOPSIS

quit

DESCRIPTION

quit will immediately exit the shell.

PAUSE_A MANUAL

NAME

pause_a - Pauses the shell until the user presses Enter

SYNOPSIS

pause_a

DESCRIPTION

One of the most useless commands in the entire shell. Pauses the shell until
the Enter key is pressed. Zero useful functionality.

CLS MANUAL

NAME

cls - Clears the current screen

SYNOPSIS

cls

DESCRIPTION
Clears the current screen.


HELP MANUAL

NAME

help - prints this manual

SYNOPSIS

help

DESCRIPTION

Prints the MyShell manual.


ENVIRON MANUAL

NAME

environ - Prints the current environment in the shell.

SYNOPSIS

environ

DESCRIPTION

Prints the environment variables operating within the shell. 

