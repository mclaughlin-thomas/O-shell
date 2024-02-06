/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

/* For the first half of the shell project, the part examined here in simple-shell.outline1, compile with:
 *    gcc simple-shell.c -o simple-shell -s
 * But for the second half of the project, described in simple-shell.outline2, you will compile with:
 *    gcc simple-shell.c CircularArray.c -o simple-shell -s
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//* For the second part of this project, you will need to include the header file for the circular array. 
//* Do so by putting this on the next line to uncomment it: #include "CircularArray.h"
//* By doing this, a global variable with be added named HistoryData that has room for 10 strings of data.  
//* However, for this first half of this project, just use one string, one item of history, labeled history down in main.
//* Thus, in this project, there should be no use of the circular array.

#define MAX_LINE    80 /* 80 chars per line, per command */

#define READ_END    0
#define WRITE_END   1

//*** This will only be needed in the second half of the program, not this one:  int const HISTORY_ITEMS = 10;

// function prototypes
int parse_command(char command[], char *args[]);
int check_redirect(char *args[], char op[], int len);
int check_pipe(char *args[], int len);
void handle_redirect_output(char *args[], int pos);
void handle_redirect_input(char *args[], int pos);
void handle_pipe(char *args[], int pos, int len);

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;         /* 1 is true in C. */
    int command_length;
    pid_t child;                // pid_t, the type for a process id, is defined in one of the header files as some type of int.

    int redirect_position;      // position of '>' character: what index is the > character at in the array    
    int pipe_position;          // position of '|' character: what index is the | character at in the array

    char command[MAX_LINE];  // the command that was entered
    char history[MAX_LINE];  // the most recent command in history

    //*** Use a history holding a single string for the first half of the project. That's what the command
    //*** string variable above is for. After you get this simple shell to work and are ready for the second
    //*** half of the project, come back to this. Save a backup of your working simple-shell.c file 
    //*** (for example by using:  cp simple-shell.c simple-shell.first). Then modify the
    //*** original simple-shell.c to use the circular array to hold up to 10 items of history.
    //*** That would entail commenting off the declaration of the history string just
    //*** above and replacing it by the use of the functions HistoryInsert, HistoryLookup, and
    //*** HistoryInit.  Make sure that all uses of the old history string are commented off
    //*** below and that you use HistoryInsert and HistoryLookup instead.  You should look at
    //*** CircularArray.c to see what these functions do and what parameters they take. The
    //*** HistoryInit function is only used once, right here before we hit the while loop
    //*** for the shell.  If you get stuck, contact Br. David.

    int count = 0;   // count indicates how many commands have been processed
    // This is the main loop for the shell interface. It allows the shell to repeatedly show the prompt, get the user's command,
    // and execute that command. Then loop around and do the same steps over and over.
    while (should_run){
        //*********************************************
        //        User I/O | Print Shell Prompt and Receive Input
        //*********************************************
        printf("(*)> "); //*** DO THIS: print your prompt here. It could be as simple as >

        fflush(stdout);

        fgets(command, MAX_LINE, stdin); //*** DO THIS: Use fgets to read into command what the user typed in. Look up fgets if you don't know it.
        //*********************************************
        //        User I/O | Print Shell Prompt and Receive Input
        //*********************************************



        //--------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------



        //*********************************************
        //        Pre-Tokenization | !! logic
        //*********************************************
        if ( (strncmp(command,"!!",2) == 0) && (count == 0) )    //*** This is a string compare function. Look up the details.
        {
            fprintf(stderr, "No command history found\n");
            continue;
        }
        else
        {
            count = 1;   //*** This is the shell's first command.
        }

        if (strncmp(command,"!!",2) != 0)
        {
            /**
             *** DO THIS: We have entered a command, so copy the command to the history string with strcpy.
             */


        }
        else
        {   //*** DO THIS:
            // We want to carry out the !!, so we use the most recent command.
            // Use printf to print the history string.
            // Then use strcpy to copy the history string to command.
            // The command will actually get executed below. See the next DO THIS for where we set this up for execution. 

        }
        //*********************************************
        //        Pre-Tokenization | !! logic
        //*********************************************




        // Break command up into tokens which are placed in args. command_length will hold the number of tokens.
        
        

    }

    return 0;
}
