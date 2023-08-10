
// Importing required header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int num1 = 10, num2 = 20, num3 = 30;
    int fd = open("sample.txt", O_CREAT | O_RDWR, 0644);
    
	// In case file fails to open.
    if (fd == -1) {
        perror("Failed to open sample.txt");
        return 1;
    }
    
	
    pid_t child1, child2, child3;
	
	//Forking 3 child as
    child1 = fork();
    child2 = fork();
	child3 = fork();
   
    if (child1 == -1 || child2 == -1 || child3 == -1) {
        perror("Failed to create a child process.");
        return 1;
    }
    else if (child1 == 0) {
        // Logic for Executing Child 1 process.
        
        
		if (fork() !=0){
			char *message = "COMP 8567\n";
			write(fd, message, strlen(message));
			for (int i = 0; i < 15; i++) {
				printf("Child 1 PID: %d, PPID: %d\n", getpid(), getppid());
				sleep(1);
			}
        }
        num1 += 10;
        num2 += 10;
        num3 += 10;
        
		printf("\n");
        printf("Child 1: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);
        
        close(fd); //closes file
        exit(0);
    }
    else if (child2 == 0) {
            // Logic for Executing Child 2 process
            
        
            if (fork() !=0){
				char *message = "COMP 8567\n";
				write(fd, message, strlen(message));
			for (int i = 0; i < 15; i++) {
				printf("Child 2 PID: %d, PPID: %d\n", getpid(), getppid());
				sleep(1);
				}
			}
            
            num1 += 10;
            num2 += 10;
            num3 += 10;
            
			printf("\n");
            printf("Child 2: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);
            
            close(fd);  //closes file
            exit(0);
        }
		else if (child3 == 0) {
        // Logic for Executing Child 3 process 
        
        
        if (fork() !=0){
			char *message = "COMP 8567\n";
			write(fd, message, strlen(message));
			for (int i = 0; i < 15; i++) {
				printf("Child 3 PID: %d, PPID: %d\n", getpid(), getppid());
				sleep(1);
			}
        }
        num1 += 10;
        num2 += 10;
        num3 += 10;
        
		printf("\n");
        printf("Child 3: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);
        
        close(fd);  //closes file
        exit(0);
    }
    
        else {
            // Logic for Parent process
            wait(NULL);
            printf("HELLO! FROM PARENT\n");
            close(fd);
            
            num1 += 25;
            num2 += 25;
            num3 += 25;
            
            printf("After Increment Values are - Parent: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);
        }
    
    int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            // Prints when the child process terminates after completing its processing.
            printf("Child process terminated after processing(WIFEXITED) with the exit status - %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            // Prints the message when the child process terminated even before teh completeion of its processing.
            printf("Child process terminated by signal(WIFSIGNALED) %d\n", WTERMSIG(status));
        }
        
    return 0;
}
