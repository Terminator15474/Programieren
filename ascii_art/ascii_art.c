#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE (16*1024*1025)

void checkHeaders(char* buffer) {
    if((unsigned char) buffer[0] != 0x89) {
        printf("error header byte 1");
        exit(1);
    }

    if( (unsigned char) buffer[1] == 'P' ) {
        printf("error header byte 2");
        exit(1);
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage %s <png file>", argv[0]);
    }

    FILE* input = fopen(argv[1], "r");
    if(!input) {
        printf("error: file not readable");
        exit(1);
    }
    char* buf = (char *) calloc(MAX_SIZE, 1);
    int size = fread(buf, 1, MAX_SIZE, input);

    if( size < 0) {
        printf("error: empty file");
        exit(1);
    }

    checkHeaders(buf);
    fclose(input);
    free(buf);
    return 0;
}
