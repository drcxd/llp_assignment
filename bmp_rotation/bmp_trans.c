#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdalign.h>

#include "bmp_trans.h"

extern void simd_sepia_partial(float[static 4], const float[static 4], const float[static 4], const float[static 4], const float[static 4], const float[static 4]);

static const float c[3][3] =
{
    { .393f, .769f, .189f },
    { .349f, .686f, .168f },
    { .272f, .543f, .131f }
};

static const float simd_c[3][6] =
{
    { .131f, .168f, .189f, .131f, .168f, .189f },
    { .543f, .686f, .769f, .543f, .686f, .769f },
    { .272f, .349f, .393f, .272f, .349f, .393f },
};
static char PADDING[4];

typedef struct pixel pixel_selector(const struct pixel *, const struct pixel *, const struct pixel *);

static struct pixel get_avg(
    const struct pixel *a,
    const struct pixel *b,
    const struct pixel *c);
static struct pixel get_max(
    const struct pixel *a,
    const struct pixel *b,
    const struct pixel *c);
static struct pixel get_min(
    const struct pixel *a,
    const struct pixel *b,
    const struct pixel *c);
static struct pixel window_trans(uint32_t cent, const struct image *img, pixel_selector* func);
static struct image window_trans_img(const struct image* in, pixel_selector* func);
static void sepia_one(struct pixel *pixel);
static void sepia_simd(struct pixel *pixel);
static unsigned char sat(uint64_t x)
{
    if (x < 256)
    {
        return x;
    }
    return 255;
}

void from_bmp(FILE *in, struct bmp_header *header, struct image *read)
{
    fread(header, sizeof(struct bmp_header), 1, in); /* TODO error check */
    read->width = header->width;
    read->height = header->height;
    read->data = malloc(read->width * read->height * sizeof(struct pixel)); /* TODO error check */
    uint32_t padding = get_padding_per_row(read->width, read->height, header->bits);
    struct pixel *cache = read->data;
    for (uint32_t i = 0; i < read->height; ++i)
    {
        fread(cache, sizeof(struct pixel), read->width, in);
        fread(PADDING, 1, padding, in);
        cache += read->width;
    }
}

void to_bmp(FILE *out, const struct bmp_header *header, const struct image *write)
{
    fwrite(header, sizeof(struct bmp_header), 1, out);
    uint32_t padding = get_padding_per_row(write->width, write->height, header->bits);
    struct pixel *cache = write->data;
    for (uint32_t i = 0; i < write->height; ++i)
    {
        fwrite(cache, sizeof(struct pixel), write->width, out);
        fwrite(PADDING, 1, padding, out);
        cache += write->width;
    }
}

struct image rotate(const struct image* in)
{
    struct image rot;
    rot.width = in->height;
    rot.height = in->width;
    rot.data = malloc(sizeof(struct pixel) * rot.width * rot.height);

    uint32_t pixel_num = in->height* in->width;
    struct pixel *in_pixel = NULL;
    struct pixel *rot_pixel = NULL;
    uint32_t in_row, in_col, rot_row, rot_col, rot_i;
    for (uint32_t i = 0; i < pixel_num; ++i)
    {
        in_pixel = in->data + i;
        in_row = i / in->width;
        in_col = i % in->width;

        rot_row = -in_col + in->width - 1;
        rot_col = in_row;
        rot_i = rot_row * rot.width + rot_col;
        rot_pixel = rot.data + rot_i;
        *rot_pixel = *in_pixel;
    }
    return rot;
}

struct image blur(const struct image* in)
{
    return window_trans_img(in, get_avg);
}

struct image erode(const struct image *in)
{
    return window_trans_img(in, get_min);
}

struct image dilate(const struct image *in)
{
    return window_trans_img(in, get_max);
}

struct image rotate_any_degree(const struct image* in, int32_t degree)
{
    struct image rot;
    double sind = fabs(sin(degree));
    double cosd = fabs(cos(degree));
    rot.width = in->width * sind + in->width * cosd;
    rot.height = in->height * sind + in->width * cosd;
    rot.data = malloc(sizeof(struct pixel) * rot.width * rot.height);

