// Copyright 2019 Dietrich Epp.
// This file is licensed under the terms of the MIT license. See LICENSE.txt
// for details.
#include "keycode.h"
#include "keyid.h"
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

static void show_key(HWND hwnd, WPARAM wParam, LPARAM lParam, int hidCode) {
    StringCbPrintfW(gLine1, sizeof(gLine1), L"wParam: 0x%x, lParam: 0x%x",
                    wParam, lParam);
    const char *name = keyid_name_from_code(hidCode);
    if (name == NULL) {
        name = "unknown";
    }
    StringCbPrintfW(gLine2, sizeof(gLine2), L"HID Code: %d (%S)", hidCode,
                    name);
    InvalidateRect(hwnd, NULL, TRUE);
}

static void handle_key_down(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    int hidCode = 255;
    if (0 <= wParam && wParam < 256) {
        switch (wParam) {
        case VK_SHIFT: {
            unsigned key =
                MapVirtualKeyW((lParam >> 16) & 0xff, MAPVK_VSC_TO_VK_EX);
            hidCode = key == VK_LSHIFT ? KEY_LeftShift : KEY_RightShift;
        } break;
        case VK_CONTROL:
            hidCode =
                (lParam & 0x01000000) != 0 ? KEY_RightControl : KEY_LeftControl;
            break;
        default:
            hidCode = WIN_NATIVE_TO_HID[wParam];
            break;
        }
    }
    show_key(hwnd, wParam, lParam, hidCode);
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
        handle_key_down(hwnd, wParam, lParam);
        break;
    case WM_SYSKEYDOWN:
        if (wParam == VK_MENU) {
            int hidCode;
            if ((lParam & 0x01000000) != 0) {
                hidCode = KEY_RightAlt;
                gAlt[1] = 1;
            } else {
                hidCode = KEY_LeftAlt;
                gAlt[0] = 1;
            }
            show_key(hwnd, wParam, lParam, hidCode);
        } else if (wParam == VK_F4 && (gAlt[0] || gAlt[1])) {
            ExitProcess(0);
        } else {
            handle_key_down(hwnd, wParam, lParam);
        }
        break;
    case WM_SYSKEYUP:
        if (wParam == VK_MENU) {
            if ((lParam & 0x01000000) != 0) {
                gAlt[1] = 0;
            } else {
                gAlt[0] = 0;
            }
        }
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
