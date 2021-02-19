#include "pch.h"
#include <stdgui/graphics.h>

namespace stdgui
{
    std::shared_ptr<graphics> make_graphics(graphics_props props)
    {
        std::shared_ptr<graphics> grph;
#if 1 // Unsupported platform vvv
        return std::shared_ptr<graphics>(nullptr);
#endif // ^^^ Unsupported platform
        return grph;
    }
}
