#pragma once
#include "ICanvas.h"

namespace TinyRenderer
{
	class TinyRenderer
	{
	private:
		ICanvas& canvas;

	public:
		TinyRenderer(ICanvas& canvas) : canvas(canvas) {}

		inline ICanvas& GetCanvas() const { return canvas; };
		void DrawLine(int x0, int y0, int x1, int y1, const RgbColor& color);
	};
	
}


