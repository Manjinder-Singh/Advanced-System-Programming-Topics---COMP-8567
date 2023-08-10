#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>


int main ()
{
  int child_pid_1, child_pid_2;
  child_pid_1 = fork ();
  child_pid_2 = fork ();

  if (child_pid_1 == 0)
    {
	
      if (child_pid_2 == 0)
    {
        
		// Grandchild Process
        if (chdir("/home/manjinder/lab4") != 0)
        {
            printf("Issue arised while trying to modify the current directory.\n");
            return 1;
        }
        printf("The grandchild process has successfully changed the directory.\n");\
		
        // Creating sample.txt file with all the required permissions.
        int fd = open("sample.txt", O_CREAT | O_RDWR, 0777);
        if (fd < 0)
        {
            printf("Error during the creation of file\n");
            return 1;
        }

        // Closing the file before exiting.
        close(fd);
    }
      else
    {   
        // Child Process - 1
        // Waiting for the grandchild to finish its part
		int status_check;
        waitpid(child_pid_2, &status_check, 0);
        printf(" Child process 1 is completed wait time for the child process - 2. Now, replacing with ls Command.\n");
        
		// Differentiating and print the contents of /home/manjinder/lab4
        char *args[] = {"/bin/ls", "-1", "/home/manjinder/lab4", NULL};
        execv("/bin/ls", args);
    }
    }
  else
    {
      if (child_pid_2 == 0)
    {   
		// Child process 2
        // Differentiate child 2 is executing the ls command /home/manjinder
        printf(" Child process 2, replacing with ls\n");
        char *args[] = {"ls", "-1", "/home/manjinder", NULL};
        execl("/bin/ls", "ls", "-1", "/home/manjinder", (char *)NULL);
    }
      else
    {   
        //Main Process Logic
        int status_check1, status_check2;
        waitpid(child_pid_1, &status_check1, 0);
        waitpid(child_pid_2, &status_check2, 0);
        printf("The main process is completed.\n");
    }
    }
  exit (0);
}