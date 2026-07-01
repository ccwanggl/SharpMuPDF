#ifndef __FONT_DESC
#define __FONT_DESC
#pragma once

#include "MuPDF.h"

using namespace System;
using namespace System::Text;

namespace MuPDF {

ref class PdfDictionary;
ref class TextFont;

[Flags]
public enum class FontDescriptorFlags {
	None,
	FixedPitch = 1 << 0,
	Serif = 1 << 1,
	Symbolic = 1 << 2,
	Script = 1 << 3,
	Nonsymbolic = 1 << 5,
	Italic = 1 << 6,
	AllCap = 1 << 16,
	SmallCap = 1 << 17,
	ForceBold = 1 << 18
};

public ref class FontDescriptor sealed : IDisposable {

public:
	FontDescriptor() : FontDescriptor(pdf_new_font_desc(Context::Ptr)) {
	}
	PropEmptyGet(TextFont^, Font);
	PropGet(FontDescriptorFlags, Flags, (FontDescriptorFlags)_font->flags);
	PropGet(float, ItalicAngle, _font->italic_angle);
	PropGet(float, Ascent, _font->ascent);
	PropGet(float, Descent, _font->descent);
	PropGet(float, CapHeight, _font->cap_height);
	PropGet(float, XHeight, _font->x_height);
	PropGet(float, MissingWidth, _font->missing_width);
	PropGet(bool, IsEmbedded, _font->is_embedded);
	PropGet(int, CidToGidLength, _font->cid_to_gid_len);
	PropGet(int, CidToUcsLength, _font->cid_to_ucs_len);

	int CidToGid(int cid) {
		return pdf_font_cid_to_gid(Context::Ptr, _font, cid);
	}
	void DecodeText(array<unsigned char>^ bytes, int offset, int length, StringBuilder^ sb);
	static FontDescriptor^ Load(Document^ doc, PdfDictionary^ resources, PdfDictionary^ font);
	static FontDescriptor^ Load(Document^ doc, ResourceStack^ resources, PdfDictionary^ font);

internal:
	FontDescriptor(pdf_font_desc* font) : _font(font) {
		pdf_keep_font(Context::Ptr, font);
	}
	~FontDescriptor();

private:
	pdf_font_desc* _font;
	TextFont^ _textFont;

	void DecodeRawBytes(unsigned char* data, unsigned char* end, StringBuilder^ sb);

	!FontDescriptor() {
		DropHandle(_font, pdf_drop_font);
	}
};

}

#endif