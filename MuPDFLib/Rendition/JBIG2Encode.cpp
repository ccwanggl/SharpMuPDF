#include "JBIG2Codec.h"
#include "jbig2enc.lossless.h"
#include "jbig2enc.h"
#include <malloc.h>

using namespace MuPDF;
using namespace System::Runtime::InteropServices;

array<Byte>^ JBig2Codec::LosslessEncode(int width, int height, int stride, bool zeroIsWhite, IntPtr src, bool withHeader, bool dupLineRemoval) {
	int length = 0;
	auto encoded = jbig2_lossless_encode(width, height, stride, zeroIsWhite, (uint8_t*)src.ToPointer(), withHeader, dupLineRemoval, &length);
	return ToManagedByteArray(encoded, length);
}

array<unsigned char>^ MuPDF::JBig2Codec::ToManagedByteArray(unsigned char* encoded, int length) {
	if (encoded) {
		array<Byte>^ managedArray = gcnew array<Byte>(length);
		Marshal::Copy(IntPtr(encoded), managedArray, 0, length);
		free(encoded);
		return managedArray;
	}
	return nullptr;
}

JBig2Codec::JBig2Codec(float threshold, float weight, int xres, int yres, bool fullHeaders, int refineLevel) {
	_ctx = jbig2_init(threshold, weight, xres, yres, fullHeaders, refineLevel);
}
void JBig2Codec::AddPage(int width, int height, int stride, bool zeroIsWhite, IntPtr src) {
	Pix* pix = CreatePix(width, height, stride, zeroIsWhite, (uint8_t*)src.ToPointer());
	if (pix) {
		jbig2_add_page(_ctx, pix);
		_addedPages++;
		// do not call pixDestroy, which destroys the data from src
		free(pix);
	}
}

array<Byte>^ MuPDF::JBig2Codec::CompletePages() {
	int length = 0;
	auto encoded = jbig2_pages_complete(_ctx, &length, false);
	return ToManagedByteArray(encoded, length);
}

array<Byte>^ JBig2Codec::ProducePage() {
	return ProducePage(-1, -1);
}
array<Byte>^ JBig2Codec::ProducePage(int xres, int yres) {
	if (_producedPages >= _addedPages) {
		return nullptr;
	}
	int length = 0;
	auto encoded = jbig2_produce_page(_ctx, _producedPages, xres, yres, &length);
	return ToManagedByteArray(encoded, length);
}

void JBig2Codec::ReleaseHandle() {
	jbig2_destroy(_ctx);
	_ctx = NULL;
}