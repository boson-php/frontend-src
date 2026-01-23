#include "desktop_extensions.h"

#include <system_error>

extern "C"
{

    int boson_error_code_canceled(void)
    {
        return static_cast<int>(std::errc::operation_canceled);
    }

}
