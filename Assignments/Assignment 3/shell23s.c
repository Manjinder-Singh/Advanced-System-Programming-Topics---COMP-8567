
/*
Student Details:
Name - Manjinder Singh
Student ID - 110097177
Section - 4
*/

/*
Assignment Details:
Subject - Advanced System Programming
Assignment Number - 3
Assignment Topic - Mini Shell
Language - C
Coding Style - Google
Indent Width - 1
Column Width -120
Date of Submission - July 19, 2023
*/

/*
REFERENCES :
1. LAB WORK OF ASP submitted until June 13, 2023.
2. Class Code References shared by Doc.
3. Class Lectures.
4. Code Formatter(formatter.org)
5. C Libraries and documentations.
*/

// Includig all the required header files.
#include <ctype.h>
#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Declaring required global variables(for restricting operations as per requirement) to access throughout the execution
// of command in Mini Shell.
#define MAX_INPUT_ALLOWED 4096   // Defining maximum input size to be considered
#define MAX_NUM_CMDS_ALLOWED 7   // Restricting limit to 7 Operators i.e. 8 commands set with 7 operators(&& || ;) max.
#define MAX_NUM_PIPES_ALLOWED 7  // Restricting limit to 7 pipes i.e. 8 commands(with max 5 args) set in 7 pipes.
#define MAX_NUM_ARGS_ALLOWED 5   // Restricting limit to required number of 5 arguments

// Declaring required global variables(for file operations mode selection) to access throughout the execution of command
// in Mini Shell.
#define TRUNCATE_FILE_OUTPUT 1  // Defined to use for overwriting in the file.
#define APPEND_FILE_OUTPUT 2    // Defined to use for appending new data in the file along with the old one.

/*
Function Name:- slashesEscapedMS

Function Purpose:-
1. The purpose of this function (slashesEscapedMS) is to determine whether a character at a given index in a string is
escaped by performing a check if it is preceded by backslashes.
2. Its functionality is to identify whether a character is part of an escape sequence or it is supposed to be treated as
a literal character.

Variables Passed  -
1. msStr - msStr is a pointer of type Character that represents the string in the mini shell.
2. msIndex - msIndex is a variable of type integer. It is defined and used to check if the character at a given index
msIndex is escaped, i.e., if it is preceded by an odd number of backslashes.
*/

int slashesEscapedMS(const char* msStr, int msIndex) {
 if (msIndex > 0 && msStr[msIndex - 1] == '\\') {  // Checks condition for presence of "\\"
  int msCountCheck = 0;
  while (msIndex > 0 && msStr[msIndex - 1] == '\\') {  // Loop to count the consecutive backslashes for index>0.
   msCountCheck++;                                     // Incrementing the msCountCheck variable if \\ is found.
   msIndex--;  // Decreasing Index value so that it can proceed with the while loop until it is more than zero.
  }
  return msCountCheck % 2 != 0;  // Returning the count if it is odd or even.
 }
 return 0;  // Returning zero if the condition is not true.
}

/*
Function Name:- trimmingStringMS

Function Purpose -
1. The purpose of this function (trimmingStringMS) removes any spaces or whitespace characters at the beginning as well
as at the end of the provided string.
2. It will be modify the string in-place by adjusting its start and end positions, and ensures that the trimmed string
is properly terminated before returning it at the end.

Variables Passed -
1. msStr is a pointer of type Character which represents the command entered in mini shell that we want to trim for
space occurences.
*/

char* trimmingStringMS(char* msStr) {
 int z;
 while (isspace(*msStr)) {  // For trimming leading whitespace.
  msStr++;                  // Moving pointer to the next character by incrementing.
 }
 for (z = strlen(msStr) - 1; (isspace(msStr[z])); z--)
  ;                    // For finding the last non-whitespace character.
 msStr[z + 1] = '\0';  // Terminating the string with the null after the last non-whitespace character occurrence.
 return msStr;         // Returning the trimmed string in the end.
}

/*
Function Name:- msPrepareFormattedCommand

Function Purpose -
1. The purpose of this function (prepareCommandMS) is to take a command string (msCmd), process it, and then return a
modified version of the command after checking few characters.
2. Then It adds spaces around special characters (<, >, |, &, and ;) in the command, except when they are part of a
double special character sequence (>|, &>, &&, etc.).
3. Finally, the changed command after updates is then returned as a dynamically allocated character array (char*).

Variabled Passed -
1. msCmd is a constant pointer to a character array which represents the command string that we want to prepare/modify.
*/

