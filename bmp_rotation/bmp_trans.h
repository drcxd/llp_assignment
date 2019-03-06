#ifndef _BMP_TRANS_H_
#define _BMP_TRANS_H_

#include <stdint.h>

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

void from_bmp(FILE *in, struct bmp_header *header, struct image *read);
void to_bmp(FILE *out, struct bmp_header *header, struct image *write);

void debug_print_header(struct bmp_header *header);

#endif /* _BMP_TRANS_H_ */
