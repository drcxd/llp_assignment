#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

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
        
    struct image trans_img = rotate_any_degree(&org_img, -45);

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
