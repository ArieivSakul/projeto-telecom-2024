#include "pch.h"
#include "ICanvas.h"

void TinyRenderer::ICanvas::CleanBackBuffer()
{
	Fill(TinyRenderer::RgbColor(0));
}

