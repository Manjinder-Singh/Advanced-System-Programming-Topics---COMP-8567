/*ns
Name - Manjinder Singh
Student ID - 110097177
Lab Num - 10
Subject - Advanced System Programming
*/

// Importing the required libraries for lab number - 10.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int msCountVar = 6;
pthread_mutex_t msMutex = PTHREAD_MUTEX_INITIALIZER;

void* isThreadDetOrJoin(void* arg) {
    pthread_t msThreadID = pthread_self();
    int msDetachState = 0;

    // Checking the state of the thread if it is either joinable or detached.
    pthread_attr_t* msAttri = (pthread_attr_t*)arg;
    int msRetVal = pthread_attr_getdetachstate(msAttri, &msDetachState);
    
	if (msRetVal != 0) {
        perror("Issue Encountered related to pthread_attr_getdetachstate. Please check.");
        exit(EXIT_FAILURE); // Terminating with the failure status using EXIT_FAILURE.
    }

    pthread_mutex_lock(&msMutex); // Locking at the beginning.
    
	if (msCountVar > 0) {
        if (msDetachState == PTHREAD_CREATE_DETACHED) {
            msCountVar--;
            printf("Printing the ID of Detached Thread %ld-> Current Count Value-> %d\n", msThreadID, msCountVar);
        } else {
            printf("Printing the ID of Joinable Thread %ld-> Current Count Value-> %d\n", msThreadID, msCountVar);
        }
        sleep(1);
    }
    
	pthread_mutex_unlock(&msMutex); // Unlocking at this point

    // Adding a delay of 1.5 seconds in order to observe the detached threads behaviour.
    if (msDetachState == PTHREAD_CREATE_DETACHED) {
        //sleep(1.5);
    }

    return NULL;
}



int main() {
    pthread_t msThreads[6];
    pthread_attr_t msAttris[6];
    int currVal = 0;

    while (currVal < 6) 
	{
        int msRetVal = pthread_attr_init(&msAttris[currVal]);
        if (msRetVal != 0) 
		{
            perror("Issue Encountered related to pthread_attr_init. Please check.");
            exit(EXIT_FAILURE); // Terminating with the failure status using EXIT_FAILURE.
        }

        if (currVal % 2 == 0) 
		{
            // Thread creation in the "Joinable" state.
            msRetVal = pthread_attr_setdetachstate(&msAttris[currVal], PTHREAD_CREATE_JOINABLE);
        } 
		else 
		{
            // Thread creation in the "Detached" state.
            msRetVal = pthread_attr_setdetachstate(&msAttris[currVal], PTHREAD_CREATE_DETACHED);
        }
		
        if (msRetVal != 0) 
		{
            perror("Issue Encountered related to pthread_attr_setdetachstate. Please check.");
            exit(EXIT_FAILURE); // Terminating with the failure status using EXIT_FAILURE.
        }

        msRetVal = pthread_create(&msThreads[currVal], &msAttris[currVal], isThreadDetOrJoin, &msAttris[currVal]);
        if (msRetVal != 0) 
		{
            perror("Issue Encountered related to pthread_create. Please check.");
            exit(EXIT_FAILURE); // Terminating with the failure status using EXIT_FAILURE.
        }
        currVal++;
    }

    // Finally, waiting for "Joinable" threads to be done with its execution part.
    currVal = 0;
    while (currVal < 6) 
	{
        if (currVal % 2 == 0) 
		{
            int msRetVal = pthread_join(msThreads[currVal], NULL);
            if (msRetVal != 0) {
                perror("Issue Encountered related to pthread_join. Please check.");
            exit(EXIT_FAILURE); // Terminating with the failure status using EXIT_FAILURE.
            }
            msRetVal = pthread_attr_destroy(&msAttris[currVal]);
            if (msRetVal != 0) 
			{
                perror("Issue Encountered related to pthread_attr_destroy. Please check.");
            exit(EXIT_FAILURE); // Terminating with the failure status using EXIT_FAILURE.
            }
        }
        currVal++;
    }

    printf("Displaying the value of final count in the end-> %d\n", msCountVar);

    return 0;
}



