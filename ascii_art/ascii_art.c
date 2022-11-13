#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zlib/zlib.h"

#define MAX_SIZE ( 16 * 1024 * 1024 )

struct header {
    int width;
    int height;
    char bit_depth;
    char color_type;
    char compression_method;
    char filter_method;
    char interlace_method;
} header;

struct plte_palette {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} plte_palette;

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

char* inflateData(char* input_data, int size) {
    char* output = malloc(size);
    z_stream stream;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    if(inflateInit(&stream) != Z_OK) {
        stream.avail_in = size;
        stream.next_in = input_data;
        stream.avail_out = size;
        stream.next_out = output;
        inflate(&stream, Z_NO_FLUSH);
        (void)inflateEnd(&stream);
    }
    return output;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage %s <png file>\n", argv[0]);
        exit(1);
    }

    FILE* input = fopen(argv[1], "rb");
    if(!input) {
        printf("error: file not readable");
        exit(1);
    }
    char* buf = (char *) malloc(MAX_SIZE);

    int size = fread(buf, sizeof(char), MAX_SIZE, input);

    if( size < 0) {
        printf("error: empty file");
        exit(1);
    }

    checkHeadersPNG(buf);

    struct header png_header;

    char data[size];

    int pos = 8;
    while(pos < size) {
        char lenbuf[4];
        memcpy(lenbuf, buf + pos, 4);
        pos +=4;
        int len = get_big_endian(lenbuf);
        char chunktype[4];
        memcpy(chunktype, buf + pos, 4);
        char chunkbuf[len];
        pos += 4;
        memcpy(chunkbuf, buf + pos, len);
        pos+=len;
        char crcbuf[4];
        memcpy(crcbuf, buf+pos, 4);
        pos+=4;

        if( strcmp("IHDR", chunktype) == 0) {
            png_header.width = get_big_endian(chunkbuf);
            png_header.height = get_big_endian(chunkbuf+4);
            png_header.bit_depth = chunkbuf[8];
            png_header.color_type = chunkbuf[9];
            png_header.compression_method = chunkbuf[10];
            png_header.filter_method = chunkbuf[11];
            png_header.interlace_method = chunkbuf[12];
        }

        if( strcmp("PLTE", chunktype) == 0) {
            int i;
            struct plte_palette paletts[len/3];
            for (i = 0; i < len/3; i++) {
                paletts[i].red = chunkbuf[i*3];
                paletts[i].green = chunkbuf[i * 3 + 1];
                paletts[i].blue = chunkbuf[i * 3 + 2];

                printf("red: %i, green: %i, blue %i\n", paletts[i].red, paletts[i].blue, paletts[i].blue);
            }
            printf("i: %i", i);
        }

        if(strcmp("IDAT", chunktype) == 0) {
            char compression_method = chunkbuf[0];
            char flags = chunkbuf[1];
            char compressed_data[len-6];
            strcpy(compressed_data, chunkbuf+2);
            int check_value = get_big_endian(chunkbuf+len-2);
            char* true_data = inflateData(compressed_data, len-6);

            printf("compression_method: %d, flags: %d, compressed_data: %s, check_value: %i\n", compression_method, flags, compressed_data, check_value);
        }
    }
    printf("width: %i, height: %i, bit_depth: %i, color_type: %i, compression_method: %i, filter_method: %i, interlace_method: %i", png_header.width, png_header.height, png_header.bit_depth, png_header.color_type, png_header.compression_method, png_header.filter_method, png_header.interlace_method);
    fclose(input);
    free(buf);
    return 0;
}
