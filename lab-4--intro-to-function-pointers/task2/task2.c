#include <stdio.h>
#include <stdlib.h>

/* Define function prototypes */
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

/* Function pointer type */
typedef int (*Operation)(int, int);

int main(void)
{
    int a = 6, b = 3;
    Operation operations[] = {add, subtract, multiply, divide};

    while (1)
    {
        printf("Operand 'a' : %d | Operand 'b' : %d\n", a, b);
        printf("Specify the operation to perform (0 : add | 1 : subtract | 2 : Multiply | 3 : divide | 4 : exit): ");

        int choice;
        scanf("%d", &choice);

        /* Check for the exit condition */
        if (choice == 4)
        {
            printf("Exiting program\n");
            break;
        }

        /* Validate the choice */
        if (choice < 0 || choice >= sizeof(operations) / sizeof(operations[0]))
        {
            printf("Invalid choice. Please enter a valid operation.\n");
            continue;
        }

        /* Perform the chosen operation using the function pointer */
        int result = operations[choice](a, b);
        printf("x = %d\n", result);
    }

    return 0;
}

/* Define the functions */
int add(int a, int b)
{
    printf("Adding 'a' and 'b'\n");
    return a + b;
}

int subtract(int a, int b)
{
    printf("Subtracting 'b' from 'a'\n");
    return a - b;
}

int multiply(int a, int b)
{
    printf("Multiplying 'a' and 'b'\n");
    return a * b;
}

int divide(int a, int b)
{
    if (b == 0)
    {
        printf("Cannot divide by zero\n");
        return 0; // Returning 0 in case of division by zero
    }

    printf("Dividing 'a' by 'b'\n");
    return a / b;
}
