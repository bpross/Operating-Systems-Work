/*Author: Benjamin Ross
 * Date: 4/24/12
 * Assignment 1 for CMPS111 with Prof. Miller
 * Basic Shell in C
 */

/*Includes*/
#include  <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/* This function is used to parse the input line from the user
 * It returns the command, arguments, pipe and redirection characters 
 * in the char array args
 */
void  parse(char *line, char **args)
{
    /* this is used to check to see if the line is terminated with a newline*/
    char *p;
    if ((p = strchr(line, '\n')) != NULL){
        *p = '\0';
    }

    /*While not the end of the line*/
    while (*line != '\0') {

        /* Replace the whitespaces with null characters. This is so every
         * command or argument is terminated with the null char as per the
         * specs
         */
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';
        
        *args++ = line;                  

        /* If the char is not whitespace it has been saved into args. Advance*/
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') 
            line++;             
    }

    /*Mark the end of the argument list so execvp will work*/
    *args = '\0';                 
}


/* This is used to execute a command with NO REDIRECTION OR PIPES */
void execute(char **argv)
{
    pid_t  pid;
    int status;

    /* Fork the process and check to see if it is successful */
    if ((pid = fork()) < 0) 
    {
        printf("ERROR: forking child process failed\n");
        exit(0);
    } 
    /* Run the command and check to see if the exec fails */
    else if (pid == 0) { 
        if (execvp(*argv, argv) < 0) {     
            printf("*** ERROR: exec failed\n");
            exit(0);
        }
    }
    else {                                 
        /* This causes the parent to wait for completion */
        while (wait(&status) != pid);
                                                              
    }
}

/* This is used to execute a command that has output redirection */

void execute_re_out(char **args, char *file)
{
    pid_t pid;
    int status;

    int out_file = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    if (out_file < 0)
    {
        printf("ERROR: Redirection failed to open file\n return value: %d\n",out_file);
        exit(0);
    }
    if ((pid = fork()) < 0)
    {
        printf("ERROR: forking child process failed\n");
        close(out_file);
        exit(0);
    }
    else if(pid == 0)
    {
        dup2(out_file,1);
        close(out_file);
        if (execvp(*args, args) < 0)
        {
            printf("ERROR: exec failed\n");
            exit(0);
        }
    }
    else
    {
        while(wait(&status) != pid);
    }
    
}

/* This is used to execute a command that has output redirection */

void execute_re_in(char **args, char *file)
{
    pid_t pid;
    int status;

    int in_file = open(file, O_RDONLY);
    if (in_file < 0)
    {
        printf("ERROR: Redirection failed to open file\n return value: %d\n",in_file);
        exit(0);
    }
    if ((pid = fork()) < 0)
    {
        printf("ERROR: forking child process failed\n");
        close(in_file);
        exit(0);
    }
    else if(pid == 0)
    {
        close(0);
        dup(in_file);
        close(in_file);
        if (execvp(*args, args) < 0)
        {
            printf("ERROR: exec failed\n");
            exit(0);
        }
    }
    else
    {
        while(wait(&status) != pid);
    }
    
}

/* This is used to check to see if there is a redirection or pipe in the
* command and send it off to be excuted by the correct function
*/

void check_for_meta(char **args)
{
    int i;
    char *file;
    int found = 0; 
    for(i=0;i < 64 && args[i] != NULL; ++i)
    {
        if(strcmp(args[i], ">") == 0)
        {
            file = args[i+1];
            args[i] = '\0';
            args[i+1] = '\0';
            execute_re_out(args, file);
            found = 1;
        }
        else if(strcmp(args[i], "<") == 0)
        {   
            file = args[i+1];
            args[i] = '\0';
            args[i+1] = '\0';
            execute_re_in(args, file);
            found = 1;
        }
        else if(strcmp(args[i], "|") == 0)
        {
            printf("Pipe detected\n");
            found = 1;
        }
    }
    if (found == 0)
        execute(args);
}


/*No arguments are givent to the program so main can be void */
void  main(void)
{
    char line[1024];             
    char *args[64];              
    /* This is the main loop that will execute commands. It only exits when
     * exit is typed in by the user
     */
    while (1) {         
        /* Print the prompt */
        printf("# ");   

        /* Read in the user entered line */
        fgets(line,sizeof(line),stdin);
        printf("\n");

        /* Send the line to be parsed */
        parse(line, args);      

        /* Check to see if the command is exit */
        if (strcmp(args[0], "exit") == 0)  
            exit(0);            
        check_for_meta(args);
    }
}
