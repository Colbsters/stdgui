#include "pch.h"
#include "d2d1_graphics.h"

namespace stdgui
{
    namespace d2d1
    {
        d2d1_graphics::d2d1_graphics()
            :m_d2dFactory(nullptr), m_d2dRt(nullptr), m_isDrawing(false), m_props()
        {
        }

        d2d1_graphics::~d2d1_graphics()
        {
            destroy();
        }

        bool d2d1_graphics::create(const graphics_props& props)
        {
            m_props = props;

            HRESULT hres;

            D2D1_FACTORY_OPTIONS factOpt;
#if defined(_DEBUG) || defined(DEBUG)
            factOpt.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
            factOpt.debugLevel = D2D1_DEBUG_LEVEL_ERROR;
#endif
            hres = D2D1CreateFactory(
                D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED,
                __uuidof(ID2D1Factory),
                &factOpt,
                reinterpret_cast<void**>(&m_d2dFactory));

            if (FAILED(hres))
                return false;

            if (!create_rendertarget_resources())
                return false;

            props.wnd->push_event_handler(std::bind(&d2d1_graphics::event_handler, this, std::placeholders::_1));

            return true;
        }

        bool d2d1_graphics::destroy()
        {
            m_d2dRt->Release();
            m_d2dFactory->Release();

            return true;
        }

        bool d2d1_graphics::begin_draw()
        {
            if (m_d2dRt && !m_isDrawing)
            {
                if (m_props.draw_mode == graphics_mode::retained)
                    BeginPaint(*reinterpret_cast<HWND*>(m_props.wnd->get_native_handle()), &m_ps);
                m_d2dRt->BeginDraw();
            }
            else
            {
                return false;
            }

            m_isDrawing = true;

            return true;
        }

        bool d2d1_graphics::end_draw()
        {

            if (m_d2dRt && m_isDrawing)
            {
                HRESULT hres = m_d2dRt->EndDraw();
                if (hres == D2DERR_RECREATE_TARGET)
                    create_rendertarget_resources();
                else if (FAILED(hres))
                    return false;

                if (m_props.draw_mode == graphics_mode::retained)
                    EndPaint(*reinterpret_cast<HWND*>(m_props.wnd->get_native_handle()), &m_ps);
            }
            else
            {
                return false;
            }

            return true;
        }

        std::unique_ptr<void> d2d1_graphics::data()
        {
            std::unique_ptr<d2d1_graphics_data> data = std::make_unique<d2d1_graphics_data>();
            data->props = m_props;
            data->d2dFactory = m_d2dFactory;
            data->d2dRt = m_d2dRt;
            return data;
        }

        bool d2d1_graphics::create_rendertarget_resources()
        {
            HRESULT hres;

            rect<int> rc = m_props.wnd->get_client_rect();
            hres = m_d2dFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(
                    D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT),
                D2D1::HwndRenderTargetProperties(
                    *reinterpret_cast<HWND*>(m_props.wnd->get_native_handle()),
                    D2D1::SizeU(rc.width(), rc.height())),
                &m_d2dRt);

            if (FAILED(hres))
                return false;

            return true;
        }

        void d2d1_graphics::event_handler(const window_event& event)
        {
            using namespace stdgui::operators;

            switch (event.code)
            {
            case window_event::resizing:
            {
                rect<int> rc = event.wnd->get_client_rect();
                m_d2dRt->Resize(D2D1::SizeU(rc.width(), rc.height()));
                std::cout << "Direct2D render target resized to: " << rc << "\n";
                break;
            }
            default:
                break;
            }
        }
    }
}
