#ifndef STDGUI_POINT_H
#define STDGUI_POINT_H

#include <utility>
#include <ostream>

namespace stdgui
{
	// A simple point structure
	template<typename T>
	struct point
	{
		T x, y;

		point()
			:x(0), y(0)
		{
		}

		point(T x, T y)
			:x(x), y(y)
		{
		}

		point(const point& pt)
			:x(pt.x), y(pt.y)
		{
		}

		point(point&& pt)
			:x(std::move(pt.x)), y(std::move(pt.y))
		{
		}

		// Translates the position
		// NOTES:
		//	- Moves by amount, does not set position
		//	- This function is destructive (changes value)
		inline point& translate(const point& trans)
		{
			x += trans.x;
			y += trans.y;
			return *this;
		}

		inline bool operator==(const point& other)
		{
			return ((x == other.x) && (y == other.y));
		}

		inline bool operator!=(const point& other)
		{
			return ((x != other.x) && (y != other.y));
		}

		inline point& operator=(const point& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}
	};

	// 2D coordinate
	template<typename T>
	using coord = point<T>;
	// 2D size
	template<typename T>
	using size = point<T>;

	template<typename T>
	inline point<T> make_point(T x, T y)
	{
		return point<T>(x, y);
	}

	template<typename T>
	inline coord<T> make_coord(T x, T y)
	{
		return coord<T>(x, y);
	}

	template<typename T>
	inline size<T> make_size(T x, T y)
	{
		return size<T>(x, y);
	}

	// Translates the position
	// NOTES:
	//	- Moves by amount, does not set position
	template<typename T>
	inline point<T> translate(const point<T>& pt, const point<T>& trans)
	{
		return point(pt.x + trans.x, pt.y + trans.y);
	}

	namespace operators
	{
		template<typename T>
		inline std::ostream& operator<<(std::ostream& stream, const point<T>& pt)
		{
			stream << "{ x=" << pt.x << ", y=" << pt.y << " }";
			return stream;
		}
	}
}

#endif // !STDGUI_POINT_H