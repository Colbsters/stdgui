#include "pch.h"
#include <stdgui/window.h>

namespace stdgui
{
    std::shared_ptr<window> make_window_shared(const window_props& props)
    {
        return std::shared_ptr<window>(nullptr);
    }

    std::unique_ptr<window> make_window_unique(const window_props& props)
    {
        return std::unique_ptr<window>(nullptr);
    }
}
