#include <stdio.h>

int main()
{
    // Initialise variables
    float A; 
    float B; 
    float C;

    printf("=== Calculating A + B ===\n");
    printf("A:");
    scanf("%f", &A); // Reads a float from user input and stores it in A.
    getchar(); // Waits for user to input. 
    printf("B:");
    scanf("%f", &B); // Reads a float from user input and stores it in B.
    getchar(); // Waits for user to input. 

    // Calculate the sum of A and B
    C = A + B; // Store the sum in C
    printf("%f + %f = %f\n", A, B, C); // Output sum
    printf("=== End of program ===\n");
    return 0;
}