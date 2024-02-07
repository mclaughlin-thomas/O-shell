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
#include <sys/stat.h> // NOTE NOTE NOTE MAY NEED TO REMOVE, ADDED TO WORK ON MY MACHINE for the premade output function w S_IRUSR and S_IWUSR

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
    int count = 0;   // count indicates how many commands have been processed
    int input_redirect_position;   // input/output redirection

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

    while (should_run){
        // This is the main loop for the shell interface. It allows the shell to repeatedly show the prompt, get the user's command,
        // and execute that command. Then loop around and do the same steps over and over.


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
        if ( (strncmp(command,"!!",2) == 0) && (count == 0) ){
            //*** This is a string compare function above

            fprintf(stderr, "No command history found\n");
            continue;
        }
        else{
            count = 1;   //*** This is the shell's first command.
        }
        if (strncmp(command,"!!",2) != 0){
            //This may be a real command that is not related to the !! process
            strcpy(history, command); // Copies value of command into history
        }
        else{
            //This may be a legitamte !! request/command, so carry out the history as command
            printf("%s", history);
            strcpy(command,history);
            
            // The command will actually get executed below. See the next DO THIS for where we set this up for execution.
            /*              May want to cehckhistory just incase it is empty, although I think it is already covered from the very first if statement with count
            
                if (strlen(history) == 0) {
                    // If there's no history yet, inform the user.
                    printf("No commands in history.\n");
                }
                else {
                    printf("Executing last command: %s", history);
                    // Copy the history command back to command for execution.
                    strcpy(command, history);
                    // The actual execution logic for the command would follow here.
                }
            
            */
        }
        //*********************************************
        //        Pre-Tokenization | !! Logic
        //*********************************************



        //--------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------



        //*********************************************
        //        Tokenizing the Command
        //*********************************************
        command_length = parse_command(command, args); // Break command up into tokens which are placed in args. command_length will hold the number of tokens.
        //*********************************************
        //        Tokenizing the Command
        //*********************************************



        //--------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------



        //*********************************************
        //        Token Logic
        //*********************************************
        if (command_length == 0){
            continue;   //*** Nothing to execute. Skip the rest of this loop body and go to the top of loop. See above.
        }

        if (strcmp(args[0],"exit") == 0){
            break;   //*** Break out of the loop and end shell if exit
        }

        if (strcmp(args[0],"!!") == 0){
            strcpy(command, history); //*** DO THIS: Use strcpy to copy the last command from history and put it into the command variable.
            command_length = parse_command(command, args); // may have to retokenize the command here if the execution logic banks on the args array post this point
        }
        if ( (redirect_position = check_redirect(args, ">", command_length)) > 0){
            //*** This section handles output redirection, as in ls -l > filename
            handle_redirect_output(args, redirect_position);
            continue;
        }
        //*********************************************
        //        Token Logic
        //*********************************************



        //--------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------



        //*********************************************
        //        Redirect Input/Output
        //*********************************************
        if ((redirect_position = check_redirect(args, ">", command_length)) > 0) {
            // output redirection
            handle_redirect_output(args, redirect_position);
            continue;
        }

        input_redirect_position = check_redirect(args, "<", command_length);
        if (input_redirect_position > 0) {
            // input redirection
            handle_redirect_input(args, input_redirect_position);
            continue;
        }
        //*********************************************
        //        Redirect Input/Output
        //*********************************************



        //--------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------



        //*********************************************
        //        Pipe Handling
        //*********************************************
        if ( (pipe_position = check_pipe(args, command_length)) > 0)
        {
            //*** The next section handles a pipe, the | symbol. For example, the command typed in and
            //*** entered might be something like:  ls | wc
            //*** That one pipes a listing of files into the wordcount program, telling you how many
            //*** words (and characters and lines) are in the output. This section is finished.
            handle_pipe(args, pipe_position, command_length);
            continue;
        }
        //*********************************************
        //        Pipe Handling
        //*********************************************



        //--------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------



        //*********************************************
        //        Forking Process
        //*********************************************
        child = fork();

        if (child == 0){
            // child process
            args[command_length] = NULL;
            execvp(args[0], args);          //*** Here is where the new command is executed.

            fprintf(stderr, "Command not found\n");  //*** If the exec worked, this message will not be seen.
            break;
        }
        else if (child > 0){
            wait(NULL); //*** DO THIS: This is the parent process. Have it wait here for the child to complete.
        }
        else{
            fprintf(stderr, "Error forking child\n");
            return -1;
        }
        //*********************************************
        //        Forking Process
        //*********************************************
    }

    return 0;
}

//******* Everything after this point is complete. You may however, have to call some of these
//******* functions at some spots in your code above. Some such calls are already present.

/**
 * Parses the command and places each token into the args array.
 * The count of the number of args is returned in the function name.
 */
int parse_command(char command[], char *args[])
{
    char *spart = strtok(command, " \n");  //*** strtok function breaks up the command string into tokens...
    int length = 0;                        //*** ...here with space or newline as separators.

    while (spart != NULL)
    {
        args[length] = spart;   // Store the current token into the args array.
        length++;
        spart = strtok(NULL, " \n");  // Get the next token from the same command string. The separators are space or newline.
    }

    return length;   // The number of tokens placed into the args array.
}

/**
 * Checks if there is file input redirection (op, short for operator, is the < or > symbol) and return its location.
 */
int check_redirect(char *args[], char op[], int len)
{
    int rv = 0;
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if ( strcmp(args[i],op) == 0)
        {
            rv = i;
            break;
        }
    }

    return rv;
}

/**
 * Handles the situation of a pipe.
 *
 * pos is the position in the args array where the '|' occurs.
 */
void handle_pipe(char *args[], int pos, int len)
{
    pid_t child_1;
    pid_t child_2;
    int fd[2];

    int i;

    child_1 = fork();

    if (child_1 == 0) {
        // child process ... soon to be a parent!
        if (pipe(fd) == -1) {
            fprintf(stderr,"CLOSE ERROR\n");
            return;
        }

        // have the child fork another child
        child_2 = fork();
        if (child_2 > 0) {
            // parent
            close(fd[READ_END]);
            close(STDOUT_FILENO);

            dup2(fd[WRITE_END],STDOUT_FILENO);

            // now exec the first command
            args[pos] = NULL;
            execvp(args[0], args);
        }
        else if (child_2 == 0) {
            // child 2
            close(fd[WRITE_END]);
            //close(STDIN_FILENO);

            dup2(fd[READ_END],STDIN_FILENO);

            // now exec the second command
            execvp(args[pos+1],&args[pos+1]);
        }
        else {
            fprintf(stderr, "Error forking child\n");
        }
    }
    else if (child_1 > 0) {
        // parent process
        wait(NULL);
    }
    else
    {
        fprintf(stderr, "Error forking child\n");
    }
}

/**
 * Handles the situation of redirected input.
 *
 * pos is the position in the args array where the '<' occurs.
 */
void handle_redirect_input(char *args[], int pos)
{
    pid_t child;
    int fd;

    child = fork();

    if (child == 0)
    {
        // child process
        args[pos] = NULL;

        fd = open(args[pos+1], O_RDONLY);

        if (fd == -1) {
            fprintf(stderr,"OPEN ERROR\n");
            exit(1);
        }

        // the file descriptor has been duped to standard input
        if (dup2(fd, STDIN_FILENO) == -1) {
            fprintf(stderr,"DUP ERROR\n");
            exit(1);
        }

        // close the original file descriptor
        if (close(fd) == -1) {
            fprintf(stderr,"CLOSE ERROR\n");
            exit(1);
        }

        execvp(args[0], args);   //*** Run the command

        fprintf(stderr, "Command not found\n");   //*** This only prints if exec fails.
    }
    else if (child > 0)
    {
        // This is really the parent process, so wait for the child to finish. 
        wait(NULL);
    }
    else
    {
        fprintf(stderr, "Error forking child\n");
    }
}

/**
 * Handles the situation of redirected output.
 *
 * pos is the position in the args array where the '>' occurs.
 */
void handle_redirect_output(char *args[], int pos)
{
    pid_t child;
    int fd;

    child = fork();

    if (child == 0)
    {
        // child process
        args[pos] = NULL;

        fd = open(args[pos+1], O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);

        if (fd == -1) {
            fprintf(stderr,"OPEN ERROR\n");
            exit(1);
        }

        // the file descriptor is duped to standard output
        if (dup2(fd, STDOUT_FILENO) == -1) {
            fprintf(stderr,"DUP ERROR\n");
            exit(1);
        }

        // close the original file descriptor
        if (close(fd) == -1) {
            fprintf(stderr,"CLOSE ERROR\n");
            exit(1);
        }

        execvp(args[0], args);   //*** Run the command.
        fprintf(stderr, "Command not found\n");   //*** This only prints if exec fails.
    }
    else if (child > 0)
    {
        // This is really the parent process, so wait for the child to finish.
        wait(NULL);
    }
    else
    {
        fprintf(stderr, "Error forking child\n");
    }
}

/**
 * Evaluates whether a pipe is present or not
 *
 */
int check_pipe(char *args[], int len) {
    for (int i = 0; i < len; i++) {
        if (strcmp(args[i], "|") == 0) {
            return i;
        }
    }
    return -1; // Pipe not found
}
