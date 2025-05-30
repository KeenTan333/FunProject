#include <stdio.h>
#include <complex.h>
#include <math.h>

void f(double x, double *y, double *dy)
{
    // Function whose root we want to find, outputs also derivative of f(x) with complex step

    // Perturbation
    double h = 1e-100; 
    // Create a complex number with perturbation
    double complex z = x + h * I; 
    
    // Evaluate function
    double complex out = (3 * z * z) - (2 * z) - 4; // Try out different functions here!

    // Extract real part
    *y = creal(out); 
    // Extract imag part
    *dy = cimag(out / h); 
}


int main()
{
    // Initialise variables
    double x0;
    double x1;
    double y; 
    double dy;
    double eps = 1;

    // Initial guess for the root
    printf("Initial guess: ");
    scanf("%lf", &x0);

    // Initialise iteration variables
    int iter = 0;

    // Convergence tolerance
    double tol = 1e-100;

    while (eps > tol)
    {
        // Evaluate function
        f(x0, &y, &dy);

        // Run Newton iteration
        x1 = x0 - y / dy;

        // Increment iteration count
        iter++;

        // Print iteration details
        printf("iter %d | x: %f, y: %f, dy: %e\n", iter, x0, y, dy);

        // Calculate convergence criterion
        eps = fabs(x1 - x0);

        // Update x0 to new value
        x0 = x1;
    }

    // Print final result
    printf("Root found: x = %f after %d iterations\n", x0, iter);
    printf("Function value at root: f(x) = %f\n", y);
    printf("Derivative at root: f'(x) = %f\n", dy);
    printf("=== End of program ===\n");
    
    return 0;
}