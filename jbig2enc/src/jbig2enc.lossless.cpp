#include "jbig2enc.h"
#include "jbig2enc.lossless.h"
#include <malloc.h>
#include <allheaders.h>
#if (LIBLEPT_MAJOR_VERSION == 1 && LIBLEPT_MINOR_VERSION >= 83) || LIBLEPT_MAJOR_VERSION > 1
#include "pix_internal.h"
#endif

#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t

uint8_t* jbig2_lossless_encode(int width, int height, int stride, bool zeroIsWhite, uint8_t* const source, bool withHeader, bool dupLineRemoval, int* const length) {
	Pix* pix = CreatePix(width, height, stride, zeroIsWhite, source);
	if (!pix) {
		return NULL;
	}
	u8* ret = jbig2_encode_generic(pix, withHeader, 0, 0, dupLineRemoval, length);
	free(pix);
	return ret;
}

Pix* CreatePix(int width, int height, int stride, bool zeroIsWhite, uint8_t* const source) {
	Pix* pix = (Pix*)malloc(sizeof(Pix));
	if (pix == NULL) {
		return NULL;
	}
	u32 mask = zeroIsWhite ? 0x0 : 0xffffffff;
	u8* pl = source;
	u8* p;
	u32* pw;
	char a, b;
	for (int h = 0; h < height; h++) {
		p = pl;
		for (int w = 0; w < stride; w += 4) {
			pw = (u32*)p;
			*pw = *pw ^ mask;
			a = *p;
			*p = p[3];
			p++;
			b = *p;
			*p = p[1];
			p++;
			*p = b;
			p++;
			*p = a;
			p++;
		}
		pl += stride;
	}

	pix->w = width;
	pix->h = height;
	pix->d = 1;
	pix->wpl = (width + 31) / 32;
	pix->spp = 1;
	pix->refcount = 1;
	pix->data = (u32*)source;
	pix->informat = IFF_UNKNOWN;
	pix->xres = pix->yres = 0;
	pix->special = 0;
	pix->text = NULL;
	pix->colormap = NULL;
	return pix;
}
