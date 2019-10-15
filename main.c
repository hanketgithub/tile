//
//  main.c
//  tile
//
//  Created by Hank Lee on 8/22/18.
//  Copyright (c) 2015 Hank Lee. All rights reserved.
//
// Split a picture into four
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#include "tile.h"

#define MAX_WIDTH   7680
#define MAX_HEIGHT  4320


static uint8_t img[MAX_WIDTH * MAX_HEIGHT * 3];
static uint8_t tl[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3]; // top-left
static uint8_t tr[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3]; // top-right
static uint8_t bl[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3]; // bottom-left
static uint8_t br[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3]; // bottom-right


int main(int argc, const char * argv[])
{
    int fd_src;
    int fd_tl;
    int fd_tr;
    int fd_bl;
    int fd_br;
    
    ssize_t rd_sz;
    
    uint32_t width;
    uint32_t height;
    uint32_t wxh;
    uint32_t pix_sz;
    uint32_t c_pixel_per_sample;

    char *cp;
    char output[256] = { 0 };

    if (argc < 6)
    {
        fprintf(stderr, "useage: %s [src_file] [width] [height] [bit_depth] [420 | 422]\n", argv[0]);
        
        return -1;
    }
    
    rd_sz       = 0;
    width       = 0;
    height      = 0;
    wxh         = 0;
    cp          = NULL;
    pix_sz      = 0;
    c_pixel_per_sample = 4;
    
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0)
    {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    
    // specify output file name (0,0)
    cp = strchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_tl.yuv");
    
    fd_tl = open
            (
             output,
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR
            );
    // specify output file name (0,1)
    memset(output, 0, sizeof(output));
    cp = strchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_tr.yuv");
    
    fd_tr = open
            (
             output,
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR
            );
    // specify output file name (1,0)
    memset(output, 0, sizeof(output));
    cp = strchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_bl.yuv");
    
    fd_bl = open
            (
             output,
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR
            );
    // specify output file name (1,1)
    memset(output, 0, sizeof(output));
    cp = strchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_br.yuv");
    
    fd_br = open
            (
             output,
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR
            );
            
    width   = atoi(argv[2]);
    height  = atoi(argv[3]);
    pix_sz  = (atoi(argv[4]) == 10) ? 2 : 1;
    c_pixel_per_sample = (strcmp(argv[5], "422") == 0) ? 2 : 4;
    
    wxh = width * height;

    
    fprintf(stderr, "Processing: ");

    while (1)
    {
        // Y
        rd_sz = read(fd_src, img, wxh * pix_sz);
        if (rd_sz == wxh * pix_sz)
        {
            tile(img, width, height, tl, tr, bl, br, pix_sz);

            write(fd_tl, tl, wxh / 4 * pix_sz);
            write(fd_tr, tr, wxh / 4 * pix_sz);
            write(fd_bl, bl, wxh / 4 * pix_sz);
            write(fd_br, br, wxh / 4 * pix_sz);
        }
        else
        {
            break;
        }

        uint32_t c_height = (c_pixel_per_sample == 2) ? height : height / 2;

        // U
        rd_sz = read(fd_src, img, wxh / c_pixel_per_sample * pix_sz);
        if (rd_sz == wxh / c_pixel_per_sample * pix_sz)
        {
            tile(img, width / 2, c_height, tl, tr, bl, br, pix_sz);

            write(fd_tl, tl, wxh / c_pixel_per_sample / 4 * pix_sz);
            write(fd_tr, tr, wxh / c_pixel_per_sample / 4 * pix_sz);
            write(fd_bl, bl, wxh / c_pixel_per_sample / 4 * pix_sz);
            write(fd_br, br, wxh / c_pixel_per_sample / 4 * pix_sz);
        }
        else
        {
            break;
        }
        
        // V
        rd_sz = read(fd_src, img, wxh / c_pixel_per_sample * pix_sz);
        if (rd_sz == wxh / c_pixel_per_sample * pix_sz)
        {    
            tile(img, width / 2, c_height, tl, tr, bl, br, pix_sz);

            write(fd_tl, tl, wxh / c_pixel_per_sample / 4 * pix_sz);
            write(fd_tr, tr, wxh / c_pixel_per_sample / 4 * pix_sz);
            write(fd_bl, bl, wxh / c_pixel_per_sample / 4 * pix_sz);
            write(fd_br, br, wxh / c_pixel_per_sample / 4 * pix_sz);
        }
        else
        {
            break;
        }
        fputc('.', stderr);
        fflush(stderr);
    }
        
    close(fd_src);
    close(fd_tl);
    close(fd_tr);
    close(fd_bl);
    close(fd_br);

    fprintf(stderr, "Done\n");
            
    return 0;
}
