#pragma once

#include <cmath>

namespace TinyRenderer::Helpers
{
	template <typename T>
	struct Vector2
	{
		union
		{
			struct { T X, Y; };
			struct { T u, v; };
			T raw[2];
		};

		Vector2() : u(0), v(0) {}
		Vector2(T u, T v) : u(u), v(v) {}
	};

	template<typename T>
	struct Vector3
	{
		union
		{
			struct { T X, Y, Z; };
			struct { T ivert, iuv, inorm; };
			T raw[3];
		};

		Vector3(T x, T y, T z) : X(x), Y(y), Z(0) {}
		Vector3() : Vector3(0, 0, 0) {}
	};

	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector3<int> Vector3i;
	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
	
}