// Name - Manjinder Singh
// Section - 4
// Course Code and Subject Name - COMP8567-4-R-2023S|Advanced Systems Programming
// Student ID - 110097177  

/*
References - 
1. Class Code Files shared by DOC during lecture.
2. Illustrations shared on Brightspace for Assignment 1
3. Assignement PDF for Problem Statement.
4. NFTW Library Documentation. (https://linux.die.net/man/3/nftw)
*/

/* Steps to execute 
First we will execute on Terminal: gcc -o ncpmvdir ncpmvdir.c
Then we executed the below illustartions one by one:

./ncpmvdir ~sample1 ~/sample2 -cp
./ncpmvdir ~sample1 ~/sample2 -mv

./ncpmvdir ~sample1 ~/sample2 -cp pdf
./ncpmvdir ~sample1 ~/sample2 -mv java

./ncpmvdir ~sample1 ~/sample4/sample5 -cp
./ncpmvdir ~sample1 ~/sample4/sample5 -mv

./ncpmvdir ~sample1 ~/sample4/sample5 -cp pdf
./ncpmvdir ~sample1 ~/sample4/sample5 -mv java
*/

// Creating symbolic constants or macros.
#define MAXIMUM_EXTENSIONS 6 // Maximum 6 extensions are allowed.
#define MAXIMUM_DEPTH 64 // Maximum depth allowed is 64 for this program execution for directory traversal.
#define MAXIMUM_PATH_LENGTH 512 // Maxmimum path length is restricted to 512
#define DEBUG_MODE_ENABLED 1 //  It indicates about the debug mode. Its value can be either 0(Inactive) or 1(active).
#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1

// Including external header files.
#include <ftw.h> // Header file defined for nftw function which will be used during the execution of this program for traversing directory trees.
#include <sys/stat.h> // System specific header file which works with file and fiel system information.
#include <unistd.h> // Provides access to many operating system functionalities.
#include <libgen.h> // Provides functionality for manipulating file path.
#include <stdio.h> // Provides functions for Input and Output operations.
#include <stdlib.h> // Provides function for memory allocation and process control.
#include <string.h> // Provides functions for string(chr, cmp, len, cpy, cat) manipulation.


char * source_directory_path, * destination_directory_path, * home_directory; // Declaring 3 character pointers.
char resolved_source_directory_path[MAXIMUM_PATH_LENGTH], resolved_destination_directory_path[MAXIMUM_PATH_LENGTH], output_directory[2 * MAXIMUM_PATH_LENGTH], temporary_path[MAXIMUM_PATH_LENGTH]; // Declaring arrays for storage space to store sequence of characters.
int debug_mode = DEBUG_MODE_ENABLED; // Refering to symbolic constant declared on top of the program execution.

int mode = 0, number_of_extensions = 0; // Defining 2 integer variables. Mode value 0 defines inactive or default state and declaring number of extensions to zero. Latter variable will keep a check on the count of extensions during the program execution.
char * extensions[MAXIMUM_EXTENSIONS]; //Declaring an array of character pointers. It will be used to process files with specific extensions or perform operations based on different file types.

// Function created to log messages in log_file.txt(placed in the same directory as this file.)
void log_message(const char* logged_message) 
	{
    FILE* log_file = fopen("log_file.txt", "a"); // It will open the log file in append mode.
    if (log_file != NULL) 
	{
        fprintf(log_file, "%s\n\n", logged_message); // It will write the message to the log file in the end.
        fclose(log_file); // fclose() function will close the log file.
    }
	}

// Callback function for nftw to copy/move directories.
// Callback function for nftw which performs file treewalk.
int remove_directory_callback(const char * filepath,
   const struct stat * file_stat, int flag_type,
      struct FTW * ftwbuf) 
	  {
   // Delete the entire source file/directory
   if (debug_mode) {
      if (flag_type == FTW_F) // Executing if it is a regular file.
	  {
         printf("Removing a file present at this filepath: %s\n", filepath);
      } else if (flag_type == FTW_DP) // Executing if it is a directory.
	  {
         printf("Removing a directory present at this filepath: %s\n", filepath);
      } else
         printf("Removing from the filepath: %s\n", filepath);
   }
   remove(filepath); // For deleting the file or dir
   return 0;
}

// This function is resolving relative path by converting to absolute path.
char * resolve_relative_path(char * path) 
{
   char temporary[MAXIMUM_PATH_LENGTH]; // Temporary variable to store the path.
   char current_working_directory[MAXIMUM_PATH_LENGTH]; // for storing path of current working directory.
   char * pointer = NULL;
   size_t length;

   // Copy the path to temporary variable.
   snprintf(temporary, sizeof(temporary), "%s", path); //Copying input path using snprintf. Also, used for safely formatting a string into a fixed-sized character array.
   length = strlen(temporary); //strlen function of string library calculates the length of path saved in temporary variable.

   if (strncmp(temporary, "./", 2) == 0) // For relative path then converting to absolute path and then updating the new path.
   {
      getcwd(current_working_directory, sizeof(current_working_directory));
      sprintf(temporary_path, "%s%s", current_working_directory, temporary + 1);
   } else // If already a absolute path
      strcpy(temporary_path, temporary);
   
   // Replacing last part of path from "/" to "\0"  if "/" is found.
   length = strlen(temporary_path);
   if (temporary_path[length - 1] == '/')
	   temporary_path[length - 1] = '\0';
   return temporary_path;

}

// This function is recursively creating a directory folders/structure.
int directory_creation(const char * path) {
   // Temporary variable to store the path
   char temp[1024];
   char * p = NULL;
   size_t len;

   // Using snprintf to Copy the contents of the path argument into the temp array.
   snprintf(temp, sizeof(temp), "%s", path);
   len = strlen(temp);

   // Replacing the trailing '/' if any with 0 to remove trailing slash from the path.
   if (temp[len - 1] == '/')
      temp[len - 1] = 0;

   // Iterating through the each character of string to replace '/' with null character '\0'
   for (p = temp + 1;* p; p++)
      if ( * p == '/') {
         * p = 0; // Replace the '/' with null character
         // Create the directory with read, write, and execute permissions for the owner
		 if (debug_mode) {
		 log_message("Directory is created.");
            printf("Creating directory and directory path is: %s\n", temp);
         }
         mkdir(temp, S_IRWXU); //Creating a directory using mkdir function using READ, Write and Execute permission.
         * p = '/'; // Restoring the null character to its original value.
      }

   mkdir(temp, S_IRWXU); // For creating the last directory in the temp string, which represents the final directory in the path.

   // Return success i.e. 0
   return 0;
}

// This function is Verifying if a file has a particular extension to allow for operation(copy/move)
int check_extensions(const char * file_path) 
	{
	// The strrchr function is utilized in order to find the last occurrence of the character '.' in the file path. 	
   const char * extension = strrchr(file_path, '.');

   // In case no extension is found, 0 is returned.
   if (!extension) return 0;

   // Incrementing the extension pointer to skip the dot(.).
   extension++;
   
   // Iterating through extensions
   for (int i = 0; i < number_of_extensions; i++) 
   {
      if (strcmp(extension, extensions[i]) == 0) 
	  {
         // 1 is returned if Extension is found in the list.
         return 1;
      }
   }

   // 0 is returned if Extension is not found in the list.
   return 0;
}

int copy_move_directory_callback(const char * filepath,
   const struct stat * file_stat, int flag_type, struct FTW * ftwbuf) 
   {
   int creating_directory = 0;

   // Constructing the destination path
   char destination_path[4096];
   strcpy(destination_path, output_directory);
   strcat(destination_path, filepath + strlen(resolved_source_directory_path));

   // Leaving aside the files/folders that are not specified in the extension list.
   if (number_of_extensions > 0 && flag_type == FTW_F && check_extensions(filepath)) 
   {
      if (debug_mode) // It skips the files that are not in the extension list if debugging mode is enabled, it will print a message.
	  {
         printf("Skipping this filepath: %s\n", filepath);
      }
   }

   // 1. Creating a directory if flag type is FTW_D, 
   // 2. It will print few messages if DEBUG_MODE_ENABLED is set to 1.
   // 3. Then it will create a correponding directory in the destination path with appropriate permissions mentioned in the below logic.
   else if (flag_type == FTW_D) 
   {
      if (debug_mode) {
         printf("Copying filepath for the directory is: %s\n", filepath);
         printf("Destination path to which directory is copied: %s\n", destination_path);
      }
	  //Creating the directory with Read, Write, and Execute Permission for the owner.
      mkdir(destination_path, S_IRWXU);
   }

   // 1. Copying or moving a file if flag type is FTW_F(signifies regular file).
   // 2. It will print few messages if DEBUG_MODE_ENABLED is set to 1.
   // 3. It will copy the file by opening(using fopen) the source and destination files, reading the content of the source file character by character, and then proceeding with writing it to the destination file. 
   // 4. It then closes both files using fclose.
   else if (flag_type == FTW_F) 
   {
      if (debug_mode) 
	  {
         printf("Filepath Copied: %s\n", filepath);
         printf("Destination of Filepath: %s\n", destination_path);
      }
      FILE * source_file = fopen(filepath, "r");
      FILE * destination_file = fopen(destination_path, "w");
      int c;
      while ((c = fgetc(source_file)) != EOF) // Iterating until reaches end of file.
	  {
         fputc(c, destination_file);
      }
      fclose(source_file);
      fclose(destination_file);
   }
   return 0;
}


// MAIN FUNCTION
int main(int argc, char * argv[]) {
   umask(0); // File mode creation mask is set to zero which will allow all permissions to be set on newly created files as well as directories. 
   char * base;
   int source_directory_path_length;

   // Verifying if the correct number of arguments are passed or not.
   if (argc < 4) 
   {
   log_message("ERROR: Wrong command syntax used.");
      printf("Synopsis to use: directory_copy_move [source_directory] [destination_directory] [options] <extension list>\n");
      return 1;
   } 
   else if (argc > MAXIMUM_EXTENSIONS + 4) 
   {
	  log_message("Error: Maximum extensions limit exceeded.\n");
      printf("Error: Maximum %d extensions are permitted.\n", MAXIMUM_EXTENSIONS);
      return 1; 
   }
	
   // Verifying if the specified options are either -cp or -mv, then setting mode accordingly.
   if (strcmp(argv[3], "-cp") == 0) 
   {
      mode = 0;
	  log_message("Copy Mode : Mode 0 is set for Copy(mv) Operation.");
   } 
   else if (strcmp(argv[3], "-mv") == 0) 
   {
      mode = 1;
	  log_message("Move Mode : Mode 1 is set for Move(mv) Operation.");
   } else 
   {
	  log_message("Error: You have used Invalid option. You can either use -cp(for copy) or -mv(for moving file or directory).\n");
      printf("Error: You have used Invalid option. You can either use -cp(for copy) or -mv(for moving file or directory).\n");
      return 1;
   }

   // Verifying if the extension list is supplied by the user in the terminal command and adding it to the extensions array for future use for performing operations.
   for (int i = 4; i < argc; i++) 
   {
      extensions[number_of_extensions++] = argv[i];
   }
   
   // Getting the path of the home directory.
	home_directory = getenv("HOME");

	// Reading the source and destination directories from the input arguments supplied by user.
	source_directory_path = argv[1];
	destination_directory_path = argv[2];

	// Resolving the relative paths using the "resolve_relative_path" function for the source and destination directories.
	sprintf(resolved_source_directory_path, "%s", resolve_relative_path(source_directory_path));
    sprintf(resolved_destination_directory_path, "%s", resolve_relative_path(destination_directory_path));

	// Getting the path length using "strlen" function of the resolved source directory.
	source_directory_path_length = strlen(resolved_source_directory_path);

	// Verifying if the source directory exists or not.
	struct stat source_stat;
	if (stat(resolved_source_directory_path, &source_stat) == -1) 
	{
		log_message("Error: Source directory is not available.");
		perror("Error: Source directory is not available.");
		return 1;
	}

	// Verifying if the source and destination directories are the equivalent or if the destination is inside the source directory. - This condition prevents infinite loop and is not permissible.
	if (strncmp(resolved_source_directory_path, resolved_destination_directory_path, source_directory_path_length) == 0) 
	{
		log_message("Error: Source and destination directories are either in the same path or the destination is inside the source directory.\n");
		log_message("This is not allowed as it will lead to an infinite loop.\n");
		printf("Check for Error: Source and destination directories are either in the same path or the destination is inside the source directory.");
		return 1;
	}

	//Verifying for Destination Directory
	base = basename(resolved_source_directory_path);
	sprintf(output_directory, "%s/%s", resolved_destination_directory_path, base);

	// Verifying if both the source and destination directories belongs to the home directory hierarchy.
	if (strncmp(resolved_source_directory_path, home_directory, strlen(home_directory)) || strncmp(resolved_destination_directory_path, home_directory, strlen(home_directory))) 
	{
		log_message("Error: Both source and destination directories must be within the home directory hierarchy.\n");
		printf("Check for Error: Both source and destination directories must be within the home directory hierarchy.\n");
		return 1;
	}

	// Creating output directory if it does not exist using creating_directory function.
	directory_creation(resolved_destination_directory_path);

	// Copy or move the directory using C library nftw(New File Tree Walk).
	
	// If the nftw function returns a non-zero value which will eventually means that there is a failure in traversing the directory tree. In this case, an error message is logged into log file.
	if (nftw(resolved_source_directory_path, copy_move_directory_callback, MAXIMUM_DEPTH, FTW_PHYS) != 0) 
	{
		log_message("Check for Error: Error is related to nftw failure, please check.");
		return 1;
	}

	//If the mode is 1 that means the program is in move(mv) mode, and tries to remove the source directory using the nftw function. If it displays , it wil log an error message to the log file and return 1.
	if (mode == 1 && nftw(resolved_source_directory_path, remove_directory_callback, MAXIMUM_DEPTH, FTW_DEPTH | FTW_PHYS) != 0) 
	{
		printf("Check for nftw and logic for move mode.");
		log_message("Check for Error: Error related to the failure of nftw.");
		return 1;
	}
	return 0;
	}

/*
HANDLED ERROR CONDITIONS IN THE ABOVE PROGRAM:

1. Wrong command syntax
2. Maximum extensions limit exceeded
3. Invalid option(other than cp/mv)
4. Source directory not available
5. Source and destination directories lies in the same path or destination inside source directory(it will cause infinite loop.)
6. Source and destination directories not within the home directory hierarchy(source and destination lies in HOME directory)
7. nftw failure for directory traversal

Above mentioned error conditons are checked at different stages of the program execution and then appropriate messages are logged in a file and also, printed on terminal.

*/