char* msPrepareFormattedCommand(const char* msCmd) {
 const char* msSpecialChar = "<>|&;";
 const char* msDoubleSpecialChar = ">|&";
 size_t msLenOfCommand = strlen(msCmd);
 char* msChangedCommand = malloc(msLenOfCommand * 2 + 1);  // Allocating memory to the command after modifications.
 size_t y = 0, z = 0;
 while (y < msLenOfCommand) {                                                    // Iterating through the command
  if (strchr(msSpecialChar, msCmd[y]) != NULL && !slashesEscapedMS(msCmd, y)) {  // Checking for the special characters.
   if (strchr(msDoubleSpecialChar, msCmd[y]) != NULL && y + 1 < msLenOfCommand &&
       msCmd[y] == msCmd[y + 1]) {       // Checking for double special characters.
    msChangedCommand[z++] = ' ';         // Adding space to the changed(updated/new) command
    msChangedCommand[z++] = msCmd[y++];  // Copying the special character to the changed(updated/new) command.
    msChangedCommand[z++] =
        msCmd[y];  // Copying the special character in case of repetition to the changed(updated/new) command.
    msChangedCommand[z++] = ' ';  // Adding space to the changed(updated/new) command.
    msChangedCommand[z++] = ' ';  // Adding additional space to the changed(updated/new) command.
   } else {
    msChangedCommand[z++] = ' ';       // Adding space to the changed(updated/new) command.
    msChangedCommand[z++] = msCmd[y];  // Copying the special character to the changed(updated/new) command.
    msChangedCommand[z++] = ' ';       // Adding space to the changed(updated/new) command.
   }
  } else {
   msChangedCommand[z++] = msCmd[y];  // Copying the non-special character to the changed(updated/new) command.
  }
  y++;  // Incrementing the counter by 1.
 }
 msChangedCommand[z] = '\0';  // Terminating the modified command with the null i.e. '\0'
 return msChangedCommand;     // In the end, returning the modified command.
}

/*
Function Name:- msCheckCommand

Function Purpose -
1. The purpose of this function (msCheckCommand) is to validate a command by checking the number of arguments it
contains.
2. The function will trim the leading as well as trailing whitespace from the command, tokenizes it by spaces, and the
counts the number of arguments.
3. Finally, it will return true if the number of arguments is within a specified range (MAX_NUM_ARGS_ALLOWED) and
greater than zero; otherwise, it returns false.

Variabled Passed -
1. msCmd is a pointer to a character datatype that represents the command in mini shell.
*/

int msCheckCommand(char* msCmd) {
 char* msCommand = strdup(msCmd);          // strdup will create the copy of the command.
 msCommand = trimmingStringMS(msCommand);  // Removing leading as well as trailing whitespaces from the command.
 char* tokStr = strtok(msCommand, " ");    // Performing command tokenize operation using space delimiter.
 int argsChk = 0;
 while (tokStr != NULL) {      // Iterating through command to count for the arguments number.
  tokStr = strtok(NULL, " ");  // Going to the next token.
  argsChk++;                   // Then increase the argument count by 1.
 }
 return argsChk <= MAX_NUM_ARGS_ALLOWED &&
        argsChk > 0;  // It will return whether the arguments are within the limit as declared in global variables.
}

/*
Function Name:- msCreateSpecialStrTokens

Function Purpose -
The purpose of this function (msCreateSpecialStrTokens) is to parse a given string(msString) and create special tokens
by splitting it based on specific delimiters such as "&&", "||", "|", and ";".
2. The function also allocates memory for each token and stores them in the msTokens array.
3. Then it will return the total count of special tokens created (newSpcTokens).

Variabled Passed -
1. msString - This variable is a reference to a sequence of characters. It represents the input string that the function
will operate on.
2. msTokens - The variable tokens is a reference to an array of strings. It is declared as a pointer to a pointer to a
character array (char**). It will store the special characters extracted from the input string msString, and each
special character will be stored as a separate token in the array.
*/

