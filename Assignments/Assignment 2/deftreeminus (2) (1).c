//Works on Oracle VM Virtual Box, Not on No Machine
//Working Code
// Name - Manjinder Singh || Student ID - 110097177 || Section - 4
// Assignment - 2 || Question - 2 Solution

/*
References : 
1. Class Study Content
2. Class Code Snippets
3. Assignment 1 - solution of Advanced System Programming submitted duirng this course.
4. Assignment 2 - solution of 1st question is referred as some parts were common.
*/

// #define(pre processor directives) is used in C programming language to define macros or constants.
// Below are the three constants declared to be used for the execution of first problem statement of assignment question 1
#define MAXIMUM_CHILD 250 // Restricting maximum number of child processes allowed to 250.
#define MAXIMUM_DEPTH 250 // Restricting maximum depth to 250 for process level.
#define DEBUG_MODE_IS_ACTIVE 0 // It will be used to enable and disable debug mode during execution. it is used with if(debug mode) sets to Value 1 means True and 0 means false(will not show print statement).

// Including required header files for the execution of the program(assignment - 2,question - 2)
#include <stdio.h> // It is used to handle I/O(scanf,printf) functions for assignment - 2,question - 2.
#include <dirent.h> // It is used to execute directory(readdir,opendir) functions for assignment - 2,question - 2.
#include <ctype.h> // It is used to execute character(tolower,isstring) functions for assignment - 2,question - 2.
#include <stdbool.h> // It is used to handle boolean(true,false) data type for assignment - 2,question - 2.
#include <string.h> // It is used to handle string(strlen,strcmp) manipulations functions for assignment - 2,question - 2.
#include <unistd.h> // It is used to execute POSIX operating system for API functions for assignment - 2,question - 2.
#include <stdlib.h> // It is used to handle library(of type standard(free,atoi)) funcions for assignment - 2,question - 2.
#include <sys/wait.h> // It is used to interpret the exit status of different child processes for assignment - 2,question - 2.. It mainly deals with the constants like WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG, wait(),waitpid().
#include <time.h> // It is used for time related functions already defined under time header file for assignment - 2,question - 2.

int childProcessIDs[MAXIMUM_CHILD]; // Declared integer array with name 'childProcessIDs' for storing process IDs of child with the size of MAXIMUM_CHILD(constant variable).
int bashdProcessID; // Declared integer variable with name 'bashdProcessID' for storing process ID of bash process.
int processPID; 

// This function returns the parent process ID(called as PPID) of a given process(Called as PID).
int getParentProcessID(int processID) {
  char statFilePath[150]; // Character variable created to store the path of stat file.
  FILE *statFilePtr; // File pointer declared for the stat file.
  int parentProcessID; // To store parent process ID(PPID) in the integer variable 'parentProcessID'. 

  sprintf(statFilePath, "/proc/%d/stat", processID);// In this line, sprintf function is used to format and store sequence of characters which is a path to the Stat file with the specific process id.
  statFilePtr = fopen(statFilePath, "r"); // Opens the stat file for reading.
  if (statFilePtr == NULL) { // For checking if the file oepration is success.
	printf("\nError opening the stat file, please check.");
    perror("Error opening the stat file, please check."); // Displaying error message in case opening stat file fails.
    exit(EXIT_FAILURE); // It will terminate the program with a failure status.
  }

  fscanf(statFilePtr, "%*d %*s %*c %d", &parentProcessID); // Reads parent process id from 4th place.
  fclose(statFilePtr); // Closing the Stat file before exit.
  return parentProcessID; // Returns the process id of parent.
}

