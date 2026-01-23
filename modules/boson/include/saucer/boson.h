#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "export.h"

    enum boson_backend
    {
        BOSON_BACKEND_WEBVIEW2,
        BOSON_BACKEND_WEBKIT,
        BOSON_BACKEND_WEBKIT_GTK,
        BOSON_BACKEND_QT,
        BOSON_BACKEND_UNKNOWN = -1,

        BOSON_BACKEND_LAST = BOSON_BACKEND_QT,
    };

    BOSON_EXPORT extern const char* boson_version(void);

    BOSON_EXPORT extern boson_backend boson_get_backend(void);
    BOSON_EXPORT extern const char* boson_get_backend_str(void);


#ifdef __cplusplus
}
#endif
