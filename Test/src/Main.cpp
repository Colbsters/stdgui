// Just to varify that things will compile
#include <stdgui/stdgui_core.h>

#include <stdgui/window.h>

#include <iostream>
#include <memory>

using namespace stdgui::operators;

void EventHandler(const stdgui::window_event& event)
{
	switch (event.code)
	{
	case stdgui::window_event::create:
		std::cout << "Window created\n";
		break;
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
		std::cout << "Window painted\n";
		break;
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

	while (window->update());

	return 0;
}
