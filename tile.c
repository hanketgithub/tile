#include <stdint.h>
#include <string.h>

void tile
(
    const uint8_t src[],
    int width,
    int height,
    uint8_t *dst_tl,
    uint8_t *dst_tr,
    uint8_t *dst_bl,
    uint8_t *dst_br
)
{
    uint8_t *p;
    uint8_t *q;
    uint8_t *r;
    int i;
	
	p = (uint8_t *) src;
	q = dst_tl;
	r = dst_tr;
    
    // Top half
    for (i = 0; i < height / 2; i++)
    {
        memcpy(q, p, width / 2);
        p += width / 2;
        q += width / 2;
        memcpy(r, p, width / 2);
        p += width / 2;
        r += width / 2;
    }
    
    q = dst_bl;
    r = dst_br;
    // Bottom half
    for (i = 0; i < height / 2; i++)
    {
        memcpy(q, p, width / 2);
        p += width / 2;
        q += width / 2;
        memcpy(r, p, width / 2);
        p += width / 2;
        r += width / 2;
    }
}
