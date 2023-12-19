#include "core/assertion.h"
#include "nkpch.h"

#include "platform/platform_win32.h"

namespace nk {
    f64 PlatformWin32::s_clock_frequency = 0;
    LARGE_INTEGER PlatformWin32::s_start_time = {};

    // Defined at end of the file
    LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam);

    PlatformWin32::PlatformWin32(str application_name, const u32 x, const u32 y, const u32 width, const u32 height)
        : Platform(application_name, x, y, width, height) {
        m_hinstance = GetModuleHandleA(0);

        // Setup and register window class
        HICON icon = LoadIcon(m_hinstance, IDI_APPLICATION);
        WNDCLASSA wc;
        memset(&wc, 0, sizeof(wc));
        wc.style = CS_DBLCLKS; // Get double clicks
        wc.lpfnWndProc = win32_process_message;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_hinstance;
        wc.hIcon = icon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Manage the cursor manually
        wc.hbrBackground = NULL; // Transparent
        wc.lpszClassName = "nk_window_class";

        if (!RegisterClassA(&wc)) {
            MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            AssertMsg(false, "Window registration failed");
            return;
        }

        // Create window
        u32 client_x = x;
        u32 client_y = y;
        u32 client_width = width;
        u32 client_height = height;

        u32 window_x = client_x;
        u32 window_y = client_y;
        u32 window_width = client_width;
        u32 window_height = client_height;

        u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        u32 window_ex_style = WS_EX_APPWINDOW;

        window_style |= WS_MAXIMIZEBOX;
        window_style |= WS_MINIMIZEBOX;
        window_style |= WS_THICKFRAME;

        // Obtain the size of the border
        RECT border_rect = {0, 0, 0, 0};
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

        // In this case, the border rectangle is negative
        window_x += border_rect.left;
        window_y += border_rect.top;

        // Grow by the size of the OS border
        window_width += border_rect.right - border_rect.left;
        window_height += border_rect.bottom - border_rect.top;

        HWND handle = CreateWindowExA(
            window_ex_style,
            "nk_window_class",
            application_name.c_str(),
            window_style,
            window_x,
            window_y,
            window_width,
            window_height,
            0, 0, m_hinstance, 0
        );

        if (handle == 0) {
            MessageBoxA(NULL, "Window creation failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
            AssertMsg(false, "Window creation failed!");
            return;
        }
        m_hwnd = handle;

        bool should_activate = true; // TODO: if the window should not accept input, this should be false.
        i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
        // TODO:
        // If initally minimized, use WS_MINIMIZE : WS_SHOWMINOACTIVATE
        // If initilly maxsimized, use SW_SHOWMAXIMIZED : WS_MAXIMIZE
        ShowWindow(m_hwnd, show_window_command_flags);

        // Clock setup
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        s_clock_frequency = 1.0 / static_cast<f64>(frequency.QuadPart);
        QueryPerformanceCounter(&s_start_time);
    }

    PlatformWin32::~PlatformWin32() {
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
        }
    }

    bool PlatformWin32::pump_messages() {
        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        return true;
    }

    f64 PlatformWin32::get_absolute_time() {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);
        return static_cast<f64>(current_time.QuadPart) * s_clock_frequency;
    }

    void PlatformWin32::sleep(u64 ms) {
        Sleep(ms);
    }

    LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam) {
        switch (msg) {
            case WM_ERASEBKGND:
                // Notify the OS that erasing will be handled by the application to prevent flicker.
                return 1;
            case WM_CLOSE:
                // TODO: Fire an event fot the application to quit.
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            case WM_SIZE: {
                // RECT rect;
                // GetClientRect(hwnd, &rect);
                // u32 width = rect.right - rect.left;
                // u32 height = rect.bottom - rect.top;
                // TODO: Fire an event to window resize.
            } break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                // Key pressed/released
                // bool pressed = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
                // TODO: Input processing.
            } break;
            case WM_MOUSEMOVE: {
                // i32 x_position = GET_X_LPARAM(lparam);
                // i32 y_position = GET_Y_LPARAM(lparam);
                // TODO: Mouse processing.
            } break;
            case WM_MOUSEWHEEL: {
                // f32 z_delta = GET_WHEEL_DELTA_WPARAM(wparam);
                // if (z_delta != 0) {
                //     // Flatten the input to an OS-independent (-1, 1)
                //     z_delta /= 120.0f;
                //     i8 z_delta_normalized = z_delta < 0 ? -1 : 1;
                //     // TODO: Mouse Wheel processing.
                // }
            } break;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                // bool pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_LBUTTONDOWN;
                // TODO: Mouse button processing.
            } break;
        }

        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
}
