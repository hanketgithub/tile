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


static uint8_t img[MAX_WIDTH * MAX_HEIGHT * 3 / 2];
static uint8_t tl[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3 / 2]; // top-left
static uint8_t tr[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3 / 2]; // top-right
static uint8_t bl[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3 / 2]; // bottom-left
static uint8_t br[MAX_WIDTH / 2 * MAX_HEIGHT / 2 * 3 / 2]; // bottom-right


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

    char *cp;
    char output[256] = { 0 };

    if (argc < 4)
    {
        fprintf(stderr, "useage: %s [src_file] [width] [height]\n", argv[0]);
        
        return -1;
    }
    
    rd_sz       = 0;
    width       = 0;
    height      = 0;
    wxh         = 0;
    cp          = NULL;
    
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
    
    wxh = width * height;

    
    fprintf(stderr, "Processing: ");

    while (1)
    {
        // Y
        rd_sz = read(fd_src, img, wxh);
        if (rd_sz == wxh)
        {
            tile(img, width, height, tl, tr, bl, br);

            write(fd_tl, tl, wxh / 4);
            write(fd_tr, tr, wxh / 4);
            write(fd_bl, bl, wxh / 4);
            write(fd_br, br, wxh / 4);
        }
        else
        {
            break;
        }
        
        // U
        rd_sz = read(fd_src, img, wxh / 4);
        if (rd_sz == wxh / 4)
        {
            tile(img, width / 2, height / 2, tl, tr, bl, br);

            write(fd_tl, tl, wxh / 4 / 4);
            write(fd_tr, tr, wxh / 4 / 4);
            write(fd_bl, bl, wxh / 4 / 4);
            write(fd_br, br, wxh / 4 / 4);
        }
        else
        {
            break;
        }
        
        // V
        rd_sz = read(fd_src, img, wxh / 4);
        if (rd_sz == wxh / 4)
        {    
            tile(img, width / 2, height / 2, tl, tr, bl, br);

            write(fd_tl, tl, wxh / 4 / 4);
            write(fd_tr, tr, wxh / 4 / 4);
            write(fd_bl, bl, wxh / 4 / 4);
            write(fd_br, br, wxh / 4 / 4);
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
