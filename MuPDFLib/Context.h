#include "fitz.h"
#include "ObjWrapper.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#ifndef __CONTEXT
#define __CONTEXT

namespace MuPDF {

#pragma warning( push )
#pragma warning( disable : 4091 )
typedef ref class Document;
typedef ref class Pixmap;
typedef value struct BBox;
// in mupdf_load_system_font.c
extern "C" void install_load_windows_font_funcs(fz_context* ctx);
#pragma warning (pop)

public ref class Context : IDisposable {
public:
	~Context() {
		ReleaseHandle();
	}

	/// <summary>
	/// Gets or sets rendition anti-alias level. Valid values are ranged [0, 8].
	/// </summary>
	static PropGetSet(int, AntiAlias,
		fz_aa_level(Ptr),
		fz_set_aa_level(Ptr, value)
	);

	/// <summary>
	/// Gets or sets text rendition anti-alias level. Valid values are ranged [0, 8].
	/// </summary>
	static PropGetSet(int, TextAntiAlias,
		fz_text_aa_level(Ptr),
		fz_set_text_aa_level(Ptr, value)
	);

	static void SetErrorCallback(Action<bool, String^>^ callback);

internal:
	static property Context^ Current {
		Context ^ get();
	}

	static PropGet(fz_context*, Ptr, Current->_context);

protected:
	!Context() {
		ReleaseHandle();
	}

private:
	fz_context* _context;
	bool _disposed;
	initonly bool _isCloned;

	static Context^ MakeMainContext();
	static Context^ _MainInstance = MakeMainContext();
	[System::ThreadStaticAttribute]
		static Context^ _Instance = _MainInstance;

	Context(fz_context* ctx, bool isCloned) : _context(ctx), _isCloned(isCloned) {
		if (!ctx) {
			throw gcnew InvalidOperationException("fz_context is null");
		}
		install_load_windows_font_funcs(ctx);
		fz_register_document_handlers(ctx);
	}

	void ReleaseHandle();

	static GCHandle _errorCallbackHandle;
	static Action<bool, String^>^ _errorCallback;
	static void ErrorCallback(bool isError, const char* message);
};

};

#endif // !__CONTEXT
