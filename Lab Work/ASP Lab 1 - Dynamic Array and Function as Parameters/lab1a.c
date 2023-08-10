#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;

    // User Input for Size
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Dynamically allocate memory here
    int* my_array = (int*)malloc(n * sizeof(int));

    //Input
    printf("Enter the elements of the array:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", my_array + i);
    }

    //Printing elements using dereferencing
    printf("Array elements in reverse order using derefrencing:\n");
    for (i = n - 1; i >= 0; i--) {
        printf("%d ", *(my_array + i));
    }

    return 0;
}