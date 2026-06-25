#include "pch.h"
#include "windows/WindowsGDICanvas.h"
#include "windows/WindowsUtils.h"

using namespace TinyRenderer;
using TinyRenderer::Windows::WindowsGDICanvas;

constexpr LPCSTR NotImplementedMsg = "Functions not implemented";

LRESULT TinyRenderer::Windows::WindowsGDICanvas::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDc = BeginPaint(hWnd, &ps);
		copyMemDcToDc(hDc);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_SIZE:
	{
		if(!stretchCanvas)
			break;
		int cx = LOWORD(lParam);
		int cy = HIWORD(lParam);
		if(cx * cy <= bitMap.bmWidth * bitMap.bmHeight)
			break;
		initializeBitmap(cx, cy);
		break;
	}
	}
	return CallWindowProc(
		lpPrevWndProc,
		hWnd,
		msg,
		wParam,
		lParam
	);
}

LRESULT __stdcall TinyRenderer::Windows::WindowsGDICanvas::s_WndProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Windows::WindowsGDICanvas* pThis = reinterpret_cast<Windows::WindowsGDICanvas*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!pThis)
		return DefWindowProc(hWnd, msg, wParam, lParam);
	return pThis->WndProc(hWnd, msg, wParam, lParam);
}

TinyRenderer::Windows::WindowsGDICanvas::WindowsGDICanvas(HWND hWnd, Helpers::Vector2i& canvasSize) : 
	_hWnd(hWnd), 
	lpvBitmapBits(NULL),
	bitMap({0}),
	stretchCanvas(false),
	hBitmapBuffer(0),
	hMemDc(0)
{
	InitializeCriticalSection(&cs);
	RECT client { canvasSize.X, canvasSize.Y} ;
	if(canvasSize.X <= 0 || canvasSize.Y <= 0)
	{
		GetClientRect(hWnd, &client);
	}
	if (!initializeBitmap(client.right, client.bottom))
		throw std::runtime_error("CreateDIBSection failed!");
	auto result = SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	lpPrevWndProc = (WNDPROC)(SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)s_WndProcedure));
}

HRESULT TinyRenderer::Windows::WindowsGDICanvas::CreateBackbufferDIBitmapCopy(_Notnull_ _Out_ HBITMAP* const hBitmap) const
{
	if (!hBitmap)
		return E_INVALIDARG;

	RECT clientRc;
	GetClientRect(_hWnd, &clientRc);
	HDC hDestMemDc = CreateCompatibleDC(this->hMemDc);
	LPVOID lpvBits;
	*hBitmap = CreateCompatibleDIBSection(hDestMemDc, &lpvBits, clientRc.right, clientRc.bottom);
	if (!lpvBits)
	{
		DeleteDC(hDestMemDc);
		*hBitmap = NULL;
		return HRESULT_FROM_WIN32(GetLastError());
	}
	SelectObject(hDestMemDc, *hBitmap);
	HRESULT hr = S_OK;
	if (!BitBlt(
		hDestMemDc,
		0, 0,
		clientRc.right, clientRc.bottom,
		hMemDc,
		0, 0,
		SRCCOPY
	))
	{
		DeleteObject(*hBitmap);
		*hBitmap = NULL;
		hr = E_FAIL;
	}
	DeleteDC(hDestMemDc);
	return hr;
}

WindowsGDICanvas& TinyRenderer::Windows::WindowsGDICanvas::FitCanvasToWindow()
{
	RECT client;
	GetClientRect(_hWnd, &client);
	initializeBitmap(client.right, client.bottom);
	return *this;
}

WindowsGDICanvas& TinyRenderer::Windows::WindowsGDICanvas::StretchCanvasOnWindowResize(bool stretchCanvas)
{
	this->stretchCanvas = stretchCanvas;
	return *this;
}

WindowsGDICanvas& TinyRenderer::Windows::WindowsGDICanvas::SetCanvasSize(TinyRenderer::Helpers::Vector2i size, _Out_opt_ HRESULT* phResult)
{
	HRESULT result = S_OK;
	if(!initializeBitmap(size.X, size.Y))
	{
		result = __HRESULT_FROM_WIN32(GetLastError());
	}
	if(phResult)
		*phResult = result;
	return *this;
}

bool TinyRenderer::Windows::WindowsGDICanvas::Scale(int width, int heigth)
{
	throw std::runtime_error(NotImplementedMsg);
}

RgbColor TinyRenderer::Windows::WindowsGDICanvas::GetPixel(int x, int y) const
{
	if (bitMap.bmWidth < (LONG)x || bitMap.bmHeight < (LONG)y)
		return {0};
	static_assert(sizeof(RgbColor) == sizeof(RGBQUAD), "RGBQUAD size is not 4");
	RgbColor color = *(RgbColor*)&((RGBQUAD*)lpvBitmapBits)[((bitMap.bmWidth) * y) + x];
	return color;
}