int msCreateSpecialStrTokens(char* msString, char** msTokens) {
 int msNewTokens = 0;
 int stringLength = strlen(msString);  // Storing the length of the input string.
 int indexPos;
 int indexChk = 0;
 for (indexPos = 0; indexPos < stringLength; indexPos++) {  // Iterating through each character of the input string.
  if (strncmp(msString + indexPos, "&&", 2) == 0) {         // Checking for 2 consecutive & characters.
   msTokens[msNewTokens] = malloc((indexPos - indexChk + 1) * sizeof(char));  // Memory allocation for the token.
   strncpy(msTokens[msNewTokens++], msString + indexChk,
           indexPos - indexChk);                            // Using strncpy for copying the substring of the token.
   msTokens[msNewTokens++] = "&&";                            // Setting the token value to - &&.
   indexPos++;                                              // Increasing the index by 1 to skip the next character.
   indexChk = indexPos + 2;                                 // Updating the index value for checking the next token.
  } else if (strncmp(msString + indexPos, "||", 2) == 0) {  // Checking for 2 consecutive | characters.
   msTokens[msNewTokens] = malloc((indexPos - indexChk + 1) * sizeof(char));  // Memory allocation for the token.
   strncpy(msTokens[msNewTokens++], msString + indexChk,
           indexPos - indexChk);           // Using strncpy for copying the substring of the token.
   msTokens[msNewTokens++] = "||";           // Setting the token value to - ||.
   indexPos++;                             // Increasing the index by 1 to skip the next character.
   indexChk = indexPos + 2;                // Updating the index value for checking the next token.
  } else if (msString[indexPos] == '|') {  // Checks for single | character.
   msTokens[msNewTokens] = malloc((indexPos - indexChk + 1) * sizeof(char));  // Memory allocation for the token.
   strncpy(msTokens[msNewTokens++], msString + indexChk,
           indexPos - indexChk);           // Using strncpy for copying the substring of the token.
   msTokens[msNewTokens++] = "|";            // Setting the token value to - |.
   indexChk = indexPos + 1;                // Updating the index value for checking the next token.
  } else if (msString[indexPos] == ';') {  // Checks for ; character.
   msTokens[msNewTokens] = malloc((indexPos - indexChk + 1) * sizeof(char));  // Memory allocation for the token.
   strncpy(msTokens[msNewTokens++], msString + indexChk,
           indexPos - indexChk);  // Using strncpy for copying the substring of the token.
   msTokens[msNewTokens++] = ";";   // Setting the token value to - ;.
   indexChk = indexPos + 1;       // Updating the index value for checking the next token.
  }
 }
 msTokens[msNewTokens] = malloc((stringLength - indexChk + 1) * sizeof(char));  // Memory allocation for the last token.
 strncpy(msTokens[msNewTokens++], msString + indexChk,
         stringLength - indexChk);  // Performing the copy operation on the remaining substring for the token.
 return msNewTokens;                  // In the end, returning the count of special tokens.
}

/*
Function Name:- msExecuteCommand

Function Purpose -
1. The purpose of this function is to execute a command in the mini shell program.
2. It will also handle execution of some commands which mainly includes the tokenization of command, handling I/O
redirection, process of forking, and the execution of command with the appropriate file descriptors.
3. This function will also handle the error conditions and provides appropriate messages on the background process
status for prcoess.

Variabled Passed -
1. msCmd - The command which is to be executed.
2. forkFlagStatus - It is for the process to decide that it should be forked or not for the background execution.
*/

