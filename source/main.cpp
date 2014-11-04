/* 
 * File:   main.cpp
 * Author: Dave Leslie
 * Class:  CIS3207
 * 
 * This program produces a simple Linux shell. This shell is
 * able to handle I/O redirection involving one program and one file,
 * and it can handle a pipe between two files. Arguments can consist
 * of up to 50 words counting spaces. Any command containing a "&"
 * before terminating will execute without delaying the shell. The command
 * "cd" followed by the path name can be used to change directories; the file
 * must be located in the current directory or in a directory designated by
 * the $PATH variable. 
 * 
 * The program was split into four, looping stages: input, parsing, execution, 
 * reset. The input stage would obtain the users command which would then be 
 * examined and parsed into an array, delimited by the " " character. Next the 
 * command would be executed based on the type of command (input redirection,
 * output redirection, pipe) with a parameter for each function to indicate
 * whether the shell would wait for a process or not. Finally, the array(s) used
 * to hold commands would be freed/NULL'd and the program would be ready for the
 * next command. This loop would be indefinite until termination.
 * 
 * The program is split up into a main.cpp (holding the
 * structure of the program) and sh_functions.cpp holding functions called by
 * main.cpp. These files are linked together with header.h that contains
 * skeletons of the functions in sh_functions.cpp.
 * 
 * 
 * LIMITATIONS:
 *      -The program is only capable of handling one redirection or pipe per
 *       command. Any more will result in errors by a called command or their
 *       exclusion entirely.
 *      -The default path will be wherever the shell is run from though "cd"
 *       can be used to change directories.
 * 
 * BUGS:
 *      A "&" symbol at the end of a command will cause any output by that
 *      command to directly follow the "shell>" prompt.
 *

 */

#include "header.h"

using namespace std;
const int MAX_ARGS = 50;

//parse input string to argv, removing spaces
//return number of elements in argv


int main(){
    
    string input;
    int argc;
    char *argv[MAX_ARGS], *cmd[2][MAX_ARGS];
    bool stop;
    RedirPipe check;
    while(true){
        char cwd[1024];
	char usr[1024];
	getlogin_r(usr, sizeof(usr));
	getcwd(cwd, sizeof(cwd)); 
        cout << usr << " " << cwd << ">";
       
        getline(cin, input); 
        
        if(input.compare("exit") == 0)
            return 0;
        if(input.find("&") != -1)
            stop = false;
        else
            stop = true;
        argc = check_str(input, argv);
        
       
       
        if(strcmp(argv[0], "cd") == 0){
            if(chdir(argv[1]) != 0)
                cout << "could not find directory" << endl;
            else
                cout << "directory changed: " << argv[1] << endl;
        }
        else{
            check = parse(argc, argv, cmd[0], cmd[1]);
            if(check == I_REDIR || check == O_REDIR){
                redir_proc(cmd[0], cmd[1], stop, check);
            }
            else if(check == PIPE)
                pipe_proc(cmd[0], cmd[1], stop);
            else
                create_proc(argv, stop);

        }
     
        stop = true;
        
        //clears argv, cmd_1, and cmd_2
        for(int i=0; i<20 && argv[i] != NULL; i++){
            free(argv[i]);
            cmd[0][i] = NULL;
            cmd[1][i] = NULL;
        }
    }
    return 0;
}
    


