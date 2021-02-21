#ifndef STDGUI_WINDOW_H
#define STDGUI_WINDOW_H

#include <stdgui/stdgui_core.h>
#include <stdgui/rect.h>
#include <stdgui/point.h>
#include <string>
#include <functional>
#include <vector>

namespace stdgui
{
	class window;

	struct window_event
	{
		enum event_code
		{
			null = 0,
			create,
			destroy,
			paint,
			moving,
			resizing,
			key_down,
			key_up,
			mouse_move,
			mouse_button
		};
		event_code code;

		std::shared_ptr<window> wnd;
		const void* params;
	};

	using window_event_handler = std::function<void(const window_event&)>;

	enum class window_state
	{
		normal = 0,
		minimized,
		maximized,
		fullscreen,
		hidden,
		restored = window_state::normal
	};

	struct window_props
	{
		window_props() = default;

		window_props(const std::string& name,
			const rect<int>& pos = make_rect_size(200, 100, 640, 480), const window_state& state = window_state::normal,
			const std::vector<window_event_handler>& event_handlers = std::vector<window_event_handler>())
			:name(name), position(pos), state(state), event_handlers(event_handlers)
		{
		}

		std::string name;
		rect<int> position = make_rect_size(200, 100, 640, 480);
		window_state state = window_state::normal;
		// This function will only recieve paint messages if not entered in the vector of event handlers
		window_event_handler paint_handler;
		// These functions will only recieve messages that aren't paint messages
		std::vector<window_event_handler> event_handlers;
	};

	enum class window_api
	{
		null,
		win32
	};

	class window
	{
	public:
		window() {}
		virtual ~window() {}
		
		virtual bool create(const window_props& props) = 0;
		virtual bool destroy() = 0;

		virtual bool update() = 0;

		virtual void repaint() = 0;
		virtual void resize(const size<int>& new_size) = 0;

		virtual rect<int> get_window_rect() = 0;
		virtual rect<int> get_client_rect() = 0;
		virtual window_api get_window_api() = 0;
		// Returns a pointer to the handle type (ex: HWND*, not HWND) 
		virtual const void* get_native_handle() = 0;

		virtual void push_event_handler(const window_event_handler& event_handler) = 0;
		virtual void set_paint_handler(const window_event_handler& paint_handler) = 0;
	};

	STDGUI_API std::shared_ptr<window> make_window(const window_props& props);
}

#endif // STDGUI_WINDOW_H