#pragma once

#include <v8.h>
#include <libplatform/libplatform.h>

class v8_platform_t {
public:
    class v8_lock_t {
    private:
        v8::Locker m_lock;
        v8::Isolate::Scope m_isolate_scope;
        v8::HandleScope m_handle_scope;

    public:
        v8_lock_t(v8::Isolate* isolate) :
            m_lock(isolate),
            m_isolate_scope(isolate),
            m_handle_scope(isolate)
        {
        }
    };

private:
    std::unique_ptr<v8::Platform> m_platform;
    v8::Isolate::CreateParams m_params;

public:
    v8_platform_t();
    ~v8_platform_t();

    v8::Isolate* create_isolate();
};
