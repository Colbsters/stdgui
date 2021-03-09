
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include <stdgui/rect.h>
#include <stdgui/point.h>
#include <stdgui/window.h>
#include <stdgui/graphics.h>

// Use literals and operators
using namespace stdgui::operators;
using namespace std::chrono_literals;


void EventHandler(const stdgui::window_event& event)
{
	static std::shared_ptr<stdgui::graphics> grph;

	switch (event.code)
	{
	case stdgui::window_event::create:
	{
		stdgui::graphics_props grphprops;
		grphprops.wnd = event.wnd;
		grph = stdgui::make_graphics(grphprops);
		std::cout << "Window created\n";
		break;
	}
	case stdgui::window_event::destroy:
		std::cout << "Window destroyed\n";
		break;
	case stdgui::window_event::moving:
		std::cout << "Window moving\nnew position:\n   " << *((const stdgui::rect<int>*)event.params) << '\n';
		break;
	case stdgui::window_event::resizing:
		std::cout << "Window resizing\nnew position:\n   " << *((const stdgui::rect<int>*)event.params) << '\n';
		break;
	case stdgui::window_event::paint:
	{
		grph->begin_draw();



		grph->end_draw();
		std::cout << "Window painted\n";
		break;
	}
	default:
		break;
	}
}

int main()
{
	stdgui::window_props wndprops("Window");
	wndprops.event_handlers.push_back(EventHandler);
	wndprops.paint_handler = EventHandler;
	auto window = stdgui::make_window(wndprops);

	while (window->update()) std::this_thread::sleep_for(10ms);

	return 0;
}
