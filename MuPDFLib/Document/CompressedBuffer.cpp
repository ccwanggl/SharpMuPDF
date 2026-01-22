#include "CompressedBuffer.h"

using namespace MuPDF;

CompressedBuffer::CompressedBuffer(fz_compressed_buffer* buffer) {
	if (buffer) {
		_buffer = buffer;
		fz_keep_compressed_buffer(Context::Ptr, buffer);
	}
}

void CompressedBuffer::ReleaseHandle() {
	if (_buffer) {
		fz_drop_compressed_buffer(Context::Ptr, _buffer);
		_buffer = NULL;
	}
}