#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#pragma pack(push, 1)
struct pixel
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};
#pragma pack(pop)

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

static uint32_t get_padding_per_row(uint32_t w, uint32_t h, uint16_t bits)
{
    return (w * bits + 31) / 32 * 4 - w * bits / 8;
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
        return 0;
    }

    struct bmp_header header;
    fread(&header, sizeof(header), 1, file);
    debug_print_header(&header);

    struct image image;
    image.width = header.width;
    image.height = header.height;
    printf("padding per row %u\n", get_padding_per_row(image.width, image.height, header.bits));

    image.data = malloc(sizeof(struct pixel) * image.width * image.height);
    fread(image.data, sizeof(struct pixel), image.width * image.height, file);
    
    FILE* temp = fopen("/mnt/d/output.bmp", "wb");
    fwrite(&header, sizeof(header), 1, temp);
    fwrite(image.data, sizeof(struct pixel), image.width * image.height, temp);
    fclose(temp);    

    struct image rot_image;
    rot_image.width = image.height;
    rot_image.height = image.width;
    rot_image.data = malloc(sizeof(struct pixel) * image.width * image.height);
    printf("padding per row rotated %u\n", get_padding_per_row(rot_image.width, rot_image.height, header.bits));


    struct bmp_header rot_header = header;
    rot_header.width = header.height;
    rot_header.height = header.width;

    for (uint32_t i = 0; i < image.width * image.height; ++i)
    {
        struct pixel *pixel = image.data + i;
        uint32_t row = i / image.width;
        uint32_t col = i % image.width;

        uint32_t rot_row = -col + image.width - 1;
        uint32_t rot_col = row;
        uint32_t rot_i = rot_row * rot_image.width + rot_col;

        struct pixel *rot_pixel = rot_image.data + rot_i;
        *rot_pixel = *pixel;
        /* printf("%hhu %hhu %hhu\n", pixel->r, pixel->g, pixel->b); */
    }

    FILE *out = fopen("/mnt/d/rot.bmp", "wb");
    if (!out)
    {
        fprintf(stderr, "can not open file /mnt/d/rot.bmp\n");
        free(rot_image.data);
        free(image.data);
        fclose(file);
        return 0;
    }
    fwrite(&rot_header, sizeof(rot_header), 1, out);
    fwrite(rot_image.data, sizeof(struct pixel), rot_image.width * rot_image.height, out);
    fclose(out);
    fclose(file);
    

    free(rot_image.data);
    free(image.data);

    return 0;
}
