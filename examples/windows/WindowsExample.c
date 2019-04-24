// Copyright 2019 Dietrich Epp.
// This file is licensed under the terms of the MIT license. See LICENSE.txt
// for details.
#include "keytable.h"

#define WIN32_LEAN_AND_MEAN

#include <SDKDDKVer.h>
#include <strsafe.h>
#include <windows.h>

static const wchar_t kClassName[] = L"exampleWindowClass";

// Text to display on screen.
static wchar_t gLine1[64], gLine2[64];

// Whether the alt buttons are pressed.
static int gAlt[2];

static void die(const wchar_t *msg) {
    MessageBoxW(NULL, msg, L"Error", MB_ICONEXCLAMATION | MB_OK);
    ExitProcess(1);
}

static void handle_key(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    unsigned keyCode = keycode_windows_from_lparam(lParam);
    const char *keyName = keycode_windows_rawname(keyCode);
    if (keyName == NULL) {
        keyName = "unknown";
    }
    StringCbPrintfW(gLine1, sizeof(gLine1), L"Win Keycode %d (%S)", keyCode,
                    keyName);
    unsigned hidCode = keycode_windows_to_hid(keyCode);
    const char *hidName = keycode_windows_name(hidCode);
    if (hidName == NULL) {
        hidName = "unknown";
    }
    StringCbPrintfW(gLine2, sizeof(gLine2), L"HID Keycode %d (%S)", hidCode,
                    hidName);
    InvalidateRect(hwnd, NULL, TRUE);
}

static void paint_window(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    TextOutW(hdc, 8, 8, gLine1, wcslen(gLine1));
    TextOutW(hdc, 8, 24, gLine2, wcslen(gLine2));
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        handle_key(hwnd, msg, wParam, lParam);
        break;
    case WM_SYSKEYDOWN:
        if (wParam == VK_F4 && GetKeyState(VK_MENU)) {
            ExitProcess(0);
        }
        handle_key(hwnd, msg, wParam, lParam);
        break;
    case WM_PAINT:
        paint_window(hwnd);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

static HWND create_window(HINSTANCE hInstance) {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = 0;
    wc.lpfnWndProc = wnd_proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = kClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&wc)) {
        die(L"Could not register window class.");
    }
    HWND hwnd = CreateWindowExW(
        WS_EX_CLIENTEDGE, kClassName, L"Keycode Example", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, 100, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) {
        die(L"Could not create window.");
    }
    return hwnd;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    HWND hwnd = create_window(hInstance);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (1) {
        MSG msg;
        if (!GetMessageW(&msg, NULL, 0, 0)) {
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
