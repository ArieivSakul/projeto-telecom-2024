#include "pch.h"
#include "windows/WindowsUtils.h"

_Success_(return != (HBITMAP)HGDI_ERROR)
HBITMAP CreateCompatibleDIBSection(HDC hDc, _Outptr_ LPVOID* lppvBits, int cx, int cy)
{
    if (!lppvBits)
        return (HBITMAP)HGDI_ERROR;
    *lppvBits = NULL;

    WORD wDcPlanes;
    WORD wDcBitCount;

    if (cx <= 0 || cy <= 0)
        return (HBITMAP)HGDI_ERROR;
    
    wDcPlanes = (WORD)GetDeviceCaps(hDc, PLANES);
    wDcBitCount = (WORD)GetDeviceCaps(hDc, BITSPIXEL);

    BITMAPINFOHEADER bmiHeader
    {
        .biSize = sizeof(BITMAPINFOHEADER),
        .biWidth = cx,
        .biHeight = cy,
        .biPlanes = wDcPlanes,
        .biBitCount = wDcBitCount,
        .biCompression = BI_RGB,
        .biSizeImage = 0,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = wDcBitCount < 16u ? wDcBitCount : 0u,
        .biClrImportant = 0
    };

    return CreateDIBSection(
        hDc,
        reinterpret_cast<const BITMAPINFO*>(&bmiHeader),
        DIB_RGB_COLORS,
        lppvBits,
        NULL,
        0
    );
    
}
