// Lab - 5 || Manjinder Singh || ASP || 110097177

// Importing required header files.
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

int signalClickCount = 0;
time_t programStartTime;
int flag;

void signalHandlerInterrupt(int signalVal){
    time_t timeNow = time(NULL);
    // In case the current time and elapsed time of program is more than 5 secs then reset pogramStartTime
	if (timeNow - programStartTime > 5){ 
        signalClickCount = 1;
        programStartTime = timeNow;
		}
    // In case the current time and elapsed time of program is less than or equal to 5 secs then increment signalClickCount.
	else{ 
        signalClickCount++; 
        flag = 1;
		}
		if( signalClickCount ==1 && flag ==1){
			printf("Press Ctrl+C again to exit in 5 secs.\n");
		}
	// In case the signalClickCount value is 2 i.e. twice ctrl+C is pressed within 5 secs then exit
    if (signalClickCount == 2){
        printf("Exiting as twice Ctrl+C is pressed within 5 seconds.\n");
        exit(0);
		}
	}
	
void signa1HandlerALARM(int signalVal) {
    printf("Resetting signalClickCount when alarm is triggered.\n");
    signalClickCount = 0;
}

// Main Logic for the program
int main(void){
    signal(SIGINT, signalHandlerInterrupt); // Registering Signal Interrupt and declaring function for this with name "signalHandlerInterrupt"
	signal(SIGALRM, signa1HandlerALARM); // Registering Signal Alarm and declaring function for this with name "signa1HandlerAlarm"
    programStartTime = time(NULL);
    while (1){
        printf("Welcome to Lab5 - Signals.\n"); // Printing required message.
        sleep(1); //Sleeps for 1 second.
    }
    return 0;
}