int msExecuteCommand(char* msCmd, int forkFlagStatus) {
 char* msDelimiterTokenize = " ";  // It will be used for splitting the command into tokens based on space delimiter.
 char* msOutputFileRedirection = NULL;   // msOutputFileRedirection - For output file redirection.
 int statusOfBackgroundProcess = 0;      // Declared for background process status.
 char* redirectionForInputFile = NULL;   // redirectionForInputFile - For input file redirection.
 char* msStrTokenize;                    // It will be used for string tokenizing.
 char* msArgsVar[MAX_NUM_ARGS_ALLOWED];  // It will store command arguments.
 char* msCommand = strdup(msCmd);        // Copying the command.

 msCommand = trimmingStringMS(msCommand);  // Removing leading as well as trailing whitespace from the command.

 int msModeOfOutput = TRUNCATE_FILE_OUTPUT;  // Setting File mode to "TRUNCATE"

 if (msCommand[strlen(msCommand) - 1] == '&') {  // It will check for the Background Process.
  statusOfBackgroundProcess = 1;                 // Setting flag value for the background process to 1.
  msCommand[strlen(msCommand) - 1] = '\0';
  msCommand = trimmingStringMS(msCommand);
 }

 if (strcmp(msCommand, "exit") == 0) {  // It will check if the command is matched with exit.
  exit(0);                              // Then exiting with the value 0.
 }

 if (strcmp(msCommand, "cd") == 0) {  // It will check if the command is matched with "cd".
  chdir(getenv("HOME"));              // For changing directory to the "HOME" directory.
  return 0;
 } else if (strstr(msCommand, "cd ") == msCommand) {  // Checking if the command is in the format " cd <dir>".
  char* msDirectoryPath = msCommand + 3;              // Extracting the dir path.
  msDirectoryPath = trimmingStringMS(
      msDirectoryPath);    // It will trim the leading as well as trailing whitespace from the directory path.
  chdir(msDirectoryPath);  // Working Dir is changed to the particular dir.
  return 0;
 }

 int y = 0;
 char* msFollowingToken;  // Token for function - strtok_r

 msStrTokenize = strtok_r(msCommand, msDelimiterTokenize, &msFollowingToken);  // Command Tokenization.

 while (msStrTokenize != NULL) {          // Looping/Iterating through tokens.
  if (strcmp(msStrTokenize, "<") == 0) {  // It will check for the input redirection symbol (<).
   redirectionForInputFile =
       strtok_r(NULL, msDelimiterTokenize, &msFollowingToken);  // Getting the file name for input for redirection.
  } else if (strcmp(msStrTokenize, ">") == 0) {                 // It will check for the output redirection symbol (>).
   msOutputFileRedirection =
       strtok_r(NULL, msDelimiterTokenize, &msFollowingToken);  // Getting the file name for output for redirection.
   msModeOfOutput = TRUNCATE_FILE_OUTPUT;                       // Setting the mode of output to Truncate.
  } else if (strcmp(msStrTokenize, ">>") ==
             0) {  // It will check for the output redirection symbol with the append (>>) mode.
   msOutputFileRedirection =
       strtok_r(NULL, msDelimiterTokenize, &msFollowingToken);  // Getting the file name for output for redirection.
   msModeOfOutput = APPEND_FILE_OUTPUT;                         // Setting the mode of output to Append.
  } else {
   msArgsVar[y++] = msStrTokenize;  // Storing the args of the command.
  }
  msStrTokenize = strtok_r(NULL, msDelimiterTokenize, &msFollowingToken);  // Fetching the next token.
 }
 msArgsVar[y] = NULL;  // It will terminate the arguments array with NULL.

 int msProcessId = 0;
 if (forkFlagStatus) {
  msProcessId = fork();  // Process Forking at this point.
 }

 if (msProcessId < 0) {  // Handling error as the PID can't be less than zero in case of forking is successful.
  printf("Issue encountered during process forking.\n");
  fprintf(stderr, "Issue encountered during process forking.\n");
  exit(1);
 } else if (msProcessId == 0) {       // In case it is a child process.
  int inputFileDesc, outputFileDesc;  // File descriptors for I/O.
  if (redirectionForInputFile) {      // It will check for the input redirection is specified or not.
   inputFileDesc = open(redirectionForInputFile, O_RDONLY);  // Opens the input file in Read Only mode.
   if (inputFileDesc < 0) {  // Handling error in case of an issue with the opening of the input file.
    printf("Issue encountered during the opening of the input file.\n");
    fprintf(stderr, "Issue encountered during the opening of the input file.\n");
    exit(1);
   }
   dup2(inputFileDesc, STDIN_FILENO);            // It will redirect the input to the file.
   close(inputFileDesc);                         // Closing the file.
  } else if (statusOfBackgroundProcess) {        // It will Check if the process is running in the Background.
   inputFileDesc = open("/dev/null", O_RDONLY);  // Opens "/dev/null" as input.
   if (inputFileDesc < 0) {                      // Handling error in case of an issue with the opening of /dev/null.
    printf("Issue encountered during the opening of the input file.\n");
    fprintf(stderr, "Issue encountered during the opening of the input file.\n");
    exit(1);
   }
   dup2(inputFileDesc, STDIN_FILENO);  // Redirecting input to "/dev/null"
   close(inputFileDesc);               // The close function will be closing the file.
  }
  if (msOutputFileRedirection) {                  // It will check if the output redirection is specified.
   if (msModeOfOutput == TRUNCATE_FILE_OUTPUT) {  // Checks output mode if set to TRUNCATE MODE.
    outputFileDesc = open(msOutputFileRedirection, O_WRONLY | O_CREAT | O_TRUNC,
                          0666);                       // It will Open the Output File in TRUNCATE MODE.
   } else if (msModeOfOutput == APPEND_FILE_OUTPUT) {  // Checks output mode if set to APPEND MODE.
    outputFileDesc = open(msOutputFileRedirection, O_WRONLY | O_CREAT | O_APPEND,
                          0666);  // It will Open the Output File in APPEND MODE.
   }
   if (outputFileDesc < 0) {  // Handling error in case of an issue with the opening of the output file.
    printf("Issue encountered during the opening of the output file.\n");
    fprintf(stderr, "Issue encountered during the opening of the output file.\n");
    exit(1);
   }
   dup2(outputFileDesc, STDOUT_FILENO);               // It will redirect the output to the file.
   dup2(open("/dev/null", O_WRONLY), STDERR_FILENO);  // It will redirect the error to "/dev/null".
   close(outputFileDesc);
  } else if (statusOfBackgroundProcess) {         // It will Check if the process is running in the Background.
   outputFileDesc = open("/dev/null", O_WRONLY);  // It will open "/dev/null" for O/P.
   if (outputFileDesc < 0) {  // Handling error in case of an issue with the opening of "/dev/null" file.
    printf("Issue encountered during the opening of the output file.\n");
    fprintf(stderr, "Issue encountered during the opening of the output file.\n");
    exit(1);
   }
   dup2(outputFileDesc, STDOUT_FILENO);               // It will redirect the output to "/dev/null".
   dup2(open("/dev/null", O_WRONLY), STDERR_FILENO);  // It will redirect the errors to "/dev/null".
   close(outputFileDesc);
  }
  if (execvp(msArgsVar[0], msArgsVar) < 0) {  // Error Handling in case of an issue with the execution of the command.
   printf("Issue encountered during the execution of the command.\nPlease Check Command if it is valid or not...\n");
   // fprintf(stderr, "Issue encountered during the execution of the command.\n");
   // perror("Issue encountered during the execution of the command.");
   // exit(1);
   return -1;
  }
 } else {                            // In case it is a parent process.
  if (!statusOfBackgroundProcess) {  // If the process is not running in the Background.
   int msProcessStatus;
   waitpid(msProcessId, &msProcessStatus, 0);  // Wait for the child process to be done with its part of the execution.
   return msProcessStatus;                     // Returns process status in the end.
  } else {  // If the process is running in the Background, then print the message and return success.
   printf("Displaying the Background Process ID(PID): %d\n", msProcessId);
   return 0;
  }
 }

 if (forkFlagStatus == 0) {
  if (!statusOfBackgroundProcess) {  // If the process is not running in the Background.
   int msProcessStatus;
   waitpid(msProcessId, &msProcessStatus, 0);  // Wait for the child process to be done with its part of the execution.
   return msProcessStatus;                     // Then return the process status.
  } else {  // If the process is running in the Background, then print the message and return success.
   printf("Displaying the Background Process ID(PID): %d\n", msProcessId);
   return 0;
  }
 }
 return 0;
}

/*
Function Name:- msExecutePipe

Function Purpose -
1. The purpose of this function is to execute multiple commands in the Mini Shell Program which are connected by the
pipe(s).
2. It will mainly handle the
2.1 creation of pipes --->
2.2 forking processes for every command --->
2.3 Setting up I/O redirection in between the commands --->
2.4 Execution of commands --->
2.5 Finally, Waiting for the last command to done with its execution part.


Variabled Passed -
1. msCmds - It will hold the commands that are to be executed.
2. msCmdsCount - It will hold the value for the number of commands in the array.
*/

int msExecutePipe(char** msCmds, int msCmdsCount) {
 if ((msCmdsCount - 1) / 2 > MAX_NUM_PIPES_ALLOWED) {  // Checking for allowed pipes operation.
  printf("MAXIMUM PIPING OPERATIONS ALLOWED ARE %d\n.", MAX_NUM_PIPES_ALLOWED);
  return 1;
 }
 int countVar;
 int numOfPipes = (msCmdsCount - 1) / 2;
 int pipeFiledes[numOfPipes][2];
 int forkStatus;

 for (countVar = 0; countVar < numOfPipes; countVar++) {  // Creating pipes here.
  if (pipe(pipeFiledes[countVar]) < 0) {                  // In case there is an issue during piping.
   printf("Issue encountered related to piping.\n");
   perror("Error related to piping.\n");
   return 1;
  }
 }

 for (countVar = 0; countVar < msCmdsCount;
      countVar += 2) {  // In this step, forking will happen and commands will be executed.
  forkStatus = fork();  // Process fork.

  // ERROR HANDLING...
  if (forkStatus < 0) {  // As PID cannot be negative, in case it is then it is considered as error.
   printf("Issue encountered related to forking.\n");
   perror("Error related to forking.\n");
   return 1;
  } else if (forkStatus == 0) {
   if (countVar > 0) {
    if (dup2(pipeFiledes[countVar / 2 - 1][0], 0) <
        0) {  // It will connect the input of the current command to the output of the previous command.
     printf("Issue encountered related to redirection(dup2).\n");
     perror("Error related to redirection(dup2).\n");
     return 1;
    }
   }
   if (countVar < msCmdsCount - 1) {  // It will connect output of the current command to the input of the next command
    if (dup2(pipeFiledes[countVar / 2][1], 1) < 0) {
     printf("Issue encountered related to redirection(dup2).\n");
     perror("Error related to redirection(dup2).\n");
     return 1;
    }
   }

   int pipeCount;
   for (pipeCount = 0; pipeCount < numOfPipes;
        pipeCount++) {  // Closing required file descriptors as per good programming practice.
    close(pipeFiledes[pipeCount][0]);
    close(pipeFiledes[pipeCount][1]);
   }

   return msExecuteCommand(msCmds[countVar], 0);  // Command execution will take place using msExecuteCommand function.
  }
 }

 for (countVar = 0; countVar < numOfPipes;
      countVar++) {  // Closing all the file descriptors in the parent process as per good programming practice.
  close(pipeFiledes[countVar][0]);
  close(pipeFiledes[countVar][1]);
 }

 int status;
 waitpid(forkStatus, &status, 0);  // It will wait until the last command is done with its part execution.
 return status;
}

/*
Function Name:- msRunCommands

Function Purpose -
1. The purpose of this function is to execute a sequence of commands in the Mini Shell Program.
2. It will mainly handle conditional and sequential execution operators like OR(||), AND(&&), Pipes(|), and
SEQUENTIAL(;).

Variabled Passed -
1. msCmds - It will hold the command that is going to be executed.
2. msLenofCmd - It will hold the length value of the command.

*/
// 325
void msRunCommands(char** msCmds, int msLenofCmd) {
 int msLastRes = 1;
 int msCmdIndexPos = 0;
 int msResultOfCmd = 0;
 int msPipeFD[2];
 int sequentialCount = 0;   // For tracking the sequential command operators count.
 int conditionalCount = 0;  // For tracking the conditional command operators count.

 while (msCmdIndexPos < msLenofCmd) {              // Comparing current position with the length of command.
  if (strcmp(msCmds[msCmdIndexPos], "||") == 0) {  // Checking for OR(||) Operator.
   conditionalCount++;  // Incrementing conditionalCount variable to keep track of || command.
   if (conditionalCount > MAX_NUM_CMDS_ALLOWED) {  // Check if limit is exceeded
    printf("MAXIMUM %d CONDITIONAL EXECUTION OPERATORS ARE ALLOWED.\n", MAX_NUM_CMDS_ALLOWED);
    return;
   }
   if (msLastRes == 0) {  // In case previous command fails, then skip to next command after OR(||) operator.
    msCmdIndexPos++;

    // It will Skip any additional commands until a different operator (; && |) is found.
    while (msCmdIndexPos < msLenofCmd && strcmp(msCmds[msCmdIndexPos], "&&") != 0 &&
           strcmp(msCmds[msCmdIndexPos], "|") != 0 && strcmp(msCmds[msCmdIndexPos], ";") != 0) {
     msCmdIndexPos++;
    }
    if (msCmdIndexPos > 0 && msCmdIndexPos < msLenofCmd &&
        strcmp(msCmds[msCmdIndexPos], "|") == 0)  // It will skip 2 positions in case the next operator is pipe.
     msCmdIndexPos += 2;
   } else {
    msCmdIndexPos++;  // Then proceeding with the next command.
   }
  } else if (strcmp(msCmds[msCmdIndexPos], "&&") == 0) {  // Checks for AND(&&) Operator.
   conditionalCount++;  // Incrementing conditionalCount variable to keep track of && along with || command.
   if (conditionalCount > MAX_NUM_CMDS_ALLOWED) {  // Check if limit is exceeded
    printf("MAXIMUM %d CONDITIONAL EXECUTION OPERATORS ARE ALLOWED.\n", MAX_NUM_CMDS_ALLOWED);
    return;
   }
   if (msLastRes != 0) {  // In case last command is success, then move to the next command after && operator.
    msCmdIndexPos++;

    // Skipping until different operator(; | ||) is not encountered.
    while (msCmdIndexPos < msLenofCmd && strcmp(msCmds[msCmdIndexPos], "||") != 0 &&
           strcmp(msCmds[msCmdIndexPos], "|") != 0 && strcmp(msCmds[msCmdIndexPos], ";") != 0) {
     msCmdIndexPos++;
    }
    if (msCmdIndexPos > 0 && msCmdIndexPos < msLenofCmd &&
        strcmp(msCmds[msCmdIndexPos], "|") == 0)  // In case the operator is Pipe(|)
     msCmdIndexPos += 2;
   } else {
    msCmdIndexPos++;  // Proceeding to the next command.
   }
  } else if (strcmp(msCmds[msCmdIndexPos], "|") == 0) {
   msCmdIndexPos++;
  } else if (strcmp(msCmds[msCmdIndexPos], ";") == 0) {
   sequentialCount++;  // Incrementing sequentialCount variable to keep track of ; (operator) count.
   if (sequentialCount > MAX_NUM_CMDS_ALLOWED) {  // Check if limit is exceeded
    printf("MAXIMUM %d SEQUENTIAL COMMANDS ARE ALLOWED.\n", MAX_NUM_CMDS_ALLOWED);
    return;
   }
   msCmdIndexPos++;
  } else if (msCmdIndexPos < msLenofCmd - 1 &&
             strcmp(msCmds[msCmdIndexPos + 1], "|") == 0) {  // Checks if the next command is having | operator.
   int msStartPos = msCmdIndexPos;                           // It will store the current command position.
   while (msCmdIndexPos < msLenofCmd - 1 &&
          strcmp(msCmds[msCmdIndexPos + 1], "|") == 0)  // It will skip multiple | operatorrs.
    msCmdIndexPos += 2;
   msExecutePipe(msCmds + msStartPos, msCmdIndexPos - msStartPos + 1);  // Executing the command with the | operator.
   msCmdIndexPos++;
  } else {  // If above mentioned conditions are not matched then it is a regular command.
   msResultOfCmd = msExecuteCommand(msCmds[msCmdIndexPos], 1);
   msLastRes = msResultOfCmd;  // It will update the result of last command.
   msCmdIndexPos++;            // Incrementing so as to proceed with the next command.
  }
 }
}

