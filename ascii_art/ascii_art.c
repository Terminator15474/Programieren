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

struct rgba {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} rgba;

struct plte_palette {
    struct rgba* pixels;
    char init;
} plte_palette;

struct png_image {
    struct header header;
    struct rgba* pixel;
    struct plte_palette palette;
} png_image;

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

int filterData(unsigned char* inputbuf, struct png_image* image) {
    int ypos = 0;
    int xpos = 0;
    char filter_type = 0;
    for (ypos = 0; ypos < image->header.height; ypos++) {
        filter_type = inputbuf[ypos];
        printf("filter type: %d\n", filter_type);
        switch (filter_type) {
            case 0:
                inputbuf+=1;
                for ( xpos = 1; xpos <= image->header.width; xpos++) {
                    char index = inputbuf[xpos];
                    printf("index: %d\n", index);
                    image->pixel = &image->palette.pixels[index];
                }
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
        }
    }
}
    
int inflateData( unsigned char* input_data, unsigned char* outputbuf, int insize, int outsize) {
    z_stream stream;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    printf("TEST");
    int ret = inflateInit(&stream);
    printf("test");
    if(ret == Z_OK) {
        printf("inflateInit successful\n");
        stream.avail_in = insize;
        stream.next_in = input_data;
        stream.avail_out = outsize;
        stream.next_out = outputbuf;
        do {
            ret = inflate(&stream, Z_FINISH);
        } while(stream.avail_in > 0);
        printf("message: %s, avail_out: %i\n", stream.msg, stream.avail_out);
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
            case Z_STREAM_END:
                printf("inflated %i bytes , output %d bytes\n", stream.total_in, stream.total_out);
        }
        (void)inflateEnd(&stream);
        return ret;
    } else {
        printf("inflateInit failed, error: %i\n", ret);
        (void)inflateEnd(&stream);
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
    struct plte_palette palette;
    struct header png_header;
    int pos = 8;
    while(pos < size) {
        unsigned char lenbuf[4];
        memcpy(lenbuf, buf + pos, 4);
        pos +=4;
        int len = get_big_endian(lenbuf);
        unsigned char chunktype[4];
        memcpy(chunktype, buf + pos, 4);
        unsigned char chunkbuf[len];
        pos += 4;
        memcpy(chunkbuf, buf + pos, len);
        pos+=len;
        unsigned char crcbuf[4];
        memcpy(crcbuf, buf+pos, 4);
        pos+=4;

        if( strcmp("IHDR",(char *) chunktype) == 0) {
            png_header.width = get_big_endian(chunkbuf);
            png_header.height = get_big_endian(chunkbuf+4);
            png_header.bit_depth = chunkbuf[8];
            png_header.color_type = chunkbuf[9];
            png_header.compression_method = chunkbuf[10];
            png_header.filter_method = chunkbuf[11];
            png_header.interlace_method = chunkbuf[12];
        }
        if( strcmp("PLTE", (char*) chunktype) == 0) {
            int i;
            palette.init = 1;
            for (i = 0; i < len/3; i++) {
                palette.pixels[i].r = chunkbuf[i*3];
                palette.pixels[i].g = chunkbuf[i * 3 + 1];
                palette.pixels[i].b = chunkbuf[i * 3 + 2];
                palette.pixels[i].a = chunkbuf[i * 3 + 3];
            }
        }

        if(strcmp("IDAT",(char*) chunktype) == 0) {
            unsigned char* true_data = malloc(len*FACTOR);
            int full_lenth = len*FACTOR;

            int return_val = inflateData(chunkbuf, true_data, len, len * FACTOR);
            printf("return value: %i", return_val);
            if( return_val != 1) { printf("ERROR when decompressing file"); exit(1); }

            struct png_image image;
            image.header = png_header;
            image.palette = palette;
            switch (png_header.color_type) {
                case 3:
                    if(palette.init ==  0) { printf("DATA ERROR no PLTE"); exit(1); }
                    filterData(true_data, &image);
            }
        }

    }
    printf("width: %i, height: %i, bit_depth: %i, color_type: %i, compression_method: %i, filter_method: %i, interlace_method: %i", png_header.width, png_header.height, png_header.bit_depth, png_header.color_type, png_header.compression_method, png_header.filter_method, png_header.interlace_method);
    fclose(input);
    free(buf);
    return 0;
}
