// Manjinder Singh (110097177)
#include <fcntl.h> // For file control options.
#include <stdio.h> // For I/O Operations.
#include <stdlib.h> // For Process Control and Conversions.
#include <unistd.h> // For file and directory operations.
#include <string.h> // For string operations.

// Main function for lab 7 Work - cat input.txt | grep COMP | wc | wc -w
int main(){
    int mss_fd1[2], mss_fd2[2], mss_fd3[2];
    int mss_writeop = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // Assigning appropriate permissions to output.txt
    if ((pipe(mss_fd1) == -1) || (pipe(mss_fd2) == -1) || (pipe(mss_fd3) == -1)) { 
        puts("Issue encountered while creating the pipe.\n"); 
        exit(1);
    }

    // 1st fork will be handling the cat command.
    pid_t cat_pid = fork();
    if (cat_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (cat_pid == 0) {
        dup2(mss_fd1[1], STDOUT_FILENO); // Assigns STDOUT_FILENO to file descriptor to mss_fd1[1].
        close(mss_fd1[0]); // Closing file descriptor mss_fd1[0]
        close(mss_fd1[1]);
        execlp("cat", "cat", "input.txt", NULL); // Passing required number of values to execlp.
        puts("Issue encountered during the execution of cat command.\n");
        exit(1);
    }

    // 2nd fork will be handling the grep command.
    pid_t grep_pid = fork();
    if (grep_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (grep_pid == 0) {
        dup2(mss_fd1[0], STDIN_FILENO); 
        dup2(mss_fd3[1], STDOUT_FILENO);
        close(mss_fd1[1]); 
        close(mss_fd3[0]);
        execlp("grep", "grep", "COMP", NULL); // Chks for "COMP" word in the file.
        puts("Issue encountered during the execution of grep command.\n");
        exit(1);
    }

    // 3rd fork will be handling the first wc command.
    pid_t wc1_pid = fork();
    if (wc1_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (wc1_pid == 0) {
        dup2(mss_fd3[0], STDIN_FILENO);
        dup2(mss_fd2[1], STDOUT_FILENO);
        // Closing required file descriptors as during forking, fds are also copied.
        close(mss_fd1[0]);
        close(mss_fd1[1]);
        close(mss_fd2[0]);
        close(mss_fd2[1]);
        close(mss_fd3[0]);
        close(mss_fd3[1]);
        execlp("wc", "wc", NULL); // Perfoming word operations on the previous output
        puts("Issue encountered during the execution of wc command.\n");
        exit(1);
    }

    // 4th fork will be handling the second wc command.
    pid_t wc2_pid = fork();
    if (wc2_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (wc2_pid == 0) {
        dup2(mss_fd2[0], STDIN_FILENO);
        dup2(mss_writeop, STDOUT_FILENO);
        close(mss_fd1[0]);
        close(mss_fd1[1]);
        close(mss_fd2[0]);
        close(mss_fd2[1]);
        close(mss_fd3[0]);
        close(mss_fd3[1]);
        execlp("wc", "wc", "-w", NULL);  // Counting "number of words" from previous output.
        puts("Issue encountered during the execution of wc -w command.\n");
        exit(1);
    }

    // Closing all file descriptors in the main parent process.
    close(mss_fd1[0]);
    close(mss_fd1[1]);
    close(mss_fd2[0]);
    close(mss_fd2[1]);
    close(mss_fd3[0]);
    close(mss_fd3[1]);
    
    // Waiting for all the child processes to finish with their execution part.
    waitpid(cat_pid, NULL, 0);
    waitpid(grep_pid, NULL, 0);
    waitpid(wc1_pid, NULL, 0);
    waitpid(wc2_pid, NULL, 0);

    return 0;
}