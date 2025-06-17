# EECS3221_IntroUnixProg
# HW1 - Unix Programming and GDB Debugging Tutorial
**CS 238P Operating Systems**  
**Due Date:** [Insert due date]  
**Points:** 100

## Overview
This assignment is designed as a hands-on tutorial to familiarize you with building simple Unix programs and debugging them using GDB (GNU Debugger). You'll work through progressively complex examples while learning essential debugging techniques.

## Learning Objectives
By completing this assignment, you will:
- Build and compile C programs in a Unix environment
- Use Makefiles for project organization
- Master basic and intermediate GDB commands
- Debug common programming errors (segmentation faults, logic errors, memory issues)
- Understand program execution flow and memory layout
- Practice systematic debugging methodology

## Prerequisites
- Access to a Unix/Linux system (use lab machines or set up a VM)
- Basic C programming knowledge
- Text editor of your choice (vim, emacs, nano, or IDE)

## Part 1: Setting Up Your Environment (10 points)

### Task 1.1: Verify Your Tools
Create a simple "Hello World" program to ensure your development environment is working.

1. Create a file called `hello.c`:
```c
#include <stdio.h>

int main() {
    printf("Hello, Unix Programming!\n");
    return 0;
}
```

2. Compile and run:
```bash
gcc -o hello hello.c
./hello
```

3. Verify GDB is installed:
```bash
gdb --version
```

**Deliverable:** Screenshot showing successful compilation and GDB version.

## Part 2: Basic Unix Program with Makefile (20 points)

### Task 2.1: Create a Simple Calculator
Build a command-line calculator that performs basic arithmetic operations.

Create `calculator.c`:
```c
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
```

### Task 2.2: Create a Makefile
Create a `Makefile` for your calculator:

```makefile
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
TARGET=calculator
SOURCE=calculator.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: clean debug
```

**Deliverable:** Working calculator program with Makefile that compiles without warnings.

## Part 3: Introduction to GDB (25 points)

### Task 3.1: Basic GDB Commands
Using your calculator program, practice these GDB commands:

1. **Starting GDB:**
```bash
make debug
# or
gdb ./calculator
```

2. **Practice these commands inside GDB:**
```gdb
(gdb) help
(gdb) list
(gdb) break main
(gdb) run 10 + 5
(gdb) step
(gdb) next
(gdb) print num1
(gdb) print argv[1]
(gdb) info locals
(gdb) backtrace
(gdb) continue
(gdb) quit
```

### Task 3.2: Debugging Exercise
Create a buggy version of your calculator called `buggy_calc.c` with intentional errors:

```c
#include <stdio.h>
#include <stdlib.h>

int add(int a, int b) {
    return a + b;
}

int main(int argc, char *argv[]) {
    // Bug 1: Off-by-one error in argument check
    if (argc != 3) {  // Should be 4!
        printf("Usage: %s <num1> <operator> <num2>\n", argv[0]);
        return 1;
    }
    
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[3]);
    
    // Bug 2: Uninitialized variable
    int result;
    char op = argv[2][0];
    
    if (op == '+') {
        result = add(num1, num2);
    }
    // Bug 3: Missing else cases - result might be uninitialized
    
    printf("Result: %d\n", result);
    return 0;
}
```

Use GDB to:
1. Find and fix the argument count bug
2. Identify the uninitialized variable issue
3. Test with different inputs

**Deliverable:** Document the bugs you found and how GDB helped you identify them.

## Part 4: Advanced Debugging - Segmentation Faults (25 points)

### Task 4.1: Debug Memory Issues
Create `memory_bug.c` with common memory errors:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_string(char *str) {
    // Bug: No null check
    int len = strlen(str);
    printf("String length: %d\n", len);
    
    // Bug: Writing past array bounds
    char buffer[5];
    strcpy(buffer, str);  // Potential buffer overflow
    printf("Copied: %s\n", buffer);
}

int main() {
    char *ptr = NULL;
    
    printf("Testing with NULL pointer...\n");
    process_string(ptr);  // Will cause segfault
    
    printf("Testing with long string...\n");
    process_string("This string is way too long for the buffer");
    
    return 0;
}
```

### Task 4.2: Use GDB to Debug Crashes
1. Compile with debug symbols: `gcc -g -o memory_bug memory_bug.c`
2. Run in GDB: `gdb ./memory_bug`
3. When it crashes, use these commands:
```gdb
(gdb) run
(gdb) backtrace
(gdb) frame 0
(gdb) print str
(gdb) print ptr
(gdb) list
```

4. Fix the bugs and test again

**Deliverable:** Fixed version of `memory_bug.c` with explanations of what caused each crash.

## Part 5: Multi-file Project Debugging (20 points)

### Task 5.1: Create a Multi-file Project
Create a simple student grade manager:

**student.h:**
```c
#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];
    int id;
    float grade;
} Student;

void add_student(Student students[], int *count, const char *name, int id, float grade);
void print_students(Student students[], int count);
float calculate_average(Student students[], int count);
Student* find_student(Student students[], int count, int id);

#endif
```

**student.c:**
```c
#include <stdio.h>
#include <string.h>
#include "student.h"

void add_student(Student students[], int *count, const char *name, int id, float grade) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students\n");
        return;
    }
    
    strcpy(students[*count].name, name);
    students[*count].id = id;
    students[*count].grade = grade;
    (*count)++;
}

void print_students(Student students[], int count) {
    printf("\n--- Student List ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Grade: %.2f\n", 
               students[i].id, students[i].name, students[i].grade);
    }
}

float calculate_average(Student students[], int count) {
    if (count == 0) return 0.0;
    
    float sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += students[i].grade;
    }
    return sum / count;
}

Student* find_student(Student students[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            return &students[i];
        }
    }
    return NULL;
}
```

**main.c:**
```c
#include <stdio.h>
#include "student.h"

int main() {
    Student students[MAX_STUDENTS];
    int count = 0;
    
    // Add some students
    add_student(students, &count, "Alice Johnson", 101, 85.5);
    add_student(students, &count, "Bob Smith", 102, 92.0);
    add_student(students, &count, "Carol Davis", 103, 78.5);
    
    print_students(students, count);
    
    printf("\nClass average: %.2f\n", calculate_average(students, count));
    
    // Find a student
    Student *found = find_student(students, count, 102);
    if (found) {
        printf("Found student: %s with grade %.2f\n", found->name, found->grade);
    }
    
    return 0;
}
```

### Task 5.2: Advanced Makefile and Debugging
Create an advanced Makefile:

```makefile
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
TARGET=grade_manager
SOURCES=main.c student.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=student.h

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

debug: $(TARGET)
	gdb ./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

.PHONY: clean debug valgrind
```

Practice debugging across multiple files:
1. Set breakpoints in different files
2. Step through function calls between files
3. Examine the call stack
4. Use `info functions` and `info variables`

**Deliverable:** Working multi-file project with detailed debugging session log.

## Submission Requirements

Submit a ZIP file containing:

1. **Source Files:**
   - All `.c` and `.h` files from each part
   - All Makefiles
   - Fixed versions of buggy programs

2. **Documentation:**
   - `README.md` with compilation and running instructions
   - `debugging_log.txt` documenting your debugging sessions
   - Screenshots of successful compilations and program runs

3. **Reflection Report:**
   - Brief summary of what you learned about Unix programming
   - Most challenging debugging scenario and how you solved it
   - Comparison of debugging with and without GDB

## Grading Rubric

- **Part 1 (10%):** Environment setup and basic compilation
- **Part 2 (20%):** Calculator program and Makefile correctness
- **Part 3 (25%):** GDB command proficiency and bug identification
- **Part 4 (25%):** Memory debugging and crash analysis
- **Part 5 (20%):** Multi-file project and advanced debugging

## Tips for Success

1. **Read error messages carefully** - they often tell you exactly what's wrong
2. **Use incremental development** - build and test frequently
3. **Learn to read assembly** - use `disassemble` in GDB when confused
4. **Practice systematic debugging** - hypothesize, test, verify
5. **Use version control** - consider using git for your project

## Additional Resources

- GDB Quick Reference: `help` command within GDB
- GNU Make Manual: `man make`
- Valgrind for memory debugging: `man valgrind`
- Online GDB tutorial: [GNU GDB Documentation](https://www.gnu.org/software/gdb/documentation/)

## Academic Integrity

This is an individual assignment. You may discuss concepts with classmates, but all code must be your own work. Cite any external resources used.

---

**Good luck and happy debugging!** 🐛🔍
