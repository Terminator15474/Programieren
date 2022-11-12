#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE ( 16 * 1024 * 1024 )

//Check headers for png
void checkHeadersPNG(char* buffer) {
    if((unsigned char) buffer[0] != 0x89) {
        printf("error header byte 1\n");
        exit(1);
    }

    if(! (unsigned char) buffer[1] == 'P' ) {
        printf("error header byte 2\n");
        exit(1);
    }
}

int get_big_endian(const char *buf) {
    return ((unsigned char)buf[0] << 24) |
           ((unsigned char)buf[1] << 16) |
           ((unsigned char)buf[2] << 8)  |
            (unsigned char)buf[3];
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage %s <png file>\n", argv[0]);
        exit(1);
    }

    FILE* input = fopen(argv[1], "r");
    if(!input) {
        printf("error: file not readable");
        exit(1);
    }
    char* buf = (char *) malloc(MAX_SIZE);
    int size = fread(buf, 1, MAX_SIZE, input);

    if( size < 0) {
        printf("error: empty file");
        exit(1);
    }

    checkHeadersPNG(buf);

    int pos = 8;
    printf("size: %i\n", size);
    while(1) {
        char lenbuf[4];
        memcpy(lenbuf, buf + pos, 4);
        pos +=4;
        int len = get_big_endian(lenbuf);
        char chunktype[4];
        memcpy(chunktype, buf + pos, 4);
        char chunkbuf[len];
        memcpy(chunkbuf, buf + pos, len);
        pos+=len;

        printf("chunk: %s -len: %d (%d)\n", chunkbuf, len, size - (pos + len + 12));
    }

    fclose(input);
    free(buf);
    return 0;
}
