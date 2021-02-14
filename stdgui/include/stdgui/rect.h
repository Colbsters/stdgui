#ifndef STDGUI_RECT_H
#define STDGUI_RECT_H

#include <utility>
#include <ostream>

#include "point.h"

namespace stdgui
{
	// A basic rectangle structure
	template<typename T>
	struct rect
	{
		T left, top, right, bottom;

		rect()
			:left(0), top(0), right(0), bottom(0)
		{
		}

		// Left and top are set to 0
		rect(T width, T height)
			:left(0), top(0), right(width), bottom(height)
		{
		}

		rect(const point<T>& top_left, const point<T>& bottom_right)
			:left(top_left.x), top(top_left.y), right(bottom_right.x), bottom(bottom_right.y)
		{
		}

		rect(T left, T top, T right, T bottom)
			:left(left), top(top), right(right), bottom(bottom)
		{
		}

		rect(const rect& rc)
			:left(rc.left), top(rc.top), right(rc.right), bottom(rc.bottom)
		{
		}

		rect(rect&& rc)
			:left(std::move(rc.left)), top(std::move(rc.top)), right(std::move(rc.right)), bottom(std::move(rc.bottom))
		{
		}

		inline T width() const { return right - left; }
		inline T height() const { return bottom - top; }

		// Flips the rectangle about the vertical axis
		inline rect flip() const
		{
			return rect(left, bottom, right, top);
		}

		// Translates the rectangle
		// NOTES:
		//	- Moves by amount, does not set position
		//	- This function is destructive (changes value)
		inline rect& translate(const point<T>& trans)
		{
			left += trans.x;
			top += trans.y;
			right += trans.x;
			bottom += trans.y;
			return *this;
		}

		bool operator==(const rect& other)
		{
			return ((left == other.left) &&
					(top == other.top) &&
					(right == other.right) &&
					(bottom == other.bottom));
		}

		bool operator!=(const rect& other)
		{
			return ((left != other.left) &&
					(top != other.top) &&
					(right != other.right) &&
					(bottom != other.bottom));
		}
		
		rect& operator=(const rect& other)
		{
			left = other.left;
			top = other.top;
			right = other.right;
			bottom = other.bottom;
			return *this;
		}
	};

	template<typename T>
	inline rect<T> make_rect(T x1, T y1, T x2, T y2)
	{
		return rect(x1, y1, x2, y2);
	}

	template<typename T>
	inline rect<T> make_rect(const point<T>& pt1, const point<T>& pt2)
	{
		return rect(pt1, pt2);
	}

	// Creates a rect with a position and a size instead of 2 positions
	template<typename T>
	inline rect<T> make_rect_size(T x, T y, T width, T height)
	{
		return rect(x, y, x + width, y + height);
	}

	// Creates a rect with a position and a size instead of 2 positions
	template<typename T>
	inline rect<T> make_rect_size(coord<T>& pos, size<T>& size)
	{
		return rect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	}

	template<typename T>
	inline bool point_in_rect(const point<T>& pt, const rect<T>& rc)
	{
		return ((rc.left < pt.x) && (pt.x < rc.right) &&
				(rc.top < pt.y) && (pt.y < rc.bottom));
	}

	// Translates the rectangle
	// NOTES:
	//	- Moves by amount, does not set position
	template<typename T>
	inline rect<T> translate(const rect<T>& rc, const point<T>& trans)
	{
		return rect(rc.left + trans.x, rc.top + trans.y, rc.right + trans.x, rc.bottom + trans.y);
	}

	namespace operators
	{
		template<typename T>
		inline std::ostream& operator<<(std::ostream& stream, const rect<T>& rc)
		{
			stream << "{ LT={" << rc.left << ", " << rc.top << "}, RB={" << rc.right << ", " << rc.bottom << "} [" << rc.width() << 'x' << rc.height() << "] }";
			return stream;
		}
	}
}

#endif // !STDGUI_RECT_H