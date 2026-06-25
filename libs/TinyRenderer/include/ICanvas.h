#pragma once

//#include "pch.h"

namespace TinyRenderer
{
	struct RgbColor
	{
		union
		{
			struct
			{

				unsigned char B, G, R, A;
			};
			unsigned int value;
		};

		RgbColor(unsigned int rgbaValue) : value(rgbaValue) {}
		RgbColor(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) : R(r), G(g), B(b), A(alpha) {}
		RgbColor() : RgbColor(0) {}
	};

	namespace Colors
	{
		constexpr int White = 0xFFFFFF00;
		constexpr int Blue = 0x00FF0000;
		constexpr int Green = 0x0000FF00;
		constexpr int Black = 0x00000000;
	}

	enum class ColorFormat
	{
		GrayScale = 1,
		RGB = 3,
		RGBA = 4
	};

	class ICanvas
	{
	public:
		virtual bool Scale(int width, int heigth) = 0;
		virtual bool SetPixel(int x, int y, const RgbColor& color) = 0;
		virtual bool Fill(const RgbColor& color) = 0;
		virtual bool filp_horizontally() = 0;
		virtual bool flip_vertically() = 0;

		virtual RgbColor GetPixel(int x, int y) const = 0;
		virtual ColorFormat GetColorFormat() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeigth() const = 0;

		virtual void SwapBuffers() = 0;
		virtual void CleanBackBuffer();

		~ICanvas() = default;
	};
	
}