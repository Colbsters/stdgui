#ifndef STDGUI_CORE_H
#define STDGUI_CORE_H


#ifdef STDGUI_EXPORTS
#define STDGUI_API __declspec(dllexport)
#else
#define STDGUI_API __declspec(dllimport)
#endif

namespace stdgui
{
	// Just to make sure that a .lib (exports) file gets generated, will be removed
	STDGUI_API void foo();
}

#endif // !STDGUI_CORE_H
