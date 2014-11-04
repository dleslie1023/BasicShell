/* 
 * File:   sh_functions.cpp
 * Author: Dave Leslie
 * Class:  CIS3207
 * 
 * Contains a series of functions to be used with a simple
 * shell program. These functions are utilized in tandem to
 * provide functionality to a command line shell. All functions
 * involved with parsing, creating new processes, and running new
 * processes.
 *
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "header.h"

using namespace std;

/*Params: input        the string to be parsed
 *        argv       where the command and its
 *                     parameters are to be stored 
 * 
 * Tokenizes a string using " " as a delimiter.
 * The last element is set to NULL. Tracks the size of 
 * argv using argc to mark the end of the array.
 * 
 * Return: location of the NULL element of the array.
 */
int check_str(string input, char **argv){
    char cmd[100], *tok;
    int argc;
    
    strcpy(cmd, input.c_str());
    argc = 0;
    tok = strtok(cmd, " ");
    while(tok){
        argv[argc] = (char*)malloc(strlen(tok)+1);
        strcpy(argv[argc++], tok);
        tok = strtok(NULL, " ");
    }
    argv[argc] = NULL;
 
    return argc;
    
    
}

/*Params: argc        location of NULL element
 *        argv        array of commands and params        
 *        cmd_1       for redirection, first command
 *        cmd_2       for redirection, second command
 *                    or a file
 * Checks the command array for a pipe or redirect, and, if
 * found, flags which type was found. Then the command array is 
 * split at the pipe or redirection and causes everything in the 
 * first half of the command array to be found in cmd_1 and the 
 * second half in cmd_2. Both arrays end with NULL.  If no pipe
 * or redirect is found, no change is made to any array.
 * 
 * Return: PIPE for pipe, I_REDIR for "<", O_REDIR for ">",
 *         NONE if nothing was found.
 */
RedirPipe parse(int argc, char **argv, char **cmd_1, char **cmd_2){
    RedirPipe result = NONE; //what type of redir/pipe, if any
    int cmd_spl, cnt; //where the cmds are separated
    
    for(int i=0; i<argc; i++){
        if(strcmp(argv[i], "|") == 0){
            result = PIPE;
            cmd_spl = i;
        }
        else if(strcmp(argv[i], "<") == 0){
            result = I_REDIR;
            cmd_spl = i;
        }     
        else if(strcmp(argv[i], ">") == 0){
            result = O_REDIR;
            cmd_spl = i;
        }
        else if(strcmp(argv[i], "&") == 0)
            argv[i] = NULL;
    }
    
    if (result != NONE){
        for(int i=0; i < cmd_spl; i++)
            cmd_1[i] = argv[i];


        cnt = 0;
        for(int i=cmd_spl+1; i<argc && argv[i] != NULL; i++)
            cmd_2[cnt++] = argv[i];

        cmd_1[cmd_spl] = NULL;
        cmd_2[cnt] = NULL;
    }
    return result;
}

/*Params: argv        list of commands and params
 *        stop          boolean to indicate if the
 *                      shell should wait
 * 
 * Uses fork() and execvp() to create a child that then
 * runs a program with parameters specified in argv. If
 * the file is not found, child exits with error. The shell
 * will wait for the completion of the child unless disabled
 * by stop in which case the PID of the child will be displayed.
 * 
 * Return: PID of the child
 */
int create_proc(char **argv, bool stop){
    int pid;
    
    if((pid = fork()) == 0){
       execvp(*argv, argv);
       cerr << "error: could not find file: " << *argv << endl;
       exit(1);
    }
    else{
            if(stop){
                int status = 0;
                wait(&status);
            }
            else{
                cout << "PID: " << pid << endl;
            }
       }
    
    return pid;
    
}

/*Params: cmd_1         first command with parameters
 *        cmd_2         file to be used as source of 
 *                      output/input
 *        stop          boolean to indicate if the
 *                      shell should wait
 *        redir         type of redirection: input redirection,
 *                      output redirection
 * 
 * Uses fork and execvp() to create a child process as specified by cmd_1 
 * that will execute according to the redir parameter. Redirection causes 
 * the child process to open a new stdin or stdout using a file specified 
 * by cmd_2. 
 * 
 * Return: none
 * 
 */
void redir_proc(char **cmd_1, char **cmd_2, bool stop, RedirPipe redir){

    int pid;

    if(redir == I_REDIR){   
        if((pid = fork()) == 0){
            int newstdin = open(*cmd_2, O_RDONLY);
            close(0);
            dup(newstdin);
            close(newstdin);
            execvp(*cmd_1, cmd_1);
            cout << "bad file name: " << *cmd_1 << endl;
            exit(1);
            
        }
        else{
            if(stop){
            int status = 0;
            wait(&status);
        }
        else
            cout << "pid: " << pid << endl;
        }
            
    }
    else{
        if((pid = fork()) == 0){
            int newstdout = open(*cmd_2, O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
            close(1);
            dup(newstdout);
            close(newstdout);
            execvp(*cmd_1, cmd_2);
            cout << "bad file name: " << *cmd_1 << endl;
            exit(1);
            
        }
        else{
            if(stop){
            int status = 0;
            wait(&status);
        }
            else
                  cout << "PID: " << pid << endl;
        }
    }
}
    

/*Params: cmd_1         first command with parameters
 *        cmd_2         second command with parameters
 *        stop          boolean to indicate if the
 *                      shell should wait
 * 
 *Creates a pipe that is then linked between two files forked
 * from the shell. 
 * 
 * Return: none
 */
void pipe_proc(char **cmd_1, char **cmd_2, bool stop){
    int pid, pid_2;
    int thePipe[2];
    
    if(pipe(thePipe) != 0){
        cerr << "unable to create pipe" << endl;
        return;
    }
        
  
    if((pid = fork()) == 0){
         dup2(thePipe[0], 0);
         close(thePipe[1]);
         execvp(*cmd_2, cmd_2);
         cerr << "bad file name: " << *cmd_2 << endl;
         exit(1);
     }
     else if((pid_2 = fork())== 0){
         dup2(thePipe[1], 1);
         close(thePipe[0]);
         execvp(*cmd_1, cmd_1);
         cerr << "bad file name: " << *cmd_1 << endl;
         exit(1);
     }
     else{
         if(!stop){
             cout << "pid: " << pid << endl;
             cout << "pid 2: " << pid_2 << endl;
         }
         else{
             int status = 0;
             wait(&status);
         }
     }
        
        
}
