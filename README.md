# EECS3221: Unix Programming and GDB Debugging Tutorial
**Due Date:** [Insert due date]  
**Points:** 100

## Overview
This assignment is designed as a hands-on tutorial to familiarize you with building simple Unix programs and debugging them using GDB (GNU Debugger). You'll work through progressively complex examples as you learn essential debugging techniques.

## Learning Objectives
By completing this assignment, you will:
- Build and compile C programs in a Unix environment
- Use Makefiles for project organization
- Master basic and intermediate GDB commands
- Debug common programming errors (segmentation faults, logic errors, memory issues)
- Understand program execution flow and memory layout
- Practice systematic debugging methodology

## Prerequisites
- Access to a Unix/Linux system (use EECS lab servers or set up a Unix-based VM)
- Basic C programming knowledge
- Text editor of your choice (vim, emacs, nano, or IDE)

## Part 1: Setting Up Your Environment (10 points)

### Task 1.1: Verify Your Tools
Create a simple "Hello World" program to ensure your development environment is working.

1. Create a file called `hello.c`:
```c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void) {
    
    printf("Hello world\n"); 
    return 0; 
}

```

2. Compile and run:
```bash
$ gcc -o hello hello.c
$ ./hello
```

3. Verify GDB is installed:
```bash
$ gdb --version
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
Create a `Makefile` by default name as `Makefile` for your calculator:

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
**Compiling and Running:**
```bash
$ make
$ ./calculator 8 + 9
```
**Note:** As can be seen in the line that defines `CFLAGS`, we have `-g`, which compiles your program with debugging symbols. This will allow us to start your program under the control of gdb.

**Deliverable:** Working calculator program with Makefile that compiles without warnings.

## Part 3: Introduction to GDB (25 points)

### Task 3.1: Basic GDB Commands
Using your calculator program, practice these GDB commands:

1. **Starting GDB:** Here you have two ways to start GDB for your `calculator` program.
```bash
$ make debug
# or
$ gdb ./calculator
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
GDB is a feature-rich debugger, and it will take you some time to learn all the features. While you don't need to be a master of GDB for this course, you may use AI tools to become more familiar with GDB, if interested.
At a high level, you need only two main things: 1) breakpoints and 2) the ability to examine data. Breakpoints can be set with the `b` command inside gdb.

### Task 3.2: Debugging Exercise
For this section, let us consider the following program:

```c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


unsigned long sum(int n) {
    int i;
    unsigned long sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + i;
    }
    return sum;
}
int main(void) {
    printf("Hello world\n"); 
    unsigned long s;
    s = sum(100);
    printf("Sum:%ld\n", s);
    return 0;
}

```
Let us compile the program with debugging symbols (`-g` flag):
```bash
$ gcc main.c -o hello -Wall -g 
```

Running the program independently is not particularly useful. Let's try setting a breakpoint on the "main" function to examine what the program is doing. 
Type break main in the GDB command prompt (or b for short) and then run the program with r.

```bash
(gdb) break main
Breakpoint 1 at 0x56b: file main.c, line 21.
(gdb) r
Starting program: ...  
Breakpoint 1, main () at main.c:21
26          s = sum(100);
(gdb)
```
The debugger stopped at the beginning of the `main` function (line 26 of main.c). You can examine the source code of the program by typing `list` (or l for short).

```bash
(gdb) list
16        sum = sum + i;
17    }
18    return sum;
19 }
20 int main(void) {
21    printf("Hello world\n"); 
22    unsigned long s;
23    s = sum(100);
24    printf("Sum:%ld\n", s);
25    return 0;
```

Now you can execute the program line by line by typing `next` (or' n' for short), which executes the following line. By default, typing `next` will skip over functions. Type `step` (or s for short) to step into a function. 
Try stepping into the `sum` function by running `step`.

```bash
(gdb) s
sum (n=100) at main.c:14
14    unsigned long sum = 0;
```
We are now inside the `sum` function. Type `l` to list the source code, and then type `n` repeatedly to execute the function line by line. 
Note that we can also type `n` once, and then simply hit `Enter`, asking GDB to execute the last command for us.

```bash
(gdb) l
9 #include <sys/wait.h>
10	
11	
12 unsigned long sum(int n) {
13    int i;
14    unsigned long sum = 0;
15    for (i = 0; i < n; i++) {
16        sum = sum + i;
17    }
18    return sum;
(gdb) n
15    for (i = 0; i < n; i++) {
(gdb) 
16        sum = sum + i;
(gdb) 
15    for (i = 0; i < n; i++) {
(gdb) 
16        sum = sum + i;
(gdb) 

```

## TUI: Graphical User Interface
The second most helpful feature is the TUI mode, which turns GDB into a modern debugger.

You can switch into TUI by pressing Ctrl-X and then typing "1", or start gdb in TUI mode immediately.
```
$ gdb hello -tui
```

You can also type `tui enable` in the gdb command prompt. Start the program from the beginning and single `step` it with `n` and `s`. The source code of the program will scroll in the TUI window at the top of the screen.

### Examining data
You can print the values of variables with `print` or `p` for short, e.g., print the values of i and sum:
```
(gdb) p i
(gdb) p sum
```

## Conditional breakpoints
While debugging programs, it's often helpful to see what the program is doing right before it crashes. One way to do this is to step through, one at a time, every statement of the program until we reach the point of execution where we want to examine the state of the program. This works, but sometimes you may wish to just run until you get a particular section of code based on a condition, and stop execution at that point so you can examine data at that point of execution.

For instance, in the `sum` function, you might want to examine the state of the program when the index i is equal to 50. You can single `step` until `i` increments and reaches the value 50, but this would be very tedious.

GDB allows you to set conditional breakpoints. To set a conditional breakpoint to break inside the loop of the `sum` function when the index `i` is equal to 50, we do the following: 
first, `list` the source code to get the exact source lines; second, set a breakpoint inside the `main.c` file at line 16 with break main.c:16; third, to make the breakpoint trigger only when i is equal to 50 
(and not trigger for every iteration of the loop) We type `condition 2 i==50`. 

```
$gdb main
...
(gdb) break main
(gdb) l
7 #include <sys/types.h>
8 #include <sys/stat.h>
9 #include <sys/wait.h>
10	
11	
12 unsigned long sum(int n) {
13    int i;
14    unsigned long sum = 0;
15    for (i = 0; i < n; i++) {
16        sum = sum + I;

(gdb) break main.c:16
Breakpoint 2 at 0x4005ee: file main.c, line 16.

(gdb) condition 2 i==50
```

Note that the 2 in the condition refers to the breakpoint number we were notified about when we initially set the breakpoint. We can also achieve the above in one command statement with the following:
```
(gdb) break main.c:16 if i==50
```
We now `run` (if you have not already) and continue execution of the program with the `continue` or `c` command.
```
(gdb) r
(gdb) c
Continuing.
Breakpoint 2, sum (n=100) at main.c:16
16              sum = sum + I;
```
When the breakpoint is hit, we can check if the value of i is really 50:
```
(gdb) p i
$1 = 50
(gdb)
```

## Exploring crashes





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
