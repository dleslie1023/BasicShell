****************************README shell v1******************************

***AUTHOR***
Dave Leslie

****What is it?****
A simple shell program that can be used on linux.

****What does it do?****
Executes basic commands with their paramaters as defined by a given
command. Also allows the execution of commands with outputs/inputs
to/from a designated file following the ">"/"<" character. Pipes can
be implemented in the same way by separating two commands by the "|"
symbol.

****How do I use it?****
Almost like any other shell: type in a command and its parameters, press enter
and like magic the command is executed (only without the magic). The command
"cd" followed by a path name can be used to change directories and the command
"exit" can be used to close the shell. Appending an "&" on to the end of any
command will cause that command to run without the shell waiting for its
termination.

****Limitations****
-The shell can only handle one file redirection OR pipe. Not both and not multiple
 of either.
-Any use of the ">" operator will overwrite the contents of a designated file. The
 ">>" operator is not available to this program.
-The default path is where the command is located.

****Bugs/Known Issues****
When using "&" at the end of a command, should that command print to the console,
its output will be in line with the "shell>" prompt. This will not rectify until a 
command that does not produce output is called.
