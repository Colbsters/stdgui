#include "pch.h"
#include <stdgui/graphics.h>

#include <d2d1/d2d1_graphics.h>

namespace stdgui
{
    std::shared_ptr<graphics> make_graphics(graphics_props props)
    {
        std::shared_ptr<graphics> grph;
#if defined(_WINDOWS) || defined(_WIN32) // Windows vvv
        switch (props.api)
        {
        case graphics_api::def:
        case graphics_api::direct2d:
            grph = std::make_shared<d2d1::d2d1_graphics>();
        }
#else // ^^^ Windows ||| Unsupported platform vvv
        return std::shared_ptr<graphics>(nullptr);
#endif // ^^^ Unsupported platform
        grph->create(props);
        return grph;
    }
}
