#include <stdio.h>
#include <malloc.h>

#include "bmp_trans.h"

static char PADDING[4];

static uint32_t get_padding_per_row(uint32_t w, uint32_t h, uint16_t bits);

void from_bmp(FILE *in, struct bmp_header *header, struct image *read)
{    
    fread(header, sizeof(struct bmp_header), 1, in); /* TODO error check */
    read->width = header->width;
    read->height = header->height;
    read->data = malloc(read->width * read->height * sizeof(struct pixel)); /* TODO error check */
    uint32_t padding = get_padding_per_row(read->width, read->height, header->bits);
    uint32_t row_bytes = sizeof(struct pixel) * read->width;
    struct pixel *cache = read->data;
    for (uint32_t i = 0; i < read->height; ++i)
    {
        fread(cache, sizeof(struct pixel), read->width, in);
        fread(PADDING, 1, padding, in);
        cache += row_bytes;
    }
}

void to_bmp(FILE *out, struct bmp_header *header, struct image *write)
{
    header->width = write->width;
    header->height = write->height;
    fwrite(header, sizeof(struct bmp_header), 1, out);
    uint32_t padding = get_padding_per_row(write->width, write->height, header->bits);
    uint32_t row_bytes = sizeof(struct pixel) * write->width;
    struct pixel *cache = write->data;
    for (uint32_t i = 0; i < write->height; ++i)
    {
        fwrite(cache, sizeof(struct pixel), write->width, out);
        fwrite(PADDING, 1, padding, out);
        cache += row_bytes;
    }
}

void debug_print_header(struct bmp_header *header)
{
    char *p = ((char*)header);
    printf("file id %c%c\n", *p, *(p + 1));
    printf("file size %u\n", header->file_size);
    printf("header_size %u\n", header->header_size);
    printf("width %u\n", header->width);
    printf("height %u\n", header->height);
    printf("bits per pixel %hu\n", header->bits);
    printf("compression method %u\n", header->compression);
    printf("data size read %u\n", header->data_size);
    printf("data size calculated %u\n", (header->width * header->bits + 31) / 32 * 4 * header->height);
}

uint32_t get_padding_per_row(uint32_t w, uint32_t h, uint16_t bits)
{
    return (w * bits + 31) / 32 * 4 - w * bits / 8;
}