bool TinyRenderer::Windows::WindowsGDICanvas::SetPixel(int x, int y, const RgbColor& color)
{
	if (bitMap.bmWidth <= (LONG)x || bitMap.bmHeight <= (LONG)y)
		return false;
	RGBQUAD* point = &((RGBQUAD*)lpvBitmapBits)[((bitMap.bmWidth) * y) + x];
	static_assert(sizeof(RGBQUAD) == 4, "RGBQUAD size is not 4");
	memcpy(point, &color.value, sizeof(int));
	return true;
}

bool TinyRenderer::Windows::WindowsGDICanvas::Fill(const RgbColor& color)
{
	static_assert(sizeof(color) == 4, "RgbColor is not 4 bytes long");
	size_t pixelArrayLen = bitMap.bmHeight * bitMap.bmWidth;
	RGBQUAD* px = static_cast<RGBQUAD*>(lpvBitmapBits);
	while (pixelArrayLen-- > 0)
		*px++ = *reinterpret_cast<const RGBQUAD*>(&color);
	return true;
}

HRESULT TinyRenderer::Windows::WindowsGDICanvas::GetBitmapBuffer(_Notnull_ _Out_ BITMAP* const phBitmap) const
{
	if (!phBitmap)
	{
		return E_INVALIDARG;
	}
	if (!GetObject(hBitmapBuffer, sizeof(BITMAP), phBitmap))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	return S_OK;
}

bool TinyRenderer::Windows::WindowsGDICanvas::filp_horizontally()
{
	throw std::logic_error(NotImplementedMsg);
}

bool TinyRenderer::Windows::WindowsGDICanvas::flip_vertically()
{
	throw std::logic_error(NotImplementedMsg);
}

ColorFormat TinyRenderer::Windows::WindowsGDICanvas::GetColorFormat() const
{
	return ColorFormat::RGB;
}

int TinyRenderer::Windows::WindowsGDICanvas::GetWidth() const
{
	RECT clientRc;
	if (!GetClientRect(_hWnd, &clientRc))
		return -1;
	return clientRc.right;
}

int TinyRenderer::Windows::WindowsGDICanvas::GetHeigth() const
{
	RECT clientRc;
	if(!GetClientRect(_hWnd, &clientRc))
		return -1;
	return clientRc.bottom;
}

void TinyRenderer::Windows::WindowsGDICanvas::SwapBuffers()
{
	RECT clientRect;
	GetClientRect(_hWnd, &clientRect);
	HDC hDc = GetDC(_hWnd);
	copyMemDcToDc(hDc);
	ReleaseDC(_hWnd, hDc);
}

TinyRenderer::Windows::WindowsGDICanvas::~WindowsGDICanvas()
{
	DeleteCriticalSection(&cs);
	SetWindowLongPtr(
		_hWnd,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(lpPrevWndProc)
	);
	SetWindowLongPtr(
		_hWnd,
		GWLP_USERDATA,
		0
	);
	DeleteObject(hBitmapBuffer);
	DeleteDC(hMemDc);
}

bool TinyRenderer::Windows::WindowsGDICanvas::initializeBitmap(int cx, int cy)
{
	HDC hDc = GetDC(_hWnd);
	if(!hMemDc)
		hMemDc = CreateCompatibleDC(hDc);
	EnterCriticalSection(&cs);
	LPVOID lpvBits;
	HBITMAP hBitmap = CreateCompatibleDIBSection(hDc, &lpvBits, cx, cy);
	if(!lpvBits || !GetObject(hBitmap, sizeof(BITMAP), &bitMap))
	{
		ReleaseDC(_hWnd, hDc);
		LeaveCriticalSection(&cs);
		return false;
	}
	SelectObject(hMemDc, hBitmap);
	InterlockedExchangePointer(&lpvBitmapBits, lpvBits);
	HBITMAP oldBitmap = hBitmapBuffer;
	InterlockedExchangePointer((void**)&hBitmapBuffer, hBitmap);
	if(oldBitmap != INVALID_HANDLE_VALUE)
		DeleteObject(oldBitmap);
	LeaveCriticalSection(&cs);
	return true;
}

void TinyRenderer::Windows::WindowsGDICanvas::copyMemDcToDc(HDC hDc)
{
	BitBlt(
		hDc,
		0,
		0,
		bitMap.bmWidth,
		bitMap.bmHeight,
		this->hMemDc,
		0, 0,
		SRCCOPY
	);
	
}