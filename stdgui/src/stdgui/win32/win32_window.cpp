#include "pch.h"
#include "win32_window.h"

namespace stdgui
{
    namespace win32
    {
        ATOM win32_window::m_clsAtom = 0;

        win32_window::win32_window()
            :m_eventHandlers(), m_paintHandler(nullptr), m_isRunning(false), m_hWnd(nullptr)
        {
        }

        win32_window::~win32_window()
        {
        }

        bool win32_window::create(const window_props& props, std::shared_ptr<window>& this_window)
        {
            m_eventHandlers = props.event_handlers;
            m_paintHandler = props.paint_handler;
            m_thisWnd = this_window;

            if (!m_clsAtom)
            {
                WNDCLASSEXW wc = { 0 };
                wc.cbSize = sizeof(WNDCLASSEXW);
                wc.cbClsExtra = 0;
                wc.cbWndExtra = sizeof(win32_window);
                wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
                wc.hCursor = LoadCursorW(0, IDC_ARROW);
                wc.hIcon = LoadIconW(0, IDI_APPLICATION);
                wc.hIconSm = LoadIconW(0, IDI_APPLICATION);
                wc.hInstance = GetModuleHandleW(0);
                wc.lpfnWndProc = WndProc;
                wc.lpszClassName = L"stdgui_wndcls_1";
                wc.lpszMenuName = nullptr;
                wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

                m_clsAtom = RegisterClassExW(&wc);

                if (!m_clsAtom)
                    return false;
            }


            auto windowNameW = std::make_unique<wchar_t[]>(props.name.length() + 1);
            size_t temp;
            mbstowcs_s(&temp, windowNameW.get(), props.name.length() + 1, props.name.c_str(), props.name.length());

            // NOTE: m_hWnd gets set in WndProc, not here
            CreateWindowExW(0,
                MAKEINTATOM(m_clsAtom),
                windowNameW.get(),
                WS_OVERLAPPEDWINDOW,
                props.position.left,
                props.position.top,
                props.position.width(),
                props.position.height(),
                nullptr, nullptr, GetModuleHandleW(0), this);
            
            int showCmd;
            switch (props.state)
            {
            case window_state::restored:
                showCmd = SW_RESTORE;
            case window_state::minimized:
                showCmd = SW_MINIMIZE;
            case window_state::maximized:
                showCmd = SW_MAXIMIZE;
            case window_state::hidden:
                showCmd = SW_HIDE;
            default:
                showCmd = SW_SHOW;
            }
            ShowWindow(m_hWnd, showCmd);

            m_isRunning = true;

            return true;
        }

        bool win32_window::destroy()
        {
            if (!DestroyWindow(m_hWnd))
                return false;

            return true;
        }

        bool win32_window::update()
        {
            MSG msg;
            while (PeekMessageW(&msg, m_hWnd, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            return m_isRunning;
        }

        void win32_window::repaint()
        {
            InvalidateRect(m_hWnd, 0, true);
            UpdateWindow(m_hWnd);
        }

        void win32_window::resize(const size<int>& new_size)
        {
            SetWindowPos(m_hWnd, HWND_TOP, 0, 0, new_size.x, new_size.y, SWP_NOMOVE);
        }

        rect<int> win32_window::get_window_rect()
        {
            RECT rc;
            GetWindowRect(m_hWnd, &rc);
            return make_rect<int>(rc.left, rc.top, rc.right, rc.bottom);
        }

        rect<int> win32_window::get_client_rect()
        {
            RECT rc;
            GetClientRect(m_hWnd, &rc);
            return make_rect<int>(rc.left, rc.top, rc.right, rc.bottom);
        }

        window_api win32_window::get_window_api()
        {
            return window_api::win32;
        }

        const void* win32_window::get_native_handle()
        {
            return &m_hWnd;
        }

        void win32_window::push_event_handler(const window_event_handler& event_handler)
        {
            m_eventHandlers.push_back(event_handler);
        }

        void win32_window::set_paint_handler(const window_event_handler& paint_handler)
        {
            m_paintHandler = paint_handler;
        }

        LRESULT win32_window::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
        {
            win32_window* window = nullptr;
            if (msg == WM_NCCREATE)
            {
                LPCREATESTRUCTW cs = (LPCREATESTRUCTW)lparam;
                window = (win32_window*)cs->lpCreateParams;
                // Store the window pointer for later use
                SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)window);
                // Set m_hWnd
                window->m_hWnd = hWnd;
            }
            else
            {
                window = (win32_window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
            }

            if (window)
            {
                window_event event = { window_event::null, window->m_thisWnd, nullptr };
                void* eventParams = nullptr;

                // Setup messages for dispatching
                switch (msg)
                {
                case WM_CREATE:
                {
                    event.code = window_event::create;
                    break;
                }
                case WM_DESTROY:
                {
                    event.code = window_event::destroy;
                    break;
                }
                // TODO: case WM_KEYDOWN
                // TODO: case WM_KEYUP
                // TODO: case WM_MOUSEMOVE // mouse_move and mouse_button
                case WM_MOVING:
                {
                    event.code = window_event::moving;
                    LPRECT rc = (LPRECT)lparam;
                    eventParams = new rect<int>(rc->left, rc->top, rc->right, rc->bottom);
                    break;
                }
                case WM_SIZING:
                {
                    event.code = window_event::resizing;
                    LPRECT rc = (LPRECT)lparam;
                    eventParams = new rect<int>(rc->left, rc->top, rc->right, rc->bottom);
                    break;
                }
                case WM_PAINT:
                {
                    event.code = window_event::paint;
                    break;
                }
                default:
                    break;
                }

                event.params = eventParams;

                // Check to make sure event is handleable
                // Paint is handled differently
                if (event.code == window_event::paint)
                {
                    if (window->m_paintHandler)
                        window->m_paintHandler(event);
                }
                else if (event.code != window_event::null)
                {
                    for (auto& eventHandler : window->m_eventHandlers)
                        eventHandler(event);
                }

                if (msg == WM_DESTROY)
                {
                    window->m_isRunning = false;
                    PostQuitMessage(0);
                }
            }

            return DefWindowProcW(hWnd, msg, wparam, lparam);
        }
    }
}
