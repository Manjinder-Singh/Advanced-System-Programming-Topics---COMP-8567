
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define ALLOWED_ELEMENTS 100

// This fn generates random int values from 10 to 20 for an array.
int randNumGeneratorFn(int lowestElement, int highestElement) {
    return lowestElement + rand() % (highestElement - lowestElement + 1);
}

// This fn calculates the sum of all the available elements in array ia_1 to calculate sum using sum_1 fn.
void *sum_1(void *arg) {
    int *msArr = (int *)arg;
    int *ValueOfSum = (int *)malloc(sizeof(int));
    *ValueOfSum =0;

    // Performing addition on all elements of array.
    for (int tempVar = 0; tempVar < ALLOWED_ELEMENTS; tempVar++) {
        *ValueOfSum += msArr[tempVar];
    }
    // Returning the final added sum of array elements and exiting from thread using pthread_exit.
    pthread_exit((void *)(intptr_t)ValueOfSum);
}

// This fn calculates the sum of all the available elements in array ia_2 to calculate sum using sum_2 fn.
void *sum_2(void *arg) {
    int *msArr = (int *)arg;
    int *ValueOfSum = (int *)malloc(sizeof(int));
    *ValueOfSum =0;

    // Performing addition on all elements of array.
    for (int tempVar = 0; tempVar < ALLOWED_ELEMENTS; tempVar++) {
        *ValueOfSum += msArr[tempVar];
    }

    // Returning the final added sum of array elements and exiting from thread using pthread_exit.
    pthread_exit((void *)(intptr_t)ValueOfSum);
}

// This fn calculates the sum of all the available elements in array ia_3 to calculate sum using sum_3 fn.
void *sum_3(void *arg) {
    int *msArr = (int *)arg;
    int *ValueOfSum = (int *)malloc(sizeof(int));
    *ValueOfSum =0;
    // Performing addition on all elements of array.
    for (int tempVar = 0; tempVar < ALLOWED_ELEMENTS; tempVar++) {
        *ValueOfSum += msArr[tempVar];
    }
    // Returning the final added sum of array elements and exiting from thread using pthread_exit.
    pthread_exit((void *)(intptr_t)ValueOfSum);
}

int main() {
    // Performing Seed opr with curr time.
    srand(time(NULL));

    // Declaring 3 arrays with the name ia1,ia2,ia3.
    int ia1[ALLOWED_ELEMENTS], ia2[ALLOWED_ELEMENTS], ia3[ALLOWED_ELEMENTS];
    pthread_t msThread1, msThread2, msThread3;
    intptr_t *sum1, *sum2, *sum3;

    // Iterating array for generating random elements for all the 3 arrays.
    for (int tempVar = 0; tempVar < ALLOWED_ELEMENTS; tempVar++) {
        ia1[tempVar] = randNumGeneratorFn(10, 20);
        ia2[tempVar] = randNumGeneratorFn(10, 20);
        ia3[tempVar] = randNumGeneratorFn(10, 20);
    }

    // Creation of 3 Threads using pthread_create to perform addition of 100 elements by passing array with elements as arg.
    pthread_create(&msThread1, NULL, sum_1, (void *)ia1);
    pthread_create(&msThread2, NULL, sum_2, (void *)ia2);
    pthread_create(&msThread3, NULL, sum_3, (void *)ia3);

    // Waiting for threads using pthread_join fn so that sum operation is performed before final doing sum of all 3 arrays.
    pthread_join(msThread1, (void **)&sum1);
    pthread_join(msThread2, (void **)&sum2);
    pthread_join(msThread3, (void **)&sum3);

    printf("Value of sum from thread 1:-  %d\n",*sum1);
    printf("Value of sum from thread 2:- %d\n",*sum2);
    printf("Value of sum from thread 3:- %d\n",*sum3);
    
    // Doing final sum based on return value of thread.
    int sumOfThreeArr = *sum1 + *sum2 + *sum3;

    free(sum1);
    free(sum2);
    free(sum3);

    // Display the total sum of all elements in the arrays
    printf("Final Sum of 3 arrays: %d\n", sumOfThreeArr);

    return 0;
}