    uint32_t delta = in->width * sind;
    struct pixel *in_pixel = NULL;
    struct pixel *rot_pixel = NULL;
    uint32_t in_row, in_col, rot_row, rot_col, rot_i;
    for (uint32_t i = 0; i < in->height * in->width; ++i)
    {
        in_pixel = in->data + i;
        in_row = i / in->width;
        in_col = i % in->width;

        rot_row = (sind * in_col - cosd * in_row) / (sind * sind - cosd * cosd) + delta;
        rot_col = (sind * in_row - cosd * in_col) / (sind * sind - cosd * cosd);
        rot_i = rot_row * rot.width + rot_col;
        rot_pixel = rot.data + rot_i;
        *rot_pixel = *in_pixel;
    }
    return rot;
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

struct pixel get_avg(const struct pixel *a, const struct pixel *b, const struct pixel *c)
{
    struct pixel avg;
    avg.b = (a->b + b->b + c->b) / 3;
    avg.g = (a->g + b->g + c->g) / 3;
    avg.r = (a->r + b->r + c->r) / 3;
    return avg;
}

struct pixel get_max(const struct pixel *a, const struct pixel *b, const struct pixel *c)
{
    struct pixel max;
    max.b = a->b > b->b ? (a->b > c->b ? a->b : c->b) : (b->b > c->b ? b->b : c->b);
    max.g = a->g > b->g ? (a->g > c->g ? a->g : c->g) : (b->g > c->g ? b->g : c->g);
    max.r = a->r > b->r ? (a->r > c->r ? a->r : c->r) : (b->r > c->r ? b->r : c->r);
    return max;
}

struct pixel get_min(const struct pixel *a, const struct pixel *b, const struct pixel *c)
{
    struct pixel min;
    min.b = a->b < b->b ? (a->b < c->b ? a->b : c->b) : (b->b < c->b ? b->b : c->b);
    min.g = a->g < b->g ? (a->g < c->g ? a->g : c->g) : (b->g < c->g ? b->g : c->g);
    min.r = a->r < b->r ? (a->r < c->r ? a->r : c->r) : (b->r < c->r ? b->r : c->r);
    return min;
}

struct pixel window_trans(uint32_t cent, const struct image *img, pixel_selector *func)
{
    struct pixel high, mid, low;
    high = func(img->data + cent + img->width - 1, img->data + cent + img->width,
        img->data + cent + img->width + 1);
    mid = func(img->data + cent - 1, img->data + cent, img->data + cent + 1);
    low = func(img->data + cent - img->width - 1, img->data + cent - img->width,
        img->data + cent - img->width - 1);
    return func(&high, &mid, &low);
}

struct image window_trans_img(const struct image* in, pixel_selector* func)
{
    struct image out;
    out.width = in->width;
    out.height = in->height;
    out.data = malloc(sizeof(struct pixel) * out.width * out.height);

    uint32_t start_row = 1;
    uint32_t end_row = in->height - 2;
    uint32_t start_col = 1;
    uint32_t end_col = in->width - 2;

    if (start_row > end_row || start_col > end_col)
    {        
        for (uint32_t i = 0; i < out.width * out.height; ++i)
        {
            *(out.data + i) = *(in->data + i); // copy the original image
        }        
    }
    else
    {
        for (uint32_t i = 0; i < in->width * in->height; ++i)
        {
            uint32_t row = i / in->width;
            uint32_t col = i % in->width;
            if (start_row <= row && row <= end_row &&
                start_col <= col && col <= end_col)
            {                
                *(out.data + i) = window_trans(i, in, func);
            }
            else
            {
                *(out.data + i) = *(in->data + i);
            }
        }
    }

    return out;
}

struct image sepia_filt_c(const struct image* in)
{
    struct image sepia;
    sepia.width = in->width;
    sepia.height = in->height;
    sepia.data = malloc(sizeof(struct pixel) * sepia.width * sepia.height);
    for (uint32_t i = 0; i < sepia.width * sepia.height; ++i)
    {
        sepia.data[i] = in->data[i];
        sepia_one(sepia.data + i);
    }
    return sepia;
}

struct image sepia_filt_simd(const struct image* in)
{
    struct image sepia;
    sepia.width = in->width;
    sepia.height = in->height;
    sepia.data = malloc(sizeof(struct pixel) * sepia.width * sepia.height);
    for (uint32_t i = 0; i < sepia.width * sepia.height; ++i)
    {
        sepia.data[i] = in->data[i];
    }
    uint32_t i = 0;
    for (i = 0; i < sepia.width * sepia.height; i += 4)
    {
        sepia_simd(sepia.data + i);
    }
    i = i - 4;
    if (i < sepia.width * sepia.height)
    {
        for (; i < sepia.width * sepia.height; ++i)
        {
            sepia_one(sepia.data + i);
        }
    }
    return sepia;
}

void sepia_one(struct pixel *pixel)
{
    const struct pixel old  = *pixel;
    pixel->r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
    pixel->g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
    pixel->b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
}

void sepia_simd(struct pixel *pixel)
{
    float b[12];
    float g[12];
    float r[12];
    for (uint32_t i = 0; i < 4; ++i)
    {
        *(b + i * 3 ) = *(b + i * 3 + 1) = *(b + i * 3 + 2) = (pixel + i)->b;
        *(g + i * 3 ) = *(g + i * 3 + 1) = *(g + i * 3 + 2) = (pixel + i)->g;
        *(r + i * 3 ) = *(r + i * 3 + 1) = *(r + i * 3 + 2) = (pixel + i)->r;
    }
    simd_sepia_partial(b, g, r, simd_c[0], simd_c[1], simd_c[2]);
    simd_sepia_partial(b + 4, g + 4, r + 4, simd_c[0] + 1, simd_c[1] + 1, simd_c[2] + 1);
    simd_sepia_partial(b + 8, g + 8, r + 8, simd_c[0] + 2, simd_c[1] + 2, simd_c[2] + 2);
    unsigned char res[12];
    for (uint32_t i = 0; i < 12; ++i)
    {
        res[i] = sat(b[i]);
        *((char*)pixel + i) = res[i];
    }    
}
