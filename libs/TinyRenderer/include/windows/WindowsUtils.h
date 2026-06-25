#pragma once

#include "pch.h"

_Success_(return != (HBITMAP)HGDI_ERROR)
HBITMAP CreateCompatibleDIBSection(HDC hDc, _Outptr_ LPVOID* lppvBits, int cx, int cy);

