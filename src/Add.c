#include <stdio.h>

int main()
{
    // Initializes variables
    float A; 
    float B; 
    float C;

    printf("=== Calculating A + B ===\n");
    printf("A:");
    scanf("%f", &A); // Reads an integer from user input and stores it in A.
    getchar(); // Waits for user to input. 
    printf("B:");
    scanf("%f", &B); // Reads an integer from user input and stores it in A.

    // Calculates the sum of A and B
    C = A + B; // Store the sum in C
    printf("A + B = %f\n", C); // Output sum
    printf("=== End of program ===\n");
    return 0;
}