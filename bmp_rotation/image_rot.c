#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#include "bmp_trans.h"

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
    /* printf("padding per row %u\n", get_padding_per_row(image.width, image.height, header.bits)); */

    image.data = malloc(sizeof(struct pixel) * image.width * image.height);
    fread(image.data, sizeof(struct pixel), image.width * image.height, file);    

    struct image rot_image;
    rot_image.width = image.height;
    rot_image.height = image.width;
    rot_image.data = malloc(sizeof(struct pixel) * image.width * image.height);
    /* printf("padding per row rotated %u\n", get_padding_per_row(rot_image.width, rot_image.height, header.bits)); */


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
