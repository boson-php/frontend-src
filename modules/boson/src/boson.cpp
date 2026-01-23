#include "boson.h"

#include <string.h>

#define BOSON_STRING(value) #value
#define BOSON_STRING_EXPAND(value) BOSON_STRING(value)

#ifndef BOSON_VERSION
    #define BOSON_VERSION master
#endif

#ifndef SAUCER_BACKEND
    #define SAUCER_BACKEND Default
#endif


extern "C"
{
    const char* boson_version(void)
    {
        return BOSON_STRING_EXPAND(BOSON_VERSION) "";
    }

    const char* boson_get_backend_str(void)
    {
        return BOSON_STRING_EXPAND(SAUCER_BACKEND) "";
    }

    boson_backend boson_get_backend(void)
    {
        const char* boson_backend_str = BOSON_STRING_EXPAND(SAUCER_BACKEND) "";

        if (strcmp(boson_backend_str, "WebView2") == 0) {
            return boson_backend::BOSON_BACKEND_WEBVIEW2;
        }
        else if (strcmp(boson_backend_str, "WebKit") == 0) {
            return boson_backend::BOSON_BACKEND_WEBKIT;
        }
        else if (strcmp(boson_backend_str, "WebKitGtk") == 0) {
            return boson_backend::BOSON_BACKEND_WEBKIT_GTK;
        }
        else if (strcmp(boson_backend_str, "Qt") == 0) {
            return boson_backend::BOSON_BACKEND_QT;
        }

        return boson_backend::BOSON_BACKEND_UNKNOWN;
    }
}
