#ifndef JBIG2ENC_JBIG2_LOSSLESS_H__
#define JBIG2ENC_JBIG2_LOSSLESS_H__

#include <stdlib.h>
#include <stdint.h>

struct Pix;

uint8_t* jbig2_lossless_encode(int width, int height, int stride, bool zeroIsWhite, uint8_t* const source, bool withHeader, bool dupLineRemoval, int* const length);

Pix* CreatePix(int width, int height, int stride, bool zeroIsWhite, uint8_t* const source);


#endif