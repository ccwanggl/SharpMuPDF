
#pragma once

#ifndef __JBIG2CODEC
#define __JBIG2CODEC

#pragma managed(push, off)
struct jbig2ctx;
#pragma managed(pop)

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MuPDF {

/// <summary>
/// <para>This class contains encoding and decoding methods for JBIG2 images.</para>
/// <para>For lossless encoding and decoding, use static methods <see cref="LosslessEncode"> and <see cref="Decode"/>.</para>
/// <para>For high-compression lossy encoding, create an instance, use <see cref="AddPage"> to add all pages, call <see cref="CompletePages"/> after all pages being added, then use a while loop to call <see cref="ProducePage"/> to obtain compressed JBIG2 pages for each added pages.</para>
/// </summary>
public ref class JBig2Codec sealed : IDisposable {
public:
	static array<Byte>^ LosslessEncode(int width, int height, int stride, bool zeroIsWhite, IntPtr src, bool withHeader, bool dupLineRemoval);
	static array<Byte>^ Decode(array<Byte>^ data, array<Byte>^ globals);
	JBig2Codec(float threshold, float weight, int xres, int yres, bool fullHeaders, int refineLevel);
	JBig2Codec() : JBig2Codec(0.92f, 0.5f, 0, 0, false, -1) {}
	~JBig2Codec() {
		ReleaseHandle();
	}
	void AddPage(int width, int height, int stride, bool zeroIsWhite, IntPtr src);
	array<Byte>^ CompletePages();
	array<Byte>^ ProducePage();
	array<Byte>^ ProducePage(int xres, int yres);
	property int Pages { int get() { return _addedPages; } }
	property int ProducedPages { int get() { return _producedPages; } }
private:
	jbig2ctx* _ctx; // context for multipage encode
	int _addedPages, _producedPages;
	static array<Byte>^ ToManagedByteArray(unsigned char* encoded, int length);
	void ReleaseHandle();
};
};

#endif // __COLORSPACE
