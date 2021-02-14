// Just to varify that things will compile
#include <stdgui/stdgui_core.h>

#include <stdgui/point.h>
#include <stdgui/rect.h>

#include <iostream>

using namespace stdgui::operators;

int main()
{
	stdgui::rect<int> rc(std::move(stdgui::make_rect_size(100, 100, 400, 300)));
	stdgui::point<int> pt(300, 250);
	std::cout << stdgui::point_in_rect(pt, rc) << '\n';
	std::cout << "pt=" << pt << '\n';
	pt = translate(pt, stdgui::point(0, 300));

	std::cout << "rc=" << rc << '\n';
	std::cout << "pt=" << pt << '\n';
	std::cout << stdgui::point_in_rect(pt, rc) << '\n';

	return 0;
}
