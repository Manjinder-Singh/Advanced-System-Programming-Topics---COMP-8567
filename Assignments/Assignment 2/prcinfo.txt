// Name - Manjinder Singh || Student ID - 110097177 || Section - 4
// Assignment - 2 || Question - 1 Solution

// Including required header files for the execution of the program(assignment question - 1)
#include <stdio.h> // Used to handle I/O(scanf,printf) functions for assi
#include <dirent.h> // Used to execute directory(readdir,opendir) functions.
#include <ctype.h> // Used to execute character(tolower,isstring) functions.
#include <stdbool.h> // Used to handle boolean(true,false) data type.
#include <string.h> // Used to handle string(strlen,strcmp) manipulations functions.
#include <unistd.h> // Used to execute POSIX operating system for API functions.
#include <stdlib.h> // Used to handle library(of type standard(free,atoi)) funcions.


// #define(pre processor directives) is used in C programming language to define macros or constants.
// Below are the two constants declared to be used for the execution of first problem statement of assignment question 1
#define MAXIMUM_PROCESSES_ALLOWED 5 // Constant declared to check so that maximum number of processes restricted to only 5.
#define DEBUG_MODE_IS_ACTIVE 0 // It will be used to enable and disable debug mode during execution. it is used with if(debug mode) sets to Value 1 means True and 0 means false(will not show print statement).

//Valid Usage: \nprcinfo [root_process] [process_id1] [process_id2] [process_id3]… [process_id(n)] [OPTION]\n
        
// This function is created to check if the provided process id is a descendant of a specified root process ID. 
int checksIfProcessIsDescendentOf(int processID, int rootProcessID) {
  char filePath[512]; // Declared character array of size 512 to store the file of the path.
  int parentProcessID; // Declared to store the process id of parent to display later.
  FILE *processStatus; //Declared a file pointer to access the process status file

  if (DEBUG_MODE_IS_ACTIVE)  // It will be executed if the flag for debug mode will be enabled.
    printf("\nVerfiying if the process is descendant of root PID %s\n", filePath);
  
  // Executed until the processID is not matched with the rootProcessID i.e. it will check if processID is descendent of rootProcessID.
  while (processID != rootProcessID) {
    sprintf(filePath, "/proc/%d/stat", processID); // In this line, sprintf function is used to format and store sequence of characters which is a path to the Stat file with the specific process id.

    if (DEBUG_MODE_IS_ACTIVE) // Goes into this loop if value is set to 1.
      printf("Opening %s\n", filePath);
    
    // It will open the proc directory entry of process in read mode only.
    processStatus = fopen(filePath, "r");
    if (!processStatus) { // It tries to open the file Path, if it is not opening then the chances are process is not running.
      printf("\nThis particular Process ID %d is not running\n", processID);
      return 0;
    }

    // Now, fetching the  Parent Process ID from the proc directory entry after skipping the first 3 entries from the file.
    fscanf(processStatus, "%*d %*s %*c %d", &parentProcessID);
    fclose(processStatus); //After reading is done, closing the processStatus file.

    if (DEBUG_MODE_IS_ACTIVE) // It will be executed if value is set to 1.
      printf("Printing the Parent Process ID: %d\n", parentProcessID);
    
    // Checks if the rootProcessID matched with the parentProcess then returning the value 1, meaning process is a descendant of root process.
    if (parentProcessID == rootProcessID) {
      return 1;
    }

    if (parentProcessID == 1) {// Checks if the parentProcessID is 1 i.e. assigned to init.
      return 0;
    }

    //  parentProcessID is assigned ot processID.
    processID = parentProcessID;
  }

  // It will return 1 if the process is a descendant after checking the conditons.
  return 1;
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
    if (DEBUG_MODE_IS_ACTIVE) {
      printf("\n Line Read Currently  : %s", statusOfLine); // To print each line of the file which is read one by one.
    }
    if (strncmp(statusOfLine, "State:", 6) == 0) { // It will compare the first 6 charcaters of status of line with the state.
      statusOfZombieProcess = statusOfLine + 7; // If it is matched then, getting the Status text written after "State:".
      if (DEBUG_MODE_IS_ACTIVE) {
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

// This function is used to retrieve the parent process id and grandparent process id from Process ID(PID).
int getParentAndGrandParentProcessID(int processID) {
  int parentProcessID, grandParentProcessID; // Declared 2 variables to store PIDs of parent and grandparent.
  parentProcessID = getParentProcessID(processID); // Gets parent process ID from process ID.
  grandParentProcessID = getParentProcessID(parentProcessID); // Gets grandparent process ID from PArent process ID.
  return grandParentProcessID; // Returns grandparent process ID at the end of function execution.
}

// This function lists all the sibling processes based on the process ID.
void enlistSiblingProcesses(int processID, int parentProcessID) {
  char procFolderPath[150]; // Character variable declared to store the /proc directory.
  DIR *directoryPath; // Declared the directory path pointer.
  struct dirent *directoryEntryPath; // Declared for directory entry structure.
  int siblingProcessID, siblingParentProcessID;
  sprintf(procFolderPath, "/proc/"); // Sprintf function is used to set proc folder path.
  if (DEBUG_MODE_IS_ACTIVE) {
    printf("Printing Proc Folder Path:- %s\n", procFolderPath);
  }

  if ((directoryPath = opendir(procFolderPath)) == NULL) { //Checks if it fails to open the proc folder path.
    printf("Failed to open the Proc directory: %s\n", procFolderPath);
    return;
  }

  printf("\nEnlisting Sibling Processes of %d Process ID:-",processID);
  while ((directoryEntryPath = readdir(directoryPath)) != NULL) { //readdir is used to iterate through each entry directory.
    if (directoryEntryPath->d_type == DT_DIR) { // It goes inside the loop if the entry is a directory with the help of DT_DIR.
      if (!isdigit(directoryEntryPath->d_name[0])) // It checks if the directory name starts with a number.
        continue; // skipping the non-digit folders as it is not required for our code logic.
      siblingProcessID = atoi(directoryEntryPath->d_name);
      if (siblingProcessID == 1)
        continue; // skipping init process directory.
      if (siblingProcessID == processID)
        continue; // skip the process itself is it matches with its own id.

      siblingParentProcessID = getParentProcessID(siblingProcessID); // Getting the sibling parent Process ID from sibling process ID.
      if (siblingParentProcessID == parentProcessID) { // If the sibling process is found then goes into the loop.
        printf("\nPID: %d\tPPID: %d\n", siblingProcessID, siblingParentProcessID); //Pritning required Ids as per function logic requirements.
      }
    }
  }
  closedir(directoryPath); // Closing the directory before exitting function.
}

// This function enlists all the defunct sibling processes based on the process ID.
void enlistDefunctSiblingProcesses(int processID, int parentProcessID) {
  char procFolderPath[150]; // Character variable 'procFolderPath' declared to store the /proc directory.
  DIR *directoryPath; // Declared the directory path pointer.
  struct dirent *directoryEntryPath; // Declared for directory entry structure.
  int siblingProcessID, siblingParentProcessID; // Declared required integer varibles for storing ids of sibling process and their parent.
  sprintf(procFolderPath, "/proc/"); // Sprintf function is used to set the proc folder path.
  
  if (DEBUG_MODE_IS_ACTIVE) {
    printf("Printing Proc Folder Path as:- %s\n", procFolderPath);}

  if ((directoryPath = opendir(procFolderPath)) == NULL) { // Checks if it is unable to open the specific path.
    printf("Fails to open the directory:- %s\n", procFolderPath);
    return;}

  printf("\nEnlisting Defunct Sibling Processes as:- \n");
  while ((directoryEntryPath = readdir(directoryPath)) != NULL) { // Iterates through each entry directory
    if (directoryEntryPath->d_type == DT_DIR) {
      if (!isdigit(directoryEntryPath->d_name[0]))
        continue; // omitting the non-digit folders as it is not a part of our requirement.
      siblingProcessID = atoi(directoryEntryPath->d_name);
      if (siblingProcessID == 1)
        continue; // skipping the init directory path as it is not required for this case.
      if (siblingProcessID == processID)
        continue; // skips the process as it has the same ID.

       siblingParentProcessID = getParentProcessID(siblingProcessID); // Fetching the sibling parent Process ID from sibling process ID using getParentProcessID.
      if (siblingParentProcessID == parentProcessID &&
          isProcessDefunct(siblingProcessID)) { // found a defunct sibling process
        printf("PID: %d\tPPID: %d\n", siblingProcessID, siblingParentProcessID); // Displaying required IDs.
      }
    }
  }
  closedir(directoryPath); // Closing the directory before function exit.
}

// This function will enlist all of the child processes based on the process ID.
void enlistChildProcesses(int processID) {
  char procFolderPath[150];  // Character variable 'procFolderPath' declared to store the /proc directory.
  DIR *directoryPath; // Declared the directory path pointer.
  struct dirent *directoryEntryPath; // Declared for directory entry structure.
  int childProcessID, childParentProcessID; // Declared required integer varibles for storing ids of child process and their parent.
  sprintf(procFolderPath, "/proc/"); // Sprintf function is used to set the proc folder path.
  
  if (DEBUG_MODE_IS_ACTIVE) {
    printf("Printing the Proc Folder Path as:- %s\n", procFolderPath);}

  if ((directoryPath = opendir(procFolderPath)) == NULL) { // Prints statement if proc folder pathis empty.
    printf("Fails to open the directory:- %s\n", procFolderPath);
    return;}

  printf("\nEnlisting the Child Processes of %d process ID as:- \n", processID);
  while ((directoryEntryPath = readdir(directoryPath)) != NULL) { // Iterates through each entry directory in proc folder.
    if (directoryEntryPath->d_type == DT_DIR) { // It goes inside the loop if the entry is a directory with the help of DT_DIR.
      if (!isdigit(directoryEntryPath->d_name[0])) // Omits folder if it is non digit folders.
        continue; 
      childProcessID = atoi(directoryEntryPath->d_name); // It will get the child process id
      if (childProcessID == 1) // Omits if it is a init folder.
        continue; 
      if (childProcessID == processID) // Omits if it is a process match with its own id.
        continue; 

      childParentProcessID = getParentProcessID(childProcessID); // Obtaining child process ID from getParentProcessID.

      if (childParentProcessID == processID) { 
        printf("PID: %d PPID: %d\n", childProcessID, childParentProcessID); // Printing required process IDs value.
      }}}
  closedir(directoryPath); // Closing the proc directory before function exit.
}

// This function will enlist all of the defunct(Zombie) child processes based on the process ID.
void enlistChildDefunctProcesses(int processID) {
    char procFolderPath[150];  // Character variable 'procFolderPath' declared to store the /proc directory.
    DIR *directoryPath; // Declared the directory path pointer.
    struct dirent *directoryEntryPath; // Declared for directory entry structure.
    int childProcessID, childParentProcessID; // Declared required integer varibles for storing ids of defunct sibling processes and their parent.
    
    sprintf(procFolderPath, "/proc/"); // Sprintf function is used to set the proc folder path.
    
    if (DEBUG_MODE_IS_ACTIVE) { 
        printf("Printing the Proc Folder Path as:-  %s\n", procFolderPath);}
    
    if ((directoryPath = opendir(procFolderPath)) == NULL) { // Displays print statement if there is an error in opening procFolderPath.
        printf("Fails to open the directory:- %s\n", procFolderPath);
        return;}
    
    printf("Printing the Defunct Child Processes as:- \n");
    
  while ((directoryEntryPath = readdir(directoryPath)) != NULL) { // Iterates through each entry directory in proc folder.
    if (directoryEntryPath->d_type == DT_DIR) { // It goes inside the loop if the entry is a directory with the help of DT_DIR.
            if (!isdigit(directoryEntryPath->d_name[0])) // Omits folder which are non digits in their name.
                continue;             
            childProcessID = atoi(directoryEntryPath->d_name); // It will convert the directory entry  which is nothing but a character array representing process ID.
            
            if (childProcessID == 1 || childProcessID == processID) // Omitting init process and its process ID is similar to itself.
                continue;
            
            childParentProcessID = getParentProcessID(childProcessID); // fetching the parent id of child process using getParentProcessID function.
            
             if (childParentProcessID == processID && isProcessDefunct(childProcessID)) { // It will check if the child process comes under defunct and has the given parent process ID.
                printf("PID: %d PPID: %d\n", childProcessID, childParentProcessID);
            }}}
    closedir(directoryPath); // Closing the proc directory before function exit.
}

// This function will enlist all the grandchild processes based on the provided process ID.
void enlistGrandChildProcesses(int pid) {
    char procFolderPath[150];  // Character variable 'procFolderPath' declared to store the /proc directory.
    DIR *directoryPath; // Declared the directory path pointer for proc folder.
    struct dirent *directoryEntryPath;  // Declared for directory entry structure.
    int childProcessID, childParentProcessID; // Declared required integer varibles for storing ids of  child processes and their parent process id.
    
    sprintf(procFolderPath, "/proc/"); // Sprintf function is used to set the proc folder path.
    
    if (DEBUG_MODE_IS_ACTIVE) {
        printf("Pritning the Proc Folder Path as:- %s\n", procFolderPath);}
    
    // Open the /proc directory
    if ((directoryPath = opendir(procFolderPath)) == NULL) { // Displays print statement if there is an error in opening procFolderPath.
        printf("Fails to open  the directory path:- %s\n", procFolderPath);
        return;}
    
    printf("Printing the Grandchild Process(es) of process ID %d.\n", pid);
    
    while ((directoryEntryPath = readdir(directoryPath)) != NULL) { // Iterates through each entry directory in proc folder.
        if (directoryEntryPath->d_type == DT_DIR) { // It goes inside the loop if the entry is a directory with the help of DT_DIR.
            if (!isdigit(directoryEntryPath->d_name[0]))
                continue; // Omits folder which are non-digit in nature.
            
            childProcessID = atoi(directoryEntryPath->d_name); // It will get the child process id and converts to integer type.
            
            if (childProcessID == 1 || childProcessID == pid) // Omitting init process and the process ID which is a parent process.
                continue;
            
            childParentProcessID = getParentProcessID(childProcessID); // fetching the parent id of child process using getParentProcessID function.
            
            if (childParentProcessID > 1 && getParentProcessID(childParentProcessID) == pid) { // It will check if the proces ID of parent is more than 1 and the provided aprent process ID is equivalent to the child process id.
                printf("PID: %d PPID: %d\n", childProcessID, childParentProcessID);
            }}} 
    closedir(directoryPath); // Closing the proc directory before function exit.
}

/* 
Function Purpose - This function will find all the children for process based on the process ID and takes 3 parameters:
1. parentProcessID - It is a parent process id.
2. childrenProcessIDs - It is an array to store the IDs of child process.
3. numberOfChildren - It is pointer to an integer to store the count of child processes found.
*/
void locateAllChildren(int parentProcessID, int *childrenProcessIDs, int *numberOfChildren) {
    DIR *procDirectoryPath = opendir("/proc");  // procDirectoryPath is a pointer to a DIR structure which opens the directory proc using opendir function.
    if (procDirectoryPath == NULL) { // If the operation of opening the proc directory is failed, it will proceed further to diaplay the error message.
        perror("Failed to open the proc directory path."); // Printing the descriptive error message using perror.
        return; 
    }
    
    struct dirent *directoryEntryPath;  // Declared for directory entry structure.
    while ((directoryEntryPath = readdir(procDirectoryPath)) != NULL) { // It will iterate the all directory entries int the proc folder.
        if (directoryEntryPath->d_type == DT_DIR &&
            strspn(directoryEntryPath->d_name, "0123456789") == strlen(directoryEntryPath->d_name)) { // It will check if the directory entry is a directory and if the name of directory consists only numbers and used to filter the directories that are not processes.
            int currentProcessID = atoi(directoryEntryPath->d_name); // assigning the process id value after converting the directory name to integer using atoi function.
            
            char statFilePath[512]; // Reading the stat file Path.
            snprintf(statFilePath, sizeof(statFilePath), "/proc/%d/stat", currentProcessID); // statFilePath is a character array which stores the stat file of the current process.
            FILE *statFileContent = fopen(statFilePath, "r"); // The file(stat file) is opened in read only mode .
            if (statFileContent != NULL) { // Proceeds further if statFile content is not null.
                int ppid;
                fscanf(statFileContent, "%*d %*s %*c %d", &ppid); //Reads the parent process id from the 4th place from the stat file content.
                fclose(statFileContent); // Closing the stat File before proceeding further.
                
                if (ppid == parentProcessID) { // Performing a check to make sure if the current process is a child of the parent process.
                    childrenProcessIDs[*numberOfChildren] = currentProcessID; // The current process id(child process id) is saved in the children at the index specified by the pointer numberOfChildren 
                    (*numberOfChildren)++; // After above line, numberOfChildren is increased to keep track the number of children found.
                }}}}
    closedir(procDirectoryPath); // Closing the proc directory before function exit.
}



void enlistNonDirectDescendents(int processID, int mainProcessID) {
    int *descendentsProcessIDs = NULL; // Pointer to an array(name - descendentsProcessIDs) of type integer to store the descendant Process IDs.
    int countOfDescendentsProcessIDs = 0; // variable declared to keep the track of the number of descendant process IDs.
    int *directChildProcessIDs = malloc(1024 * sizeof(int)); // Pointer to an array(name - directChildProcessIDs)of type integer  to store the direct child PIDs and allocating space using memory allocation function malloc.
    int countOfChildren = 0; // variable declared to find the number of direct child process ids

    locateAllChildren(processID, directChildProcessIDs, &countOfChildren); // Identifying the immediate child processes of the given process ID.

    for (int i = 0; i < countOfChildren; i++) { // Recursively finding nondirect descendants.
        int childProcessID = directChildProcessIDs[i];

        if (getParentProcessID(childProcessID) != mainProcessID) { // Checking if the each child PID is a direct child or a non-direct descendant by comparing its parent process ID.
            descendentsProcessIDs = realloc(descendentsProcessIDs, (countOfDescendentsProcessIDs + 1) * sizeof(int)); // If the child is a non direct descendant, the descendentsProcessIDs array is reallocated with realloc function to accomodate the new PID.
            descendentsProcessIDs[countOfDescendentsProcessIDs] = childProcessID; // The new process ID is added to the array.
            countOfDescendentsProcessIDs++;} // countOfDescendentsProcessIDs is incremented.
        enlistNonDirectDescendents(childProcessID, mainProcessID);} // It is called recursively for each child process ID to find their non direct descendants.
    free(directChildProcessIDs); // Free memory function is used to free up the memory.

		
    for (int i = 0; i < countOfDescendentsProcessIDs; i++) { // Printing the nondirect descendants process IDs by iterating through the descendentsProcessIDs array.
        printf("PID: %d\n", descendentsProcessIDs[i]);
 
        }

    free(descendentsProcessIDs); //releases the memory for the descendentsProcessIDs
}
// This function is used to validate if the string is  valid numeric string or not.
int ValidateNumericString(const char *numericString) {
    for (int i = 0; i < strlen(numericString); i++) { // Iterating over each character in the numericString string.
        if (!isdigit(numericString[i])) { // Checks if the current character at the particular index is not a digit.
            return 0; // If the condition evaluated to True mentioned in teh above IF loop then it will be returned.
	}}
    return 1; // Only returned If the loop finishes execution without the occurence of any non-didgit characters.
}

int main(int argumentCount, char *argumentValues[]) {
    int rootProcessID = 0; // For storing the root process ID.
    char *validOptions = NULL; // For storing the command line option to check.
    int countOfProcessIDs; // To track the number of process IDs provided. 

    if (argumentCount < 3 || argumentCount > 8) { // Executes this part in case the number of arguments entered by user on command line is less than 3 and greater than 8.
        printf("Valid Usage: \nprcinfo [root_process] [process_id1] [process_id2] [process_id3]… [process_id(n)] [OPTION]\n");
        return 1;
    }

    countOfProcessIDs = argumentCount - 2; // Computing the number of process ids(in this -2 is referring to the filename and option.

    if (argumentValues[argumentCount - 1][0] == '-') { //Finding if at this location Hypen(-) is present for depicting option value.
        validOptions = argumentValues[argumentCount - 1]; // It will fetch the commmand line option value
        countOfProcessIDs--;
    }

    if (countOfProcessIDs > MAXIMUM_PROCESSES_ALLOWED) { // It will execute if the process ids are entered more than the allowed limit of 5(in our case).
        printf("Limit is restricted to the 5 process ids(max).\n");
        return 1;
    }

    int *targetProcessIDsArrayPtr = (int *)malloc(countOfProcessIDs * sizeof(int)); // targetProcessIDsArrayPtr is an pointer to an integer array to store the process IDs.
    for (int i = 0; i < countOfProcessIDs; i++) { 
        char *targetProcessIDsStringPtr = argumentValues[i + 2];
        if (!ValidateNumericString(targetProcessIDsStringPtr)) { // Prints message if the process ID is not valid numeric string.
            printf("This is an invalid process ID '%s' .\n",targetProcessIDsStringPtr);
            free(targetProcessIDsArrayPtr); // Frees up the memory if invalid process ID exits.
            return 1;
        }
        targetProcessIDsArrayPtr[i] = atoi(targetProcessIDsStringPtr); // Converting process id of targetProcessIDsStringPtr to type integer.
        if (targetProcessIDsArrayPtr[i] < 1) { // Checks if the converted process id is less than 1.
            printf("This is an invalid process ID %d .\n",
                   targetProcessIDsArrayPtr[i]);
            free(targetProcessIDsArrayPtr); // Frees up the memory for if invalid process ID exits.
            return 1;
        }
    }

    char *rootProcessIDString = argumentValues[1];     // Fetching the root process ID from the values entered by user on terminal.
    if (!ValidateNumericString(rootProcessIDString)) { // Prints message if the process ID is not valid numeric string.
        printf("This is an invalid root process ID '%s' .\n", rootProcessIDString);
        free(targetProcessIDsArrayPtr); //frees up the memory if invalid process ID exits.
        return 1;
    }
    rootProcessID = atoi(rootProcessIDString); // Converting process id of targetProcessIDsStringPtr to type integer.
    if (rootProcessID < 1) { // Checks if the converted process id is less than 1.
        printf("This is an Invalid root process ID %d .\n", rootProcessID);
        free(targetProcessIDsArrayPtr); //frees up the memory if invalid process ID exits.
        return 1;
    }

    if (!checksIfProcessIsDescendentOf(rootProcessID, getppid())) { // Checks if the process is not a direct descendant of root process.
        printf("\nThe current bash process is not having a direct descendant of this %s root process \n",rootProcessIDString);
        return 1;
    }

	// Verifying if the target process is a descendant of the root process or not.
    printf("\nPerforming a check to make sure if the process ids are descendants of the root process or not.\n");
    for (int i = 0; i < countOfProcessIDs; i++) {
        if (!checksIfProcessIsDescendentOf(targetProcessIDsArrayPtr[i], rootProcessID)) {
            printf("\n%d - is a target process ID is not a descendant of the root process %d .\n",targetProcessIDsArrayPtr[i], rootProcessID);
        } else {
            // Irrespective of the options supplied by the user on terminal, printing process ID and parent process ID of the target process.
            printf("\n %d - is a target process ID belongs to the root process. ", targetProcessIDsArrayPtr[i]);
            printf("\nPID: %d\tPPID: %d\n", targetProcessIDsArrayPtr[i],getParentProcessID(targetProcessIDsArrayPtr[i]));
        }
    }

    if (validOptions != NULL) {
        if (strcmp(validOptions, "-dd") == 0) { // It will additionally(apart from process ID and parent process ID) lists the process IDs of all the immediate descendants of the process_id1.
            enlistChildProcesses(targetProcessIDsArrayPtr[0]);
        } else if (strcmp(validOptions, "-sb") == 0) { // It will additionally(apart from process ID and parent process ID) lists the process IDs of all the sibling processes of the process_id1.
            enlistSiblingProcesses(targetProcessIDsArrayPtr[0], getParentProcessID(targetProcessIDsArrayPtr[0]));
        } else if (strcmp(validOptions, "-gc") == 0) { // It will additionally(apart from process ID and parent process ID) lists the process IDs of all the grandchildren of the process_id1.
            enlistGrandChildProcesses(targetProcessIDsArrayPtr[0]);
        } else if (strcmp(validOptions, "-zz") == 0) { // It will additionally(apart from process ID and parent process ID) prints the status of the process_id1(Defunct/Not Defunct).
            if (isProcessDefunct(targetProcessIDsArrayPtr[0])) { // For DEFUNCT Process
                printf("\nThis is a DEFUNCT Process.\n");
            } else { // For NON-DEFUNCT Process
                printf("\nThis is a NON-DEFUNCT Process.\n");
            }
        } else if (strcmp(validOptions, "-zc") == 0) { // It will additionally(apart from process ID and parent process ID) lists the process IDs of all the direct descendants of the process_id1 that are currently in the defunct state.
            enlistChildDefunctProcesses(targetProcessIDsArrayPtr[0]);
        } else if (strcmp(validOptions, "-nd") == 0) { // It will additionally(apart from process ID and parent process ID) lists the process IDs of all the non-direct descendants of the process_id1.
            printf("\nNondirect/Indirect descendants of the process id %d. \n",targetProcessIDsArrayPtr[0]);
            enlistNonDirectDescendents(targetProcessIDsArrayPtr[0], targetProcessIDsArrayPtr[0]);
        } else if (strcmp(validOptions, "-sz") == 0) { // It will additionally(apart from process ID and parent process ID) lists the process IDs of all the sibling process of process_id1 that are defunct.
            enlistDefunctSiblingProcesses(targetProcessIDsArrayPtr[0], getParentProcessID(targetProcessIDsArrayPtr[0]));
        } else { // In case the user enters invalid option
            printf("Invalid option: %s\n", validOptions);
            return 1;
        }
    }
	free(targetProcessIDsArrayPtr); // Before final exit, cleaning up the memory.
    return 0;
}