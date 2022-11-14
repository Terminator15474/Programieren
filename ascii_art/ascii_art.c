#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zlib/zlib.h"
#include <assert.h>


#define MAX_SIZE ( 16 * 1024 * 1024 )
#define CHUNK 16000
#define FACTOR 7

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

int inflateData(char* input_data, char* outputbuf, int insize, int outsize) {
    z_stream stream;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = insize;
    stream.next_in = Z_NULL;
    int ret = inflateInit(&stream);
    if(ret == Z_OK) {
        printf("inflateInit successful");
        stream.avail_in = insize;
        stream.next_in = input_data;
        stream.avail_out = outsize;
        stream.next_out = outputbuf;
        ret = inflate(&stream, Z_NO_FLUSH);
        printf("inflated %i bytes , output %d bytes\n", stream.total_in, stream.total_out);
        switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;
                printf("error when inflating, error %i\n", ret);
            case Z_DATA_ERROR:
                printf("error when inflating, error Z_DATA_ERROR\n");
            case Z_MEM_ERROR:
                (void)inflateEnd(&stream);
                printf("error when inflating, error %i\n", ret);
            case Z_BUF_ERROR:
                printf("error when inflating, BUF_ERROR\n");
                return ret;
        }
        (void)inflateEnd(&stream);
    } else {
        printf("inflateInit failed, error: %i\n", ret);
        return ret;
    }
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
            printf("i: %i\n\n", i);
        }

        if(strcmp("IDAT", chunktype) == 0) {
            char* true_data = malloc(len*FACTOR);
            int return_val = inflateData(chunkbuf, true_data, len, len * FACTOR);
            printf("len: %i ret: %i ,data: %d\n",len ,return_val, strlen(true_data));
        }
    }
    printf("width: %i, height: %i, bit_depth: %i, color_type: %i, compression_method: %i, filter_method: %i, interlace_method: %i", png_header.width, png_header.height, png_header.bit_depth, png_header.color_type, png_header.compression_method, png_header.filter_method, png_header.interlace_method);
    fclose(input);
    free(buf);
    return 0;
}
