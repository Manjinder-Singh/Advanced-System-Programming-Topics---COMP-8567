#include <stdio.h>

int smallest2(int *n1, int *n2, int *n3, int *n4);
int largest2(int *n1, int *n2, int *n3, int *n4);
int difference(int (*smallest2)(int *, int *, int *, int *), int (*largest2)(int *, int *, int *, int *), int *n1, int *n2, int *n3, int *n4);

int main() {
    int num1, num2, num3, num4;
    //Input from user for 4 numbers
    printf("1st Num ");
    scanf("%d", &num1);

    printf("2nd Num ");
    scanf("%d", &num2);

    printf("3rd Num ");
    scanf("%d", &num3);

    printf("4th Num ");
    scanf("%d", &num4);

    int result = difference(smallest2, largest2, &num1, &num2, &num3, &num4);
    printf("Difference between the 2nd smallest and 2nd largest number: %d\n", result);

    return 0;
}

//finding 2nd smallest number
int smallest2(int *n1, int *n2, int *n3, int *n4) {
    int smallest = *n1, second_smallest;

    if (*n2 < smallest) {
        second_smallest = smallest;
        smallest = *n2;
    } else {
        second_smallest = *n2;
    }

    if (*n3 < smallest) {
        second_smallest = smallest;
        smallest = *n3;
    } else if (*n3 < second_smallest) {
        second_smallest = *n3;
    }

    if (*n4 < smallest) {
        second_smallest = smallest;
        smallest = *n4;
    } else if (*n4 < second_smallest) {
        second_smallest = *n4;
    }

    return second_smallest;
}

//finding 2nd largest number
int largest2(int *n1, int *n2, int *n3, int *n4) {
    int largest = *n1, second_largest;

    if (*n2 > largest) {
        second_largest = largest;
        largest = *n2;
    } else {
        second_largest = *n2;
    }

    if (*n3 > largest) {
        second_largest = largest;
        largest = *n3;
    } else if (*n3 > second_largest) {
        second_largest = *n3;
    }

    if (*n4 > largest) {
        second_largest = largest;
        largest = *n4;
    } else if (*n4 > second_largest) {
        second_largest = *n4;
    }

    return second_largest;
}

//computing difference between 2nd smallest and 2nd largest
int difference(int (*smallest2)(int *, int *, int *, int *), int (*largest2)(int *, int *, int *, int *), int *n1, int *n2, int *n3, int *n4) {
    return largest2(n1, n2, n3, n4) - smallest2(n1, n2, n3, n4);
}
