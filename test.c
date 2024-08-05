#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    printf("^\n");
    for(int i = 0; i < 5; i++)
        printf("%c\n", 186);
    for(int i = 0; i < 2; i++)
        printf("%c\n", 219);
    for(int i = 0; i < 10; i++)
        printf("%c\n", 186);
    printf("v\n");
    return 0;
}
