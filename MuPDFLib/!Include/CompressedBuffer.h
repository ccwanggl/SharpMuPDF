#include "../Context.h"
#include "Stream.h"
#include <mupdf/fitz/compressed-buffer.h>

#ifndef __COMPRESSEDBUFFER
#define __COMPRESSEDBUFFER

#pragma once
using namespace System;

namespace MuPDF {

	public ref class CompressedBuffer sealed : IDisposable {
	public:
		CompressedBuffer() : CompressedBuffer(fz_new_compressed_buffer(Context::Ptr)) {};

		property bool IsDisposed { bool get() { return _buffer == NULL; } }

		/// <summary>
		/// Returns storage size used for this compressed buffer (in bytes).
		/// </summary>
		property long Size { long get() { return _buffer ? (long)fz_compressed_buffer_size(_buffer) : 0; } }

		/// <summary>Open a stream to read decompressed data from this compressed buffer.</summary>
		Stream^ Open() { return gcnew Stream(fz_open_compressed_buffer(Context::Ptr, _buffer)); }

	internal:
		CompressedBuffer(fz_compressed_buffer* buffer);

		~CompressedBuffer() {
			ReleaseHandle();
		}

	private:
		fz_compressed_buffer* _buffer;

		void ReleaseHandle();
	};

}

#endif // !__COMPRESSEDBUFFER