#ifndef __PIXMAP
#define __PIXMAP

#pragma once
#include "MuPDF.h"
#include "ObjWrapper.h"

using namespace System;

namespace MuPDF {

ref class Colorspace;
enum class ColorspaceKind;

public enum class RenderingIntent : uint8_t {
	Perceptual,
	RelativeColorimetric,
	Saturation,
	AbsoluteColorimetric,
};

public ref class ColorParams sealed
{
public:
	RenderingIntent RenderingIntent;
	Byte BlackPointCompensation;
	Byte Overprinting;
	Byte OverprintingMode;

internal:
	fz_color_params ToNative() {
		return { static_cast<uint8_t>(RenderingIntent), static_cast<uint8_t>(BlackPointCompensation), static_cast<uint8_t>(Overprinting), static_cast<uint8_t>(OverprintingMode) };
	}
};

public ref class Pixmap sealed : IDisposable {
public:
	static int LoadTiffSubImageCount(IntPtr data, int length);
	static Pixmap^ LoadTiffSubImage(IntPtr data, int length, int index);

	PropGet(IntPtr, Samples, _samples);
	PropGet(int, Stride, _stride);
	PropGet(int, Width, _width);
	PropGet(int, Height, _height);
	PropGet(int, X, fz_pixmap_x(Context::Ptr, _pixmap));
	PropGet(int, Y, fz_pixmap_y(Context::Ptr, _pixmap));
	PropGet(int, Components, _components);
	PropGet(int, Colorants, fz_pixmap_colorants(Context::Ptr, _pixmap));
	PropGet(int, Spots, fz_pixmap_spots(Context::Ptr, _pixmap));
	PropGet(bool, IsMonochrome, fz_is_pixmap_monochrome(Context::Ptr, _pixmap));
	PropGet(MuPDF::BBox, BBox, fz_pixmap_bbox(Context::Ptr, _pixmap));
	property MuPDF::Colorspace^ Colorspace { MuPDF::Colorspace^ get(); };
	/// <summary>
	/// Return the number of alpha planes in a Pixmap. Does not throw exceptions.
	/// </summary>
	property int Alpha { int get() { return fz_pixmap_alpha(Context::Ptr, _pixmap); } }

	static Pixmap^ Create(ColorspaceKind colorspace, int width, int height);

	static Pixmap^ Create(ColorspaceKind colorspace, MuPDF::BBox box);

	void SetBackgroundWhite() {
		Clear(0xFF);
	}
	void Clear(int value) {
		fz_clear_pixmap_with_value(Context::Ptr, _pixmap, value);
	}
	void Invert() {
		fz_invert_pixmap(Context::Ptr, _pixmap);
	}
	void InvertLuminance();
	void Tint(int black, int white);
	void Tint(int color) {
		Tint(0, color);
	}
	void Gamma(float gamma);
	array<Byte>^ GetSampleBytes();
	Pixmap^ Mask(Pixmap^ mask);
	/// <summary>
	/// Gets a new Pixmap with converted colorspace.
	/// </summary>
	/// <param name="colorspace">Target colorspace.</param>
	/// <param name="params">Color params, typically used by CMYK rendition.</param>
	Pixmap^ ConvertColorspace(ColorspaceKind colorspace, ColorParams^ params);
internal:
	Pixmap(fz_pixmap* pixmap) : _pixmap(pixmap) {
		auto ctx = Context::Ptr;
		_width = fz_pixmap_width(ctx, pixmap);
		_height = fz_pixmap_height(ctx, pixmap);
		_components = fz_pixmap_components(ctx, pixmap);
		_stride = fz_pixmap_stride(ctx, pixmap);
		_samples = (IntPtr)(void*)fz_pixmap_samples(ctx, pixmap);
	};
	~Pixmap() {
		DropHandle(_pixmap, fz_drop_pixmap);
	}
	property fz_pixmap* Ptr {
		fz_pixmap* get() { return _pixmap; }
	}
private:
	fz_pixmap* _pixmap;
	int _width, _height, _stride, _components;
	IntPtr _samples;
	MuPDF::Colorspace^ _colorspace;

	!Pixmap() {
		DropHandle(_pixmap, fz_drop_pixmap);
	}
};

};
#endif