// This function checks if a process is in DEFUNCT(i.e. Zombie process) state or not.
bool isProcessDefunct(int processID) {
  char statusFilePathForPathInfo[100]; // Declared character array to store the path of the status file.
  FILE *statusOfFile; // Declared file pointer to handle the status file.
  char statusOfLine[150]; // To read each line from status file.
  char *statusOfZombieProcess; // statusOfZombieProcess is a character Pointer to store the status information.
  bool result_flag = false; // Boolean variable declared to store result.

  sprintf(statusFilePathForPathInfo, "/proc/%d/status", processID); // sprintf function is used to format the status file path.
  statusOfFile = fopen(statusFilePathForPathInfo, "r"); // statusOfFile variable will record the file operation of mode read.

  while (fgets(statusOfLine, sizeof(statusOfLine), statusOfFile) != NULL) { // It will first read each line from the status file using fgets operation of file function and continue untill it reaches to the end of the file.
    if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
      printf("\n Line Read Currently  : %s", statusOfLine); // To print each line of the file which is read one by one.
    }
    if (strncmp(statusOfLine, "State:", 6) == 0) { // It will compare the first 6 charcaters of status of line with the state.
      statusOfZombieProcess = statusOfLine + 7; // If it is matched then, getting the Status text written after "State:".
      if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
        printf("\n Status of Process if its Zombie or not: %s", statusOfZombieProcess);
      }
      if (strstr(statusOfZombieProcess, "Z (zombie)") != NULL) { // Checks if the function is not returning a null pointer i.e. Z (zombie) is found in the statusOfZombieProcess.
        result_flag = true; //If found the result flag is set to True.
      }
      break;
    }}

  fclose(statusOfFile); // Closing the status file under proc.
  return result_flag; // Returning the result_flag with appropriate value after th function execution completes.
}


// This function is defined to fetch the process IDs(PIDs) of all the children based on the process ID.
int *fetchAllChildProcessID(int processID) {
  char procDirPath[200]; //// This character variable is declared to store the path of the Proc directory.
  DIR *directoryPath; // Declared the directory path pointer.
  struct dirent *directoryEntryPath; // directoryEntryPath is pointer of type struct dirent which is declared for directory entry structure.
  int childProcessID, childParentProcessID; // Storing process IDs when iterating later in the function logic.
  int childCountIndex = 0; // For incrementing the index for child process ids once processed.

  sprintf(procDirPath, "/proc/"); // In this line, sprintf function is used to format and store sequence of characters which is a path to the Proc directory.

  if ((directoryPath = opendir(procDirPath)) == NULL) {  // Checks if issue with the opening of Proc directory.
    printf("As of now the process is not running: %s\n", procDirPath); // In case the process is inactive.
    return 0;
  }

  while ((directoryEntryPath = readdir(directoryPath)) != NULL) { // Checks in case the directory Path is not NULL.
    if (directoryEntryPath->d_type == DT_DIR) { // Checks if the entry is a directory with help of DT_DIR.
      if (!isdigit(directoryEntryPath->d_name[0])) // It will be skipping all the non digit folders.
        continue; 
      childProcessID = atoi(directoryEntryPath->d_name); // Converting the directory entry name to integer which is a process id.
      if (childProcessID == 1) // It will be skipping the init process folder.
        continue; 
      if (childProcessID == processID) // It will be skipping the process folder if it matches with own ID.
        continue;

	  if(kill(childProcessID,0)!=0) // It will check for the existence of the process.
		continue;

      childParentProcessID = getParentProcessID(childProcessID); // Obtaining the parent process ID of child using getParentProcessID function.

      if (childParentProcessID == processID) { // In case the process id matches with the parent process ID of child.
        childProcessIDs[childCountIndex] = childProcessID; // Then assiging the process ID of child to the array of childProcessIDs.
        childCountIndex++; // Incrementing the counter for childCountIndex variable.
      }}}

  closedir(directoryPath); // Closing the Proc directory path before function exit and return statement.
  return childProcessIDs; // Returning the array of child process IDs.
  }


// This function is defined to obtain the elapsed time for the specific process id in minutes.
int calculateElapsedTime(int processID) {
  char statFileName[200]; // For storing stat file path.
  char lineContent[100]; // For storing the content from the stat file.
  char *strPtr;
  FILE *filePtr;
  int clockTicksNum;
  struct timespec timeInNS; // timeInNS is of type struct timespec to save time in nanoseconds with precision.

  sprintf(statFileName, "/proc/%d/stat", processID); // It will create the file path  /proc/[processID]/stat to read process information.
  filePtr = fopen(statFileName, "r");
  if (filePtr == NULL) {
    printf(" The provided process ID(processID) %d is not running currently.\n", processID);
    return -1;
  }

  // Read the first line from the stat file for proces information.
  if (fgets(lineContent, 100, filePtr) == NULL) { 
    perror("Error in reading the content of the file."); // For printing error message to standard error stream(stderr).
    fclose(filePtr); // Closing the file pointer that points to the stat file.
    return -1;
  }
  
  // It will tokenize the fetched line to extract clock ticks for further processing.
  strPtr = strtok(lineContent, " "); 
  for (int i = 1; i < 22; i++) {
    strPtr = strtok(NULL, " ");
  }
  
  clockTicksNum = atoi(strPtr); // Converting strPtr value to integer.

  fclose(filePtr); // Closing the stat File.

  // Obtaining the current time using CLOCK_MONOTONIC  and calculating the elapsed time
  if (clock_gettime(CLOCK_MONOTONIC, &timeInNS) == -1) {
    perror("Error of clock gettime"); // Displays error in case there is an issue in getting the current time.
    return -1; 
  }
  int timeInMins = (int)(timeInNS.tv_sec - (clockTicksNum / sysconf(_SC_CLK_TCK))) / 60; // Returns time in minutes.
  return timeInMins; //Returning Time in Minutes.
  }

// This function is created to check if the array childProcessIDs has children or not (greater than 0(meaning process ID exists and valid))
int tocheckForChildren(int childProcessIDs[]) {
  for (int i = 0; i < MAXIMUM_CHILD; i++) {
    if (childProcessIDs[i] > 0) // Process IDs are always greater than 0 so checking for that.
      return 1; // When the child process ID is found, 1 is returned.
  }
  return 0; // When the child process ID is not found, 0 is returned
}

// This function is created to kill specific process using SIGKILL signal.
void toTerminateProcess(int processID) {
  if (processID == bashdProcessID) { // Checking if the process ID is requested for terminate is bash process than it won't terminate.
    printf("\nThis bash process ID %d is not killed. \n",processPID);
    return; // Returning without killing the process ID.
  }
  if (processID == processPID) { // Checking if the process ID is requested for terminate matched with the process itself than it won't terminate.
    printf("\nThis process ID %d is not killed. \n", processPID);
    return; // Returning without killing the process ID.
  }
  printf("Killing this PID: %d\n", processPID); // This print statement will show that the process ID is being killed.
  kill(processPID, SIGKILL); // It will terminate the provided process ID with the SIGKILL signal.
  printf("%d process id is killed successfully.\n",processPID);

}

void searchingForProcessTree(pid_t processID, int treeDepth, int processElapsedTime,int numOfDefuncts) {
  if (treeDepth >= MAXIMUM_DEPTH) { // In case the tree depth is more than specified depth.
    printf("You have Reached maximum process tree depth.\n");
    return;
  }

  int *childPIDs;
  int localChildArr[MAXIMUM_CHILD], countOfDefunct = 0, elapsedTimeVal = 0;

  elapsedTimeVal = calculateElapsedTime(processID); // Obtaining elapsed time of parent
  childPIDs = fetchAllChildProcessID(processID); // Fetching the children of specific process ID.
  memcpy(localChildArr, childPIDs, MAXIMUM_CHILD * sizeof(int)); // Copyingthe process IDs of children to localChildArr.
  memset(childProcessIDs, 0, sizeof(childProcessIDs));    // Clearing the global array of child process IDs.

  if (!tocheckForChildren(localChildArr)) // Checking if there is any children in localChildArr.
    return;

  // Counting the number of defunct children.
  if (DEBUG_MODE_IS_ACTIVE) {  // Prints only if enabled.
    printf("\nThere are child Processes of %d:\n", processID);
  }
  for (int i = 0; i < 128; i++) {
    if (localChildArr[i] == 0)
      continue; // Not considering zero values.
    if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
      printf("%d", localChildArr[i]);
    }
	
	if (isProcessDefunct(localChildArr[i])) { // In case the process is DEFUNCT.
      countOfDefunct++;
      if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
        printf("The Child is DEFUNCT\n");
      }
    } else {
      }

    searchingForProcessTree(localChildArr[i], treeDepth + 1, processElapsedTime, numOfDefuncts); // Recursive call is made using DFS algorthm to check for Defunct children count.
  }

  if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
    printf("\nProcess %d has %d defunct children and %d minutes elapsed time\n", 
           processID, countOfDefunct, elapsedTimeVal);
  }

  // Terminating parent process with elapsed time > processElapsedTime
  if (processElapsedTime > 0 && countOfDefunct > 0) { // In case the parent process has more elapsed time than the process elapsed time.
    if (elapsedTimeVal > processElapsedTime) {
      toTerminateProcess(processID); // Calling function to terminate.
      return;
    }}

  // Terminating the parent process in case the number of defunct child is greater than numOfDefuncts
  else if (numOfDefuncts > 0 && countOfDefunct > 0) {
    if (countOfDefunct >= numOfDefuncts) {
      toTerminateProcess(processID); // Calling function to terminate.
      return;
    }}

  // Terminating all of the parent processes.
  else if (processElapsedTime == 0 && numOfDefuncts == 0 && countOfDefunct > 0) {
    toTerminateProcess(processID); // Calling function to terminate.
    return;
  }
  
  if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
    printf("\nThis %d is not killed.\n", processID);
  }}

int main(int argNumCount, char *argValueSupplied[]) {

  int PROC_ELTIME = 0, NO_OF_DFCS = 0;
  char *optionValue1;
  char *optionValue2;

  pid_t root_pid = 0;

  // Going through command line arguments passed by the user to parse.
  if (argNumCount < 2 && argNumCount > 5) {
    printf("Valid Usage: deftreeminus [root_process] [OPTION1] [OPTION2] [-processid]\n");
    return 1;
  }

  // Reading process id value of root process and converting to integer.
  root_pid = atoi(argValueSupplied[1]);

  if (root_pid <= 0) {
    printf("Invalid root process PID\n");
    return 1;
  }

	// Processing the arguments based on the required number of arguments supplied by the user.
  if (argNumCount == 3) { // [file_name] [root process] [- parent processes of defunct child not to be killed]
    processPID = atoi(argValueSupplied[2] + 1);
    if (processPID <= 0) {
      printf("This is invalid process ID.\n");
      return 1;
    }
  } else if (argNumCount == 4) { // [file_name] [root process] [Option Value 1 -b or -t] [option Value 2 NO_OF_DFCS or PROC_ELTIME(resp wrt Option value 1]
    optionValue1 = argValueSupplied[2];
    optionValue2 = argValueSupplied[3];
  } else if (argNumCount == 5) { // [file_name] [root process] [Option Value 1 -b or -t] [option Value 2 NO_OF_DFCS or PROC_ELTIME(resp wrt Option value 1] [- parent processes of defunct child not to be killed]
    optionValue1 = argValueSupplied[2];
    optionValue2 = argValueSupplied[3];
    processPID = atoi(argValueSupplied[4] + 1);
    if (processPID <= 0) {
      printf("This is invalid process ID.\n");
      return 1;}}

  if (argNumCount > 3) {
    if (DEBUG_MODE_IS_ACTIVE) { // Prints only if enabled.
      printf("Printing Passed Option Value 1: %s\n", optionValue1);
      printf("Printing Passsed Option Value 2: %s\n", optionValue2);}

    if (strcmp(optionValue1, "-t") == 0) { // Comparing Strings for argument "-t"
      PROC_ELTIME = atoi(optionValue2);
      if (PROC_ELTIME < 1) {
        printf("It is Invalid process Elapsed Time value.\n");
        return 1;}} 
	else if (strcmp(optionValue1, "-b") == 0) { // Comparing Strings for argument "-b"
      NO_OF_DFCS = atoi(optionValue2);
      if (NO_OF_DFCS < 1) {
        printf("Invalid numOfDefuncts value.\n");
        return 1;}} 
	  else {
      printf("Invalid option Value is supplied.\n");
      return 1;}}
  searchingForProcessTree(root_pid, 0, PROC_ELTIME, NO_OF_DFCS); // Searching for Process Tree.
  return 0;}
