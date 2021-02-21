#ifndef STDGUI_WIN32_WINDOW_H
#define STDGUI_WIN32_WINDOW_H

#include <stdgui/window.h>

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace stdgui
{
	namespace win32
	{
		class win32_window
			:public window,
			public std::enable_shared_from_this<win32_window>
		{
		public:
			win32_window();
			~win32_window();

			virtual bool create(const window_props& props) override;
			virtual bool destroy() override;

			virtual bool update() override;
			
			virtual void repaint() override;
			virtual void resize(const size<int>& new_size) override;
			
			virtual rect<int> get_window_rect() override;
			virtual rect<int> get_client_rect() override;
			virtual window_api get_window_api() override;
			virtual void* get_native_handle() override;
			
			virtual void push_event_handler(const window_event_handler& event_handler) override;
			virtual void set_paint_handler(const window_event_handler& paint_handler) override;
		private:
			static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

			std::vector<window_event_handler> m_eventHandlers;
			window_event_handler m_paintHandler;

			bool m_isRunning;
			HWND m_hWnd;
			static ATOM m_clsAtom;
		};
	}
}

#endif