// Manjinder Singh - 110097177

//Importing required header files for lab6 Work
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int isNonDescendentTerminated = 0;
int myprocessID1, myprocessID2, myprocessID3;
int isNonDesPaused = 0;

// Handler declared for termination of signal in non descedents.
void nonDesTermSigHand(int sig)
{
    printf("Non Descendant process is terminating and its PID: %d  and PPID %d.\n ", getpid(), getppid());
    exit(0); // It will signify that it is a successful exit with 0 status code.
}

// Handler for termination of signal in the main process to terminate non descendent.
void mainTerminationSignalHandler(int sig)
{
    if (isNonDescendentTerminated == 0) // Checks if it is not terminated yet.
    {
        // Termination initiated for grand children processes using process group ID
        killpg(getpid(), SIGUSR2); //it will send signal to all processes under that group.
        kill(myprocessID1, SIGUSR2); // Utilizing SIGUSR2 for sending.
        kill(myprocessID2, SIGUSR2);
        kill(myprocessID3, SIGUSR2);
        isNonDescendentTerminated = 1; // Changing flag value after termination of non descedents.
    }
}

// Handler declared for signal flag in grand child
void nonDesSigFlagHand(int sig)
{
    isNonDesPaused = 1;
}

// Handler declared for SIGCONT signal in immediate child process
void immediateChildSigContHandler(int sig)
{
    isNonDesPaused = 1;
}

int main()
{
    int mainProcessId = getpid();

    // Set process group ID as its own PID
    if (setpgid(getpid(), mainProcessId) == -1)
    {
        perror("Error in setting setpgid.\n");
        exit(1);
    }

    myprocessID1 = fork();
    myprocessID2 = fork();
    myprocessID3 = fork();

    if (myprocessID1 < 0 || myprocessID2 < 0 || myprocessID3 < 0) 
    {
        printf("Error duirng fork() calls.\n");
        exit(1); // exit with status code 1.
    }

    if (getpid() != mainProcessId)
    {
        // Ignoring Ctrl+C signal in descendants.
        signal(SIGINT, SIG_IGN);

        // Setting SIGUSR2 handler for interprocess communication.
        signal(SIGUSR2, nonDesTermSigHand);

        // Group immediate descedents processes under the main process.
        if (getppid() == mainProcessId)
        {
            signal(SIGUSR2, nonDesSigFlagHand);
            if (setpgid(getpid(), getpid()) == -1)
            {
                perror("Error in setting setpgid.\n");
                exit(1); // exit with status code 1.
            }
        }

        while (1) // Infinite Loop 
        {
            printf("From process ID:- %d.\n", getpid());
            if (isNonDesPaused == 0)
            {
                sleep(2); // sleeps for 2 secs and then continues.
            }
            else
            {
                sleep(8);
            }
        }
    }
    else
    {
        // Parent process

        // Set main process termination signal handler
        signal(SIGINT, mainTerminationSignalHandler);

        // Ignore SIGUSR2 in parent
        signal(SIGUSR2, SIG_IGN);

        // Ignore SIGCONT in parent
        signal(SIGCONT, SIG_IGN);
        sleep(2); // sleeps for 2 secs and then continues.

        for(;;)
        {
            sleep(2); // sleeps for 2 secs and then continues.
			
            if (isNonDescendentTerminated != 0)
            {
                // Processes that are not descendents of the main process will be terminated.
                printf("From Main Process ID:-> %d.\n", getpid());
                sleep(2); // sleeps for 2 secs and then continues with the kill signal.
                kill(myprocessID1, SIGCONT);
                sleep(2); // sleeps for 2 secs and then continues with the kill signal.
                kill(myprocessID1, SIGSTOP);
                kill(myprocessID2, SIGCONT);
                sleep(2); // sleeps for 2 secs and then continues with the kill signal.
                kill(myprocessID2, SIGSTOP);
                kill(myprocessID3, SIGCONT);
                sleep(2); // sleeps for 2 secs and then continues with the kill signal.
                kill(myprocessID3, SIGSTOP);
            }
        }
    }

    return 0;
}
// 110097177(Student ID)
