#include "precompiled.hpp"
#include "v8_platform.hpp"

v8_platform_t::v8_platform_t() {
	m_platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(m_platform.get());
	v8::V8::Initialize();

	m_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

	LOG << "V8 platform started";
}

v8_platform_t::~v8_platform_t() {
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();
	delete m_params.array_buffer_allocator;

	LOG << "V8 platform stopped";
}

v8::Isolate* v8_platform_t::create_isolate() {
	return v8::Isolate::New(m_params);
}
