#pragma once

#include <v8.h>
#include <libplatform/libplatform.h>

class v8_platform_t {
private:
	std::unique_ptr<v8::Platform> m_platform;
	v8::Isolate::CreateParams m_params;

public:
	v8_platform_t();
	~v8_platform_t();

	v8::Isolate* create_isolate();
};