/*
Function Name:- msValidateCommand

Function Purpose:-
1. The purpose of this function is to validate the commands entered by the user in the Mini Shell Program
2. Its functionality will include the checking for various conditions such as the
2.1 Number of commands,
2.2 The number of operators, and
2.3 The validity of each command.

Variables:

msUserCmds: An array of strings representing the user-entered commands.
msCmdsNum: The number of commands in the array.

Variabled Passed:- 1. msUserCmds - An array of string that will represent the command entered by user on Mini Shell
Program.
2. msCmdsNum - An integer values that will tell the number of command in the array.
*/

int msValidateCommand(char** msUserCmds, int msCmdsNum) {
 if (msCmdsNum == 0) {  // In case there is no command entered by user.
  printf("Please enter command as there is no commands entered.\n");
  return 0;
 }
 if (msCmdsNum % 2 == 0) {  // It will check if the number of commands are even.
  printf("Please check INPUT  as it is not valid.\n");
  return 0;
 }

 // It will check if the number of commands exceeds the specified limit
 int opsCount = msCmdsNum / 2;
 if (opsCount > MAX_NUM_CMDS_ALLOWED) {
  printf("MAXIMUM OPERATORS LIMIT REACHED...\n");
  // perror("Operators Limit exceeded.\n");
  return 0;
 }

 // Iterating through the commands to check for their validity
 for (int curUserCmd = 0; curUserCmd < msCmdsNum; curUserCmd++) {
  if (curUserCmd % 2)  // Skip the checking in case the command is at an even index.
   continue;

  int isCmdValid = msCheckCommand(msUserCmds[curUserCmd]);  // It will check if the command is valid.

  if (!isCmdValid) {  // In case the command is invalid and not within limit.
   printf("Please check COMMAND limit and commands entered must be valid.\n");
   return 0;
  }
 }

 return 1;  // In case all commands are valid.
}

