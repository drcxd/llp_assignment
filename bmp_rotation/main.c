#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "bmp_trans.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s in_file out_file\n", argv[0]);
        return 0;
    }
    FILE *in = fopen(argv[1], "rb");
    if (!in)
    {
        fprintf(stderr, "can not open file %s\n", argv[1]);
        return 0;
    }

    struct bmp_header org_header;
    struct image org_img;
    from_bmp(in, &org_header, &org_img);

    debug_print_header(&org_header);

    struct rusage r;
    struct timeval start;
    struct timeval end;
    
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;
    /* struct image trans_img = sepia_filt_simd(&org_img); */
    struct image trans_img = sepia_filt_c(&org_img);
    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;

    long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
    printf("Time elapsed in microseconds: %ld\n", res);

    struct bmp_header trans_header = org_header;
    trans_header.width = trans_img.width;
    trans_header.height = trans_img.height;
    trans_header.data_size = (trans_img.width * 3 + get_padding_per_row(trans_img.width, trans_img.height, 24)) * trans_img.height;

    FILE *out = fopen(argv[2], "wb");
    if (!out)
    {
        fprintf(stderr, "can not open file %s\n", argv[2]);
        return 0;
    }

    to_bmp(out, &trans_header, &trans_img);

    fclose(out);
    fclose(in);
    return 0;
}
