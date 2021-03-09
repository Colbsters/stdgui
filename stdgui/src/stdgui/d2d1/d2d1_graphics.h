#ifndef STDGUI_D2D1_GRAPHICS_H
#define STDGUI_D2D1_GRAPHICS_H

#include <stdgui/graphics.h>
#include <stdgui/window.h>

struct ID2D1Factory;
struct ID2D1HwndRenderTarget;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace stdgui
{
	namespace d2d1
	{
		struct d2d1_graphics_data
		{
			graphics_props props;

			ID2D1Factory* d2dFactory;
			ID2D1HwndRenderTarget* d2dRt;
		};

		class d2d1_graphics
			:public graphics
		{
		public:
			d2d1_graphics();
			~d2d1_graphics();

			virtual bool create(const graphics_props& props) override;

			virtual bool destroy() override;

			virtual bool begin_draw() override;

			virtual bool end_draw() override;

			virtual graphics_api api() override { return graphics_api::direct2d; }

			// returns a d2d1_graphics_data struct
			virtual std::unique_ptr<void> data() override;
		private:
			bool create_rendertarget_resources();

			void event_handler(const window_event& event);

			ID2D1Factory* m_d2dFactory;
			ID2D1HwndRenderTarget* m_d2dRt;

			bool m_isDrawing;

			PAINTSTRUCT m_ps;
			graphics_props m_props;
		};
	}
}

#endif // STDGUI_D2D1_GRAPHICS_H