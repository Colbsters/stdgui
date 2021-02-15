// Just to varify that things will compile
#include <stdgui/stdgui_core.h>

#include <stdgui/window.h>

#include <iostream>
#include <memory>

using namespace stdgui::operators;

int main()
{
	stdgui::window_props wndprops("Window");
	auto window = stdgui::make_window(wndprops);

	return 0;
}