/*
MAIN FUNCTION
Process Flow :-
1. Infinite Loop to accept User Input --->
2. Triming leading as well as trainiling whitespaces from input --->
3. Checks for empty input from user --->
4. Prepares a formatted Command --->
5. Creates of command tokens --->
6. Validates command tokens -->
7. Runs validated command.
*/
int main() {
 printf("---MINI SHELL PROGRAM---\n");
 while (1) {                         // Infinite Loop for the Mini Shell Execution with the user input commands.
  char* msUserInput = malloc(5120);  // Allocates memory for the user input.
  fflush(stdout);                    // stdout is flushed to make sure prompt is displayed properly.
  printf("---->mshell$ ");           // Message to be printed for shell prompt.
  fgets(msUserInput, MAX_INPUT_ALLOWED, stdin);  // Reading the input from user using stdin.
  msUserInput =
      trimmingStringMS(msUserInput);  // It will trim the leading as well as trailing whitespace from the user input.
  if (strlen(msUserInput) == 0) {     // Checking in case the input is empty.
   continue;                          // continue will make sure to skip to the next iteration in the loop
  }
  char* msFormattedInput = msPrepareFormattedCommand(
      msUserInput);           // It will prepare the formatted command using function - msPrepareFormattedCommand
  char* msCommadTokens[200];  // Declared array to store the command tokens.
  int msNewCommadTokens = msCreateSpecialStrTokens(
      msFormattedInput, msCommadTokens);  // It will create command tokens using function - msCreateSpecialStrTokens
                                          // with the input - msFormattedInput and msCommadTokens.
  int isCommandValid =
      msValidateCommand(msCommadTokens, msNewCommadTokens);  // msValidateCommand function will validate the command.
  if (isCommandValid)                                 // In case the command is valid, then it will proceed further.
   msRunCommands(msCommadTokens, msNewCommadTokens);  // It will run the validated commands
 }
 return 0;  // Return zero in case of successful execution.
}

/*
Illustration Explained in the Vido which I executed for the Testing of this Program for Explanation:

Positive Cases
1. ls
2. ls -1 /home/lnu8/testFolderForAssgn3 -S -n
3. cat hello1.txt
4. nano new.txt (Ctrl+C to cancel and to return to shell).
5. date | wc

6. ls -1 /home/lnu8/testFolderForAssgn3 -S -n | wc -w
7. ls -1 /home/lnu8/testFolderForAssgn3 -S -n | ls -1 /home/lnu8/testFolderForAssgn3 -S -n | ls -1
/home/lnu8/testFolderForAssgn3 -S -n | ls -1 /home/lnu8/testFolderForAssgn3 -S -n | wc -w
8. ls -1 /home/lnu8/testFolderForAssgn3 -S -n | wc -w | wc -w | wc -w | wc -w | wc -w | wc -w | wc -w

9. nano hello.txt
10 wc -w < hello.txt
11 ls -l > output.txt
12. nano output.txt
13. ls -l >> output.txt
14 nano output.txt

15 ./hello1 && ./hello1 && ./hello1
16. ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1

17. ./hello1 || ./hello1 || ./hello1
18. ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1

19. ./hello1 || ./hello1 && ./hello1 || ./hello1
20. ./hello1 && ./hello1 && ./hello1 && ./hello1 || ./hello1 || ./hello1 && ./hello1 && ./hello1

21. ./bg &
22. ps -u

23. cat hello1.txt;date
24. cat hello1.txt; cat hello1.txt; cat hello1.txt; cat hello1.txt ; ls ; date ; ls 

25. cat hello1.txt; date ; ls |./hello1 && ./hello1 || ./hello1 | date | ls -1 /home/lnu8/testFolderForAssgn3 -S -n

Negative Cases(With Error Handling)

1. lsls
2. cat new.txt
3. ls -1 /home/lnu8/testFolderForAssgn3 -S -n -lha

4. ls -1 /home/lnu8/testFolderForAssgn3 -S -n | ls -1 /home/lnu8/testFolderForAssgn3 -S -uniq -n| wc -w
5. ls -1 /home/lnu8/testFolderForAssgn3 -S -n | wc -w | wc -w | wc -w | wc -w | wc -w | wc -w | wc -w | wc -w

6. ./hello100 && ./hello1 || ./hello1
7. ./hello100 || ./hello1 || ./hello1
8. ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1 && ./hello1
9. ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1 || ./hello1
10. ./hello1 || ./hello1 && ./hello1 || ./hello1 && ./hello1 || ./hello1 && ./hello1 || ./hello1 && ./hello1
11. cat hello1.txt;date; cat hello1.txt; cat hello1.txt; cat hello1.txt; cat hello1.txt ; ls ; date ; ls -1

*/

/*
ONE DRIVE LINK FOR WORKING OF CODE and OUTPUT EXPLANATION:- 
https://uwin365-my.sharepoint.com/:f:/r/personal/lnu8_uwindsor_ca/Documents/ASP%20Assignment%203?csf=1&web=1

Privacy Settings of One Drive shared Folder - People logged in University of Windsor Credentials can view it.
*/