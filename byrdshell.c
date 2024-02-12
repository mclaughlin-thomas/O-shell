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
#include "CircularArray.h" // Here you need to include the header file for the circular array. Put this on the next line by iteslf:  #include "CircularArray.h"
                           //* By doing so, a global variable with be added named HistoryData that has room for 10 strings of data.

#define MAX_LINE    80 /* 80 chars per line, per command */

#define READ_END    0
#define WRITE_END   1

int const HISTORY_ITEMS = 10;   // Global constant for the max number of history items.

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
    int n;                      //***** NEW: declare an integer variable n. Its value will indicate that the desired history item is n items back.

    int count = 0;   // count indicates how many commands have been processed
    int input_redirect_position;   // input/output redirection
    
    char command[MAX_LINE]; // the command that was entered
    //delete below
    char history[MAX_LINE]; //***** NEW: delete this line since we do not want just one string of history and replace this
                            //***** line with a call to the HistoryInit function defined in CircularArray.c.
    //delete^ above
    char str[MAX_LINE];   //**** NEW: use this variable below to hold the command obtained from history.
    
    //*** Then modify the
    //*** original simple-shell.c to use the circular array to hold up to 10 items of history.
    //*** That would entail commenting off the declaration of the history string just
    //*** above and replacing it by the use of the functions HistoryInsert, HistoryLookup, and
    //*** HistoryInit.  Make sure that all uses of the old history string are commented off
    //*** below and that you use HistoryInsert and HistoryLookup instead.  You should look at
    //*** CircularArray.c to see what these functions do and what parameters they take. The
    //*** HistoryInit function is only used once, right here before we hit the while loop
    //*** for the shell.  If you get stuck, contact Br. David.

    HistoryInit(); // initializing the new history component

    while (should_run){
        // This is the main loop for the shell interface. It allows the shell to repeatedly show the prompt, get the user's command,
        // and execute that command. Then loop around and do the same steps over and over.


        //*********************************************
        //        User I/O | Print Shell Prompt and Receive Input
        //*********************************************
        printf("\n(*)> "); //*** DO THIS: print your prompt here. It could be as simple as >

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
        else if ( (strncmp(command, "!-", 2) == 0) && (count == 0) ){
            fprintf(stderr, "No command history found\n");
            continue;
        }
        else{
            count = 1;   //*** This is the shell's first command.
        }
        //----------------------------------------------------------------------------------------------------------------------

        //*** NEW: This section deals with the history, but does not execute the user's command.
        if ((strncmp(command,"!!", 2) != 0) && (strncmp(command,"!-",2) != 0))
        {
            HistoryInsert(command);
            /**
             * We have entered a command so copy the command to history.
             */
            //**** NEW: Get rid of this strcpy and use the HistoryInsert function to put the command into the circular array.
            //strcpy(history,command);
        }
        else if (strncmp(command,"!!",2) == 0)
        {
            if (HistoryCount > 0) {
                // Fetch the most recent command from history
                HistoryLookup(1, command); // 
                printf("Executing: %s\n", command);
            }
            else {
                fprintf(stderr, "No commands in history.\n");
                continue; // Skip further processing
            }
            // we use the most recent command
            //**** NEW: Use the HistoryLookup function to find the most recent command, print it to the screen, copy it to the command
            //**** variable so we can execute it below, and also use HistoryInsert to insert it into history, thus giving the
            //**** correct history that we ran this command twice in a row.
        }
        else if (strncmp(command, "!-", 2) == 0)
        {
            //**** NEW: This code has been done for you. It removes the first two characters, the !- from the command string.
            char temp[MAX_LINE];
            int len, k, m, num;
            len = strlen(command);
            for (k = 2, m = 0; k < len; k++, m++)
               temp[m] = command[k];
            temp[len] = '\0';
            //**** NEW: This code has been done for you. It removes the first two characters, the !- from the command string.

            //printf("DEBUG: number after the !- is %s", temp); //**** NEW: temp now contains the command string without the !-. This should be an integer since the user used !-n.
            num = atoi(temp); // Converts the string to an integer//**** NEW: Use the atoi function to convert temp to an integer, placing it into variable num.
            
            //**** NEW: Use HistoryLookup with num to look up the item in history num items back.
            //**** Then print that history item on the screen, copy it into the command variable so that we can execute it below,
            //**** and use HistoryInsert to get this item into the history circular array, much like was done above.
            //**** Thus !-1 would execute the last command, the same as !!, while !-2 would run the command 2 back in history,
            //**** !-3 would run the command 3 back in history, etc.

            if (num > 0 && num <= HistoryCount){ // Check if num is within the valid range
                // Use HistoryLookup to get the command from history
                HistoryLookup(num, command); // Adjust if your HistoryLookup expects different parameters

                // Print the command that is being executed
                printf("Executing: %s\n", command);

                // Re-insert the fetched command into history, if desired
                HistoryInsert(command);

                // Tokenize and execute the command as per your shell's logic
                // This might involve re-tokenizing command into args and then calling execvp or similar
                // Example: command_length = parse_command(command, args); followed by execution logic
            }
            else{
                fprintf(stderr, "No such command in history.\n");
            }


        }

        if (strncmp(command, "history", 7) == 0){
            // Check if there are any items in the history to display
            if (HistoryCount == 0){
                printf("No commands in history.\n");
            }
            else{
                // Loop through all items in history
                for (int k = 1; k <= HistoryCount; k++){
                    char historyItem[STRINGMAX]; // Assuming STRINGMAX is the length of your commands
                    HistoryLookup(k, historyItem); // Assuming this function populates historyItem with the kth command

                    // Print the history item number and the command
                    // Adjust the print format if you store history such that the most recent command is at the highest index
                    printf("%d: %s\n", k, historyItem);
                }
            }
            continue; // Skip further processing and continue with the next iteration of the loop
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
            exit(EXIT_FAILURE);  // Terminate the child process to prevent it from continuing with the rest of the code.
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

    while (i < len){
        if (strcmp(args[i], op) == 0){
            rv = i;
            break; // Exit the loop if the condition is met
        }
        i++; // Increment at the end of the loop body
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
        exit(EXIT_FAILURE);  // Terminate the child process to prevent it from continuing with the rest of the code.
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
        exit(EXIT_FAILURE);  // Terminate the child process to prevent it from continuing with the rest of the code.
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
    int i = 0; // Initialization outside the loop

    while (i < len) {
        if (strcmp(args[i], "|") == 0) {
            return i; // Return the index if a match is found
        }
        i++; // Increment at the end of the loop body
    }
    return -1; // Return -1 if the loop completes without finding a match

}
