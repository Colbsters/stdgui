#ifndef STDGUI_GRAPHICS_H
#define STDGUI_GRAPHICS_H

#include <stdgui/stdgui_core.h>
#include <stdgui/window.h>

namespace stdgui
{
	enum class graphics_api
	{
		null = 0,
		def,
		direct2d
	};

	enum class graphics_mode
	{
		retained,
		immediate
	};

	struct graphics_props
	{
		std::shared_ptr<window> wnd;

		graphics_mode draw_mode = graphics_mode::retained;
		graphics_api api = graphics_api::def;
	};

	// A base graphics class
	// NOTE: This class should only be created (call create()) once, during the creation of the window
	class graphics
	{
	public:
		graphics() {}
		virtual ~graphics() {}

		virtual bool create(graphics_props props) = 0;
		virtual bool destroy() = 0;

		// This function must be called before any rendering command is made
		// This function call must also be matched with a end_draw() call
		// NOTE: This function will return false if:
		//	This graphics instance is still in a draw state (end_draw() hasn't been called since the last begin_draw() call)
		//	This graphics instance hasn't been initialized yet
		virtual bool begin_draw() = 0;
		// Called after window drawing is complete
		// NOTE: begin_draw() must be called before a call to this function.
		//	This function will return false if it wasn't
		virtual bool end_draw() = 0;
	};

	STDGUI_API std::shared_ptr<graphics> make_graphics(graphics_props props);
}

#endif // STDGUI_GRAPHICS_H