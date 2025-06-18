#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int a[30]; // the global array
unsigned long faulty_array(int n){
    int i;
    unsigned long sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + a[i];
    }
    return sum;
}

int main(void) {
    printf("Hello world\n");
    long int s;
    s = faulty_array(100000);
    printf("Faulty array sum:%ld\n", s);
    return 0;
}