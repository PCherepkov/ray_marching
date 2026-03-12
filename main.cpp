#include <stdio.h>
#include <stdlib.h>
// #include <windows.h>

#include "RND/frame.h"
#include "RND/render.h"
#include "main.h"

anim ani;


void SetWindow(int w, int h, bool is_full_screen) {
    if (ani.window != nullptr)
        glfwDestroyWindow(ani.window);

    if (is_full_screen)
        ani.window = glfwCreateWindow(w, h, "SDF", glfwGetPrimaryMonitor(), NULL);
    else
        ani.window = glfwCreateWindow(w, h, "SDF", NULL, NULL);

    if (ani.window == nullptr)
        return;

    glfwMakeContextCurrent(ani.window);
    glfwSetFramebufferSizeCallback(ani.window, Reshape);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return;

    glViewport(0, 0, w, h);

    ani.w = w, ani.h = h;
    glfwGetCursorPos(ani.window, &ani.x, &ani.y);
    renderInit(ani.window);
}


void Reshape(GLFWwindow* window, int w, int h) {
    ani.w = w;
    ani.h = h;
    glViewport(0, 0, w, h);
}


int main() {
    int k = glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SetWindow(1920, 1080, false);

    glClearColor(0.3f, 0.47f, 0.8f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);

    while (!ani.exit) {
        ani.processInput();
        render(ani.window);
        glfwSwapBuffers(ani.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

#if 0
#define MAIN_WINDOW_CLASS "Main Window"

frame Frame(0, 0);
bool IsFull = FALSE;

LRESULT CALLBACK WinFunc(HWND hWnd, UINT Msg,
                         WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpfnWndProc = WinFunc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = (LPCWSTR)MAIN_WINDOW_CLASS;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindow((LPCWSTR)MAIN_WINDOW_CLASS,
        L"Ray marching",
        WS_OVERLAPPEDWINDOW,
        0, 0, 320, 320,
        NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        MessageBoxA(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}


LRESULT CALLBACK WinFunc(HWND hWnd, UINT Msg,
    WPARAM wParam, LPARAM lParam)
{
    INT w = 0, h = 0;
    HDC hDC;
    CREATESTRUCT* cs;
    PAINTSTRUCT ps;
    BITMAPINFOHEADER bmih;
    static BITMAPINFO bmi;
    BOOL Redraw = true;

    w = LOWORD(lParam);
    h = HIWORD(lParam);

    switch (Msg)
    {
    case WM_CREATE:
        cs = (CREATESTRUCT*)lParam;
        SetWindowLong(hWnd, 0, (long)&Frame);
        SetTimer(hWnd, 30, 100, NULL);
        return 0;
    case WM_TIMER:
        if (!Redraw)
            return 0;
        Redraw = false;
        //Frame.Render();
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_SPACE)
            IsFull = !IsFull;
    case WM_SIZE:
        Redraw = true;
        w = LOWORD(lParam);
        h = HIWORD(lParam);

        /* Free memory from old data */
        if (Frame.hDib != NULL)
            DeleteObject(Frame.hDib);

        /* Allocate memory for new background size */
        bmih.biSize = sizeof(BITMAPINFOHEADER);
        bmih.biWidth = w;
        bmih.biHeight = -h;
        bmih.biBitCount = 32;
        bmih.biPlanes = 1;
        bmih.biCompression = 0;
        bmih.biSizeImage = w * h * 4;
        bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 1;
        bmih.biClrUsed = 0;
        bmih.biClrImportant = 0;

        bmi.bmiHeader = bmih;

        Frame.hDib = CreateDIBSection(NULL, (BITMAPINFO*)&bmih,
            DIB_RGB_COLORS, (void**)&Frame.Image, NULL, 0);
        if (Frame.hDib != NULL)
            Frame.W = w, Frame.H = h;
        else
            Frame.W = Frame.H = 0;
        Frame.Image = (DWORD*)malloc(sizeof(DWORD*) * w * h);
        Frame.Cam.SetProj(w, h).SetView(vec3(0, 2, -5),
                                        vec3(0, 1, 0),
                                        vec3(0, 1, 0));
        return 0;
    case WM_MOUSEMOVE:
        return 0;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        Frame.Render();
        // Frame.PutPixel(30, 30, color(0, 255, 0, 255));
        StretchDIBits(hDC, 0, 0, Frame.W, Frame.H, 0, 0, Frame.W, Frame.H, Frame.Image, &bmi, DIB_RGB_COLORS, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
#endif