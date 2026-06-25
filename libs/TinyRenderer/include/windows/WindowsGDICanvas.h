#pragma once
#include "../pch.h"
#include "ICanvas.h"
#include "Geometry.h"

namespace TinyRenderer::Windows
{
	class WindowsGDICanvas : public ICanvas
	{
	public:
		WindowsGDICanvas(HWND hWnd, Helpers::Vector2i& canvasSize);

		[[deprecated("Use instead 'CreateBackbufferDIBitmapCopy'. You should not use this function because the bitmap can be deleted while in use")]]
		HRESULT GetBitmapBuffer(_Notnull_ _Out_ BITMAP* const hBitMap) const;
		/// <summary>
		/// Creates a compatible bitmap containing the back buffer data.
		/// *Remarks* You must call DeleteObject after using the bitmap
		/// </summary>
		/// <param name="hMemDc"></param>
		/// <returns>On success the functions resturns zero</returns>
		HRESULT CreateBackbufferDIBitmapCopy(_Notnull_ _Out_ HBITMAP* const hBitmap) const;
		WindowsGDICanvas& FitCanvasToWindow();
		WindowsGDICanvas& StretchCanvasOnWindowResize(bool stretchCanvas);
		WindowsGDICanvas& SetCanvasSize(Helpers::Vector2i size, _Out_opt_ HRESULT* phResult);


		// Inherited via ICanvas
		virtual bool Scale(int width, int heigth) override;
		virtual bool SetPixel(int x, int y, const RgbColor& color) override;
		virtual bool Fill(const RgbColor& color) override;
		virtual bool filp_horizontally() override;
		virtual bool flip_vertically() override;
		virtual RgbColor GetPixel(int x, int y) const override;
		virtual ColorFormat GetColorFormat() const override;
		virtual int GetWidth() const override;
		virtual int GetHeigth() const override;
		virtual void SwapBuffers() override;
		~WindowsGDICanvas();

	private:
		LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT __stdcall s_WndProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool initializeBitmap(int cx, int cy);
		void copyMemDcToDc(HDC dc);
		
		const HWND _hWnd;
		bool stretchCanvas;
		WNDPROC lpPrevWndProc;
		LPVOID lpvBitmapBits;
		HDC hMemDc;
		HBITMAP hBitmapBuffer;
		BITMAP bitMap;
		CRITICAL_SECTION cs;
	};
}