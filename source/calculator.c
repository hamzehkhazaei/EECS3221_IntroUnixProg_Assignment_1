#include <stdio.h>
#include <stdlib.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

float divide(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero!\n");
        return 0;
    }
    return (float)a / b;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <num1> <operator> <num2>\n", argv[0]);
        printf("Operators: +, -, *, /\n");
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[3]);
    char op = argv[2][0];

    switch(op) {
        case '+':
            printf("%d + %d = %d\n", num1, num2, add(num1, num2));
            break;
        case '-':
            printf("%d - %d = %d\n", num1, num2, subtract(num1, num2));
            break;
        case '*':
            printf("%d * %d = %d\n", num1, num2, multiply(num1, num2));
            break;
        case '/':
            printf("%d / %d = %.2f\n", num1, num2, divide(num1, num2));
            break;
        default:
            printf("Unknown operator: %c\n", op);
            return 1;
    }

    return 0;
}