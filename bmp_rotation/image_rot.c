#include <stdio.h>
#include <stdint.h>

struct pixel
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct image
{
    uint64_t width, height;
    struct pixel* data;
};

/* enum read_status */
/* { */
/*     READ_OK = 0, */
/*     READ_INVALID_SIGNATURE, */
/*     READ_INVALID_BITS, */
/*     READ_INVALID_HEADER,     */
/* }; */

#pragma pack(push, 2)
/* Struct__attribute__((packed)) bmp_header */ /* optional for gcc */
struct bmp_header
{
    uint16_t type; /* identify bmp file should equal to 'B''M' */
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t data_offset; /* offset of the pixel array */

    uint32_t header_size; /* size of this and following variables should be 40 */
    uint32_t width; /* image width in pixel */
    uint32_t height; /* image height in pixle */
    uint16_t planes; /* number of color planes, must be 1 */
    uint16_t bits; /* number of bits per pixel */
    uint32_t compression;
    uint32_t data_size;
    int32_t hres;
    int32_t vres;
    uint32_t colors;
    uint32_t colors_imp;
};
#pragma pack(pop)

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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s file_name\n", argv[0]);
        return 0;
    }
    FILE *file = fopen(argv[1], "rb");
    if (!file)
    {
        fprintf(stderr, "can not open file %s\n", argv[1]);
    }

    struct bmp_header header;
    fread(&header, sizeof(header), 1, file);
    debug_print_header(&header);
    fclose(file);
    
    return 0;
}
