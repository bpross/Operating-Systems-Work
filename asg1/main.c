/* Author: Benjamin Ross
 * UCSC Username: bpross
 * Date: 04/18/2012
 * Project 1 for CMPS111
 * Assignment is to build a basic shell for the Minix environment
 */

#include <stdio.h>
#include <string.h>


char* parse(char *line)
{
    char *token = strtok(line, " ");
    return token;
}
int main(void)
{
    char input[1024];
    char *token;

    printf("Weclome to bshell!\n");
    for(;;)
    {
        printf("$bshell:");
        fgets(input, sizeof(input), stdin);
        token = parse(input);
        printf("tokens:\n");
        while (token)
        {
            printf("token : %s\n",token);
            token= strtok(NULL, " ");
        }
       return 0; 
    }

}
