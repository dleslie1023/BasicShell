/* 
 * File:   header.h
 * Author: Dave Leslie
 * Class:  CIS3207
 *
 * Header file for a simple shell program. Contains
 * a list of the functions used by the shell's main
 * and implemented inside its function file(sh_functions.cpp).
 * More detailed descriptions of functions are available in
 * the sh_functions.cpp file.
 */
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

//type of redirection
enum RedirPipe {PIPE, O_REDIR, I_REDIR, NONE}; 

/*
 * Parses a string removing all instances of " " and "&" and places
 * the elements in a char* array. Returns the position of the end of
 * elements within the array.
 */
int check_str(string input, char **argv);

/*
 * Determines the type of redirection or pipe (if any), and splits
 * the original command *char array into two *char arrays corresponding
 * to the individual command/file. Returns the type of redirection as an
 * enumerated type RedirPipe.
 */
RedirPipe parse(int argc, char **argv, char **cmd_1, char **cmd_2);

/*
 * Forks a process given a *char array with commands, the
 * first being the name of the file. A boolean value determines
 * whether the shell (parent) will wait for the child to terminate.
 * Returns the PID of the child.
 */
int create_proc(char **argv, bool stop);

/*
 * Forks a process and either takes input or outputs to a file indicated by
 * two *char arrays (1: process and its arguments, 2: file). Requires the type
 * of redirection to be declared. A boolean value determines whether the shell
 * (parent) will wait for the child to terminate.
 */
void redir_proc(char **cmd_1, char **cmd_2, bool stop, RedirPipe redir);

/*
 * Forks a child that connects itself via a pipe to its own child process. Output
 * from the parent process (first fork) is read as input for the child (second 
 * fork).
 */
void pipe_proc(char **cmd_1, char **cmd_2, bool stop);


    
        


