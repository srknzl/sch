
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {

    FILE * code1File = fopen("Provided Files/code1.txt","r");
    printf("%p",code1File);
    fclose(code1File);
}