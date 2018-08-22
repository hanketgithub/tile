//
//  tile.h
//  tile
//
//  Created by Hank Lee on 8/22/18.
//  Copyright (c) 2015 Hank Lee. All rights reserved.
//

#ifndef ___tile___
#define ___tile___

void tile
(
  const uint8_t  src[],
        int      width,
        int      height,
        uint8_t *dst_tl,
        uint8_t *dst_tr,
        uint8_t *dst_bl,
        uint8_t *dst_br
);

#endif
