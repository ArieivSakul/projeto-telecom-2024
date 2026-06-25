//Dupla: Lucas Alves Vieira - 832799 / Maria Luiza Barrionovo - 832216

#include "pch.h"

TCHAR szWndClassName[] = TEXT("fft_visualizer_class");
TCHAR szWndName[] = TEXT("FFT Visualizer - Espectro de Frequencia");
HINSTANCE _hInst;

#define WM_UPDATE_CANVAS WM_USER + 0x1

std::unique_ptr<TinyRenderer::Windows::WindowsGDICanvas> _canvas;
utils::ComplexArray cArray; //Matriz de pontos que definem o resultado da opera��o de fourier
TinyRenderer::Helpers::Vector2i objPosOffset = {0, 0};
TinyRenderer::Helpers::Vector2i oldMousePos = {0, 0};
DWORD frameTime = 0;
double objSpectRatio = 1.;
int objScale = 0;

ATOM MyRegisterWindow(HINSTANCE hInstance);
HRESULT initializeInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT __stdcall WindowProcedure(HWND hWmd, UINT uMsg, WPARAM wParamm, LPARAM lParam);
void DrawCanvas(TinyRenderer::TinyRenderer& renderer);
void DrawStats(HWND hWnd); // TODO: Remove this
void GenerateSinePoints(utils::ComplexArray& array); //gera o sinal de frequencia

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmd,
    int iShowCmd
)
{
    if(!MyRegisterWindow(hInstance))
    {
        return -1;
    }

    if(FAILED(initializeInstance(hInstance, iShowCmd)))
    {
        return -1;
    }

    auto pCanvas = _canvas.get();
    pCanvas->FitCanvasToWindow();
    pCanvas->StretchCanvasOnWindowResize(true);
    // Initialize renderer with a windows GDI canvas
    TinyRenderer::TinyRenderer renderer(*pCanvas);

    objScale = pCanvas->GetWidth() / 2;

    // Initializes cArray with a FFT of an 2500 Hz sine wave
    GenerateSinePoints(cArray);
    utils::CalcFFT(cArray);

    MSG msg;
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        if(msg.message == WM_UPDATE_CANVAS)
        {
            pCanvas->Fill({0x4682B4});
            DrawCanvas(renderer);
            pCanvas->SwapBuffers();
            DrawStats(msg.hwnd);
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}

ATOM MyRegisterWindow(HINSTANCE hIns)
{
    WNDCLASSEX wcex {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProcedure,
        .hInstance = hIns,
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .lpszClassName = szWndClassName
    };
    return RegisterClassEx(&wcex);
}

HRESULT initializeInstance(HINSTANCE hInstance, int nCmdShow)
{
    _hInst = hInstance;
    HWND hWnd = CreateWindow(
        szWndClassName,
        szWndName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0,
        nullptr, nullptr,
        hInstance,
        nullptr
    );
    if(!hWnd)
    {
        return E_FAIL;
    }


    TinyRenderer::Helpers::Vector2i size(-1, -1);
    _canvas = std::make_unique<TinyRenderer::Windows::WindowsGDICanvas>(hWnd, size);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return S_OK;
}

//fun��o chamada toda vez que houver uma atualiza��o na janela
LRESULT __stdcall WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        HMENU hMenuBar = CreateMenu();
        SetMenu(hWnd, hMenuBar);
        HMENU hMiFile = CreatePopupMenu();
        HMENU hMiOption = CreatePopupMenu();
        //AppendMenu(hMenuBar, MF_STRING | MF_DEFAULT, (UINT_PTR)hMiFile, "&File");

        MENUITEMINFOW mii {
            .cbSize = sizeof(MENUITEMINFOW)
        };
        mii.fMask = MIIM_STRING | MIIM_ID;
        mii.fType = MFT_STRING;

        mii.wID = 1;
        mii.dwTypeData = (LPWSTR)L"&Exit";
        InsertMenuItemW(hMiFile, 1, TRUE, &mii);
        mii.wID = 2;
        mii.dwTypeData = (LPWSTR)L"&Open";
        InsertMenuItemW(hMiFile, 2, TRUE, &mii);
        mii.wID = 3;
        mii.dwTypeData = (LPWSTR)L"&Reset";
        InsertMenuItemW(hMiOption, 2, TRUE, &mii);

        mii.fMask = MIIM_STRING | MIIM_SUBMENU;
        mii.fType = MFT_STRING;
        mii.hSubMenu = hMiFile;

        mii.dwTypeData = (LPWSTR)L"&File";
        InsertMenuItemW(
            hMenuBar,
            GetMenuItemCount(hMenuBar),
            TRUE,
            &mii
        );
        mii.hSubMenu = hMiOption;
        mii.dwTypeData = (LPWSTR)L"&Options";
        InsertMenuItemW(
            hMenuBar,
            GetMenuItemCount(hMenuBar),
            TRUE,
            &mii
        );

        SetTimer(hWnd, 0, USER_TIMER_MINIMUM, NULL);
        DrawMenuBar(hWnd);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case 1:
            if(MessageBox(
                hWnd,
                TEXT("Are you sure you want to exit?"),
                TEXT("Are you sure?"),
                MB_YESNO
            ) == IDYES)
            {
                PostQuitMessage(0);
            }
        break;
        case 2:
        {
            TCHAR szFilePathBuff[MAX_PATH]{0};
            OPENFILENAME ofn {
                .lStructSize = sizeof(OPENFILENAME),
                .hwndOwner = hWnd,
                .hInstance = _hInst,
                .lpstrFilter = TEXT(".DAT\0\0"),
                .nMaxCustFilter = 40,
                .nFilterIndex = 0,
                .lpstrFile = szFilePathBuff,
                .nMaxFile = MAX_PATH,
                .lpstrTitle = TEXT("Open dat faile"),
                .Flags = OFN_ENABLESIZING | OFN_EXPLORER,
                .lpstrDefExt = TEXT(".DAT")
            };
            if(!GetOpenFileName(&ofn))
                break;
            cArray = utils::ParseDatFile(szFilePathBuff);
            if(cArray.size() <= 0)
            {
                MessageBox(
                    hWnd,
                    TEXT("Something went wrong!"),
                    TEXT("Parse dat file error"),
                    MB_ICONERROR | MB_OK
                );
            }
            utils::CalcFFT(cArray);
        }
        break;
        case 3:
            objScale = 100;
            objPosOffset = {0, 0};
            break;
        }
    }
    break;
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
        oldMousePos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        break;
    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON)
        {
            int mX = (int)GET_X_LPARAM(lParam);
            int mY = (int)GET_Y_LPARAM(lParam);
            objPosOffset = {
                (int)(0, objPosOffset.X + (mX - oldMousePos.X) * 0.30),
                (int)(0, objPosOffset.Y + (oldMousePos.Y - mY) * 0.30)
            };
            oldMousePos = { mX, mY };
        }
        break;
    case WM_MOUSEWHEEL:
    {
        int delta = (int)(GET_WHEEL_DELTA_WPARAM(wParam));
        if(GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL)
            objSpectRatio += (double)delta * 0.0001;
        else
            objScale += (int)(delta * 0.1);
    }
    break;
    case WM_TIMER:
    {
        PostMessage(hWnd, WM_UPDATE_CANVAS, 0, 0);
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

//rotina para desenho
void DrawCanvas(TinyRenderer::TinyRenderer& renderer)
{
    auto size = cArray.size() / 2;
    if(size <= 0)
        return;

    auto maxP = *std::max_element(std::begin(cArray), std::end(cArray),
        [](const utils::Complex& a, const utils::Complex& b) {
            return a.real() < b.real();
        }
    );
    const double maxY = std::real(std::sqrt(maxP * maxP));
    const double maxX = (double)size;

    auto normalizePoint = [maxY, maxX](TinyRenderer::Helpers::Vector2d& point){
        point.X = point.X / maxX;
        point.Y = point.Y / maxY;
    };

    const int scaleX = objScale;
    const int scaleY = (int)(objScale / objSpectRatio);

    TinyRenderer::ICanvas& canvas = renderer.GetCanvas();

    int x0 = 0, y0 = 0;
    while(size-- > 0)
    {
        double amplitude = std::real(std::sqrt(cArray[size] * cArray[size])); //tamanho maximo do sinal

        TinyRenderer::Helpers::Vector2d point((double)size, amplitude);
        normalizePoint(point);
        if(!std::signbit(point.X))
            point.X *= -1;

        int x = (int)std::max(0., (point.X + 1.) * scaleX + objPosOffset.X);
        int y = (int)std::max(0., (point.Y + 1.) * scaleY + objPosOffset.Y);
        TinyRenderer::RgbColor cWhite(0x00FFFFFF);
        renderer.DrawLine(x0, y0, x, y, cWhite);
        TinyRenderer::RgbColor cRed(0x00FF0000);
        canvas.SetPixel(x0, y0, cRed);
        y0 = y;
        x0 = x;
    }
}

void DrawStats(HWND hWnd)
{

    DWORD tick = GetTickCount();
    int ft = (tick - frameTime);
    double fps = (1000 / (double)ft);
    frameTime = tick;

    std::ostringstream oss;
    oss.precision(2);

    oss << "FPS: " << std::fixed << fps << " | Frametime: " << ft;
    auto fpsStr = oss.str();
    oss.str("");

    oss << "Offset: { X:" << objPosOffset.X << " Y:" << objPosOffset.Y << " }"
    << " | SpectRatio: " << std::fixed << objSpectRatio;
    auto infoStr = oss.str();

    HDC hDc = GetDC(hWnd);
    SetTextColor(hDc, RGB(0xFF, 0, 0));
    TextOut(hDc, 0, 0, fpsStr.c_str(), (int)fpsStr.size());
    TextOut(hDc, 0, 20, infoStr.c_str(), (int)infoStr.size());
    ReleaseDC(hWnd, hDc);
}
//fun��o que gera a frequencia
void GenerateSinePoints(utils::ComplexArray& array)
{
    array.resize(10000);

    const int freq = 2500;
    const double inc = 1. / array.size();

    double t = 0;
    for(size_t i = 0; i < array.size(); i++)
    {
        array[i] = utils::Complex(std::sin(2 * M_PI * freq * t));
        t += inc;
    }
}
