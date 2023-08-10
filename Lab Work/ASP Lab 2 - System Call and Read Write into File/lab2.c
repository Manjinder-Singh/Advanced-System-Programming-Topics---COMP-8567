// Importing packages for the execution of program for Lab - Day 3
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


int main() {

    // Initializing the umask to 0 for permissions as per requirements
    umask(0000);

    // Creating new file "new.txt" with open system call along with permission 0766
    int file_name = open("new.txt", O_CREAT | O_WRONLY | O_TRUNC, 0766);

	// Returning error if in case there is an issue during creation of new.txt file
    if (file_name == -1) {
        perror("There is an error while creating the file");
        return 1; //Returning 1 if there is an error
    }

    // Writing - Welcome to COMP 8567" to file "new.txt" file at Line 1 and using \n for end line part.
    char* mystring1 = "Welcome to COMP 8567\n";
    write(file_name, mystring1, strlen(mystring1));

    // Closing the "new.txt" file using teh close operation.
    close(file_name);

    // Opening the file "new.txt" in append mode so that new text can be added along with the existing data in the file.
    file_name = open("new.txt", O_WRONLY | O_APPEND); //

	// Returning error if in case there is an issue during opening of new.txt file
    if (file_name == -1) {
        perror("There is an error to open file in append mode");
        return 1; //Returning 1 if there is an error
    }

    // Adding the text - Advanced Systems Programming - after the existing text present in the new line of the file.
    char* mystring2 = "Advanced Systems Programming\n";
    write(file_name, mystring2, strlen(mystring2));

    // Adding the text - University of Windsor - after the existing text present in the new line of the file.
    char* mystring3 = "University of Windsor\n";
    write(file_name, mystring3, strlen(mystring3));

    // Closing the file - new.txt
    close(file_name);

    // Opening the file with name "new.txt" read & write mode with O_RDWR option
    file_name = open("new.txt", O_RDWR);

	// Returning error if in case there is an issue during opening of new.txt file in read-write mode
    if (file_name == -1) {
        perror("There is an error to open file in read-write mode");
        return 1;
    }

    // Using lseek to explicitly position at a file offset i.e writing characters at a particular positions in the file.
	// 2nd value is 10 which means to position 10 bytes and then using SEEK_SET at the starting of the line.
    lseek(file_name, 10, SEEK_SET);
    
	// Adding Character "####&&&&" in the file.
    write(file_name, "####&&&&", 8);

    // Positioning 12 bytes after the previous operation is performed.
    // Using SEEK_CUR for the current position of line
    lseek(file_name, 12, SEEK_CUR);

    // Adding Character "####&&&&" in the file.
    write(file_name, "####&&&&", 8);

    // Positioning to the 40 bytes from the end of file
	// Using SEEK_END to position from th end of file
    lseek(file_name, 40, SEEK_END);
    
	// Writing Character "####&&&&" to the file.
    write(file_name, "####&&&&", 8);

    // Replacing all NULL characters with the Letter 'N'
    lseek(file_name, 0, SEEK_SET);
    char replace_miss_char;
    while (read(file_name, &replace_miss_char, 1) != 0) {
        if (replace_miss_char == '\0') {
            lseek(file_name, -1, SEEK_CUR);
            write(file_name, "N", 1);
        }
    }

    // Closing the "new.txt" file.
    close(file_name);

    // Opening the "new.txt" file in read-only mode with O_RDONLY mode.
    file_name = open("new.txt", O_RDONLY);
    if (file_name == -1) {
        perror("There is an error to open the file in read-only mode");
        return 1;
    }

    // Displaying contents of the "new.txt" file from the 10 bytes.
    char my_read_char[11];
    ssize_t bytes_read;
    while ((bytes_read = read(file_name, my_read_char, 13)) > 0) {
        my_read_char[bytes_read] = '\0';
        printf("%s\n", my_read_char);

    }
	
    // Displaying the size of the file
    off_t file_size = lseek(file_name, 0, SEEK_END);
    printf("Size of the file : %ld bytes\n", file_size);

    // Closing the "new.txt" file.
    close(file_name);
    return 0;
}
