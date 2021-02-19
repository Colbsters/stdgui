#include "pch.h"
#include <stdgui/window.h>

#include "win32/win32_window.h"

namespace stdgui
{
    std::shared_ptr<window> make_window(const window_props& props)
    {
        std::shared_ptr<window> wnd;
#if defined(_WINDOWS) || defined(WIN32) // Windows vvv
        wnd = std::make_shared<win32::win32_window>();
#else // ^^^ Windows ||| Unsupported platform vvv
        return std::shared_ptr<window>(nullptr);
#endif // ^^^ Unsupported platform
        wnd->create(props, wnd);
        return wnd;
    }
}
