#include "pch.h"
#include <stdgui/window.h>

#include "win32/win32_window.h"

namespace stdgui
{
    std::shared_ptr<window> make_window_shared(const window_props& props)
    {
        std::shared_ptr<window> wnd;
#if defined(_WINDOWS) || defined(WIN32) // Windows vvv
        wnd = std::make_shared<win32::win32_window>();
#else // ^^^ Windows ||| Unsupported platform vvv
        return std::shared_ptr<window>(nullptr);
#endif // ^^^ Unsupported platform
        wnd->create(props);
        return wnd;
    }

    std::unique_ptr<window> make_window_unique(const window_props& props)
    {
        std::unique_ptr<window> wnd;
#if defined(_WINDOWS) || defined(WIN32) // Windows vvv
        wnd = std::make_unique<win32::win32_window>();
#else // ^^^ Windows ||| Unsupported platform vvv
        return std::unique_ptr<window>(nullptr);
#endif // ^^^ Unsupported platform
        wnd->create(props);
        return wnd;
    }
}
