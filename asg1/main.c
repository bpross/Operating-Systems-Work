/*Author: Benjamin Ross
 * Date: 4/24/12
 * Assignment 1 for CMPS111 with Prof. Miller
 * Basic Shell in C
 */

/*Includes*/
#include  <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include <string.h>


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

void  execute(char **argv)
{
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {     /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } 
    else if (pid == 0) {          /* for the child process:         */
        if (execvp(*argv, argv) < 0) {     /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else {                                  /* for the parent:      */
        while (wait(&status) != pid);       /* wait for completion  */
                                                              
    }
}

void  main(void)
{
    char  line[1024];             /* the input line                 */
    char  *args[64];              /* the command line argument      */

    while (1) {                   /* repeat until done ....         */
        printf("Shell -> ");     /*   display a prompt             */
        fgets(line,sizeof(line),stdin);              /*   read in the command line     */
        printf("\n");
        parse(line, args);       /*   parse the line               */
        if (strcmp(args[0], "exit") == 0)  /* is it an "exit"?     */
            exit(0);            /*   exit if it is                */
        execute(args);           /* otherwise, execute the command */                                                                                
    }
}
