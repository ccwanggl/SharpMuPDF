#include "Buffer.h"

using namespace MuPDF;

array<Byte>^ MuPDF::Buffer::ToArray() {
	if (!_buf) return nullptr;
	unsigned char* data = nullptr;
	size_t l = fz_buffer_storage(Context::Ptr, _buf, &data);
	if (!l || !data) return nullptr;
	array<Byte>^ r = gcnew array<Byte>((int)l);
	pin_ptr<Byte> p = &r[0];
	memcpy(p, data, l);
	return r;
}

void MuPDF::Buffer::Append(array<Byte>^ data) {
	if (!_buf || data == nullptr || data->Length == 0) return;
	pin_ptr<Byte> p = &data[0];
	fz_append_data(Context::Ptr, _buf, p, data->Length);
}

String^ MuPDF::Buffer::GetString() {
	if (!_buf) return String::Empty;
	const char* s = fz_string_from_buffer(Context::Ptr, _buf);
	if (!s) return String::Empty;
	return gcnew String(s);
}

array<Byte>^ MuPDF::Buffer::Extract() {
	if (!_buf) return gcnew array<Byte>(0);
	unsigned char* data = nullptr;
	size_t l = fz_buffer_extract(Context::Ptr, _buf, &data);
	if (!l || !data) return gcnew array<Byte>(0);
	array<Byte>^ r = gcnew array<Byte>((int)l);
	pin_ptr<Byte> p = &r[0];
	memcpy(p, data, l);
	/* take ownership returned by fz_buffer_extract and free it */
	fz_free(Context::Ptr, data);
	return r;
}

MuPDF::Buffer^ MuPDF::Buffer::FromArray(array<Byte>^ data) {
	if (!data || !data->Length) {
		return gcnew Buffer(fz_new_buffer(Context::Ptr, 0));
	}
	pin_ptr<Byte> p = &data[0];
	fz_buffer* b = fz_new_buffer_from_copied_data(Context::Ptr, p, data->Length);
	return gcnew Buffer(b);
}

void MuPDF::Buffer::ReleaseHandle() {
	if (_buf) {
		fz_drop_buffer(Context::Ptr, _buf);
		_buf = NULL;
	}
}
