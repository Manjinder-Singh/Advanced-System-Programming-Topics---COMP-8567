
// Manjinder Singh (110097177)
#include <fcntl.h> // For file control options.
#include <stdio.h> // For I/O Operations.
#include <stdlib.h> // For Process Control and Conversions.
#include <unistd.h> // For file and directory operations.

//Main function for lab 7
int main()
{
    int mss_fd1[2], mss_fd2[2];

    int mss_writeop = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if ((pipe(mss_fd1) == -1) || (pipe(mss_fd2) == -1) ) {
        puts("Issue encountered while creating the pipe.\n");
        exit(1);
    }

    // 1st fork will be handling the cat command.
    pid_t cat_pid = fork();
    if (cat_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (cat_pid == 0) {
        dup2(mss_fd1[1], STDOUT_FILENO);
        close(mss_fd1[0]);
        close(mss_fd1[1]);
        execlp("cat", "cat", "input.txt", NULL);
        puts("Issue encountered during the execution of cat command.\n");
        exit(1);
    }

    // 2nd fork will be handling the wc command.
    pid_t wc1_pid = fork();
    if (wc1_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (wc1_pid == 0) {
        dup2(mss_fd1[0], STDIN_FILENO);
        
        dup2(mss_fd2[1], STDOUT_FILENO);
        close(mss_fd1[1]);
        close(mss_fd2[0]);
        execlp("wc", "wc", NULL);
        puts("Issue encountered during the execution of wc command.\n");
        exit(1);
    }

    // 3rd fork will be handling the wc -wcommand
    pid_t wc2_pid = fork();
    if (wc2_pid == -1) {
        puts("Error encountered during forking.\n");
        exit(1);
    } else if (wc2_pid == 0) {
        dup2(mss_fd2[0], STDIN_FILENO);
        
        dup2(mss_writeop, STDOUT_FILENO);
        
        close(mss_fd1[0]);
        close(mss_fd1[1]);
        close(mss_fd2[1]);
        execlp("wc", "wc", "-w", NULL);
        puts("Error encountered while executing command");
        exit(1);
    }

    // Closing all file descriptors in the main parent process.
    close(mss_fd1[0]);
    close(mss_fd1[1]);
    close(mss_fd2[0]);
    close(mss_fd2[1]);
   // close(mss_writeop);

    // Waiting for all the child processes to finish with their execution part.
    waitpid(cat_pid, NULL, 0);
    waitpid(wc1_pid, NULL, 0);
    waitpid(wc2_pid, NULL, 0);

    return 0;
}
