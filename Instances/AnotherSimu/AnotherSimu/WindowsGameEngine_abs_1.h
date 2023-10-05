#pragma once

// This is the Second Version of the Windows Console Game Engine. This version DOES include mouse inputs unlike WindowsGameEngine_abs_0.h...
// class "WindowsGameEngine" is abstract and must be inherited from. That is, override userCreate() and userUpdateBuffer() methods to implement instance of class...

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <algorithm>

enum COLOR
{
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007,
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
    PIXEL_SOLID = 0x2588,
    PIXEL_THREEQUARTERS = 0x2593,
    PIXEL_HALF = 0x2592,
    PIXEL_QUARTER = 0x2591,
};

class WindowsGameEngine
{
public:

    // screen dimensions
    short m_nScreenWidth;
    short m_nScreenHeight;

    // console handles
    HANDLE m_hConsole_init;
    HANDLE m_hConsole;
    HANDLE m_hConsole_in;

    // CHAR_INFO Structure (buffer)
    CHAR_INFO* m_bufScreen;
    DWORD m_dwBytesWritten;

    // physical console window size and screen buffer size
    SMALL_RECT m_rectWindow;
    COORD m_coord;

    // Async Keystate arrays
    short m_oldKeyState[256];
    short m_newKeyState[256];

    // Mouse states arrays
    bool m_oldMouseState[5];
    bool m_newMouseState[5];

    // mouse positions
    int m_mouseX;
    int m_mouseY;

    // Keystate Info structures
    struct keyState {
        bool bPressed = false;
        bool bReleased = false;
        bool bHeld = false;
    };

    keyState m_keyStates[256];
    keyState m_mouseStates[5];

    // thread flag
    bool m_activeTh = false;

    // console input focus
    bool m_bConsoleInFocus = true;

    WindowsGameEngine() {

        m_nScreenWidth = 800;
        m_nScreenHeight = 600;

        m_hConsole_init = GetStdHandle(STD_OUTPUT_HANDLE);
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        m_hConsole_in = GetStdHandle(STD_INPUT_HANDLE);

        m_bufScreen = nullptr;
        m_dwBytesWritten = 0;

        m_rectWindow = { 0,0,1,1 };
        m_coord = { m_nScreenWidth, m_nScreenHeight };

        m_mouseX = m_nScreenWidth / 2;
        m_mouseY = m_nScreenHeight / 2;

        std::memset(m_oldKeyState, 0, 256 * sizeof(short));
        std::memset(m_newKeyState, 0, 256 * sizeof(short));

        std::memset(m_oldMouseState, 0, 5 * sizeof(bool));
        std::memset(m_newMouseState, 0, 5 * sizeof(bool));

        std::memset(m_keyStates, 0, 256 * sizeof(keyState));
        std::memset(m_keyStates, 0, 5 * sizeof(keyState));

    }

    int constructConsole(int width, int height, int fontW = 1, int fontH = 1) {

        m_nScreenWidth = width;
        m_nScreenHeight = height;

        SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);
        SetConsoleScreenBufferSize(m_hConsole, m_coord);
        SetConsoleActiveScreenBuffer(m_hConsole);

        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = fontW;
        cfi.dwFontSize.Y = fontH;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;

        SetCurrentConsoleFontEx(m_hConsole, false, &cfi);

        m_rectWindow = { 0,0, (short)(m_nScreenWidth - 1), (short)(m_nScreenHeight - 1) };
        SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

        SetConsoleMode(m_hConsole_in, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

        m_bufScreen = new CHAR_INFO[m_nScreenWidth * m_nScreenHeight];

        return 1;

    }

    // inherited class overrides these functions
    virtual bool userCreate() = 0;
    virtual bool userUpdateBuffer(float f_elapsed_t) = 0;

    void writeBuffer() {

        WriteConsoleOutput(m_hConsole, m_bufScreen, { (short)m_nScreenWidth, (short)m_nScreenHeight }, { 0,0 }, &m_rectWindow);

    }

    // ==================================================== Drawing algorithms ===================================================================== //


    void Draw(int x, int y, short col = 0x000F, short c = 0x2588) {

        if (x >= 0 && x < m_nScreenWidth && y >= 0 && y < m_nScreenHeight)
        {
            m_bufScreen[(m_nScreenHeight - y - 1) * m_nScreenWidth + x].Char.UnicodeChar = c;
            m_bufScreen[(m_nScreenHeight - y - 1) * m_nScreenWidth + x].Attributes = col;
        }
    }

    void Fill(int x1, int y1, int x2, int y2, short col = 0x000F, short c = 0x2588) {

        for (int x = x1; x < x2; ++x)
            for (int y = y1; y < y2; ++y)
                Draw(x, y, col, c);

    }
    void FillScreen(short col = 0, short c = 0x2588) {

        for (int x = 0; x < m_nScreenWidth; ++x)
            for (int y = 0; y < m_nScreenHeight; ++y)
                Draw(x, y, col, c);
    }

    void RandomFill() {

        for (int x = 0; x < m_nScreenWidth; ++x)
            for (int y = 0; y < m_nScreenHeight; ++y)
                Draw(x, y, rand() % 16);
    }

    void DrawString(int x, int y, short col, std::wstring c) {

        for (size_t i = 0; i < c.size(); ++i) {

            m_bufScreen[(m_nScreenHeight - y - 1) * m_nScreenWidth + i].Char.UnicodeChar = c[i];
            m_bufScreen[(m_nScreenHeight - y - 1) * m_nScreenWidth + i].Attributes = col;
        }
    }

    void DrawLine(float x1, float y1, float x2, float y2, short col = 0x000F, short c = 0x2588, bool line = false, float x_i = 0, float y_i = 0) {

        float x, y, dx, dy, step;
        short i = 1;

        dx = x2 - x1;
        dy = y2 - y1;

        if (abs(dx) >= abs(dy))
            step = abs(dx);
        else
            step = abs(dy);

        dx = dx / step;
        dy = dy / step;

        x = x1;
        y = y1;

        while (i <= step) {
            Draw(x, y, col, c);
            if (line)
                DrawLine(x_i, y_i, x, y, col, c);
            x += dx;
            y += dy;
            i += 1;
        }
    }

    void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, short col = 0x000F, short c = 0x2588) {

        DrawLine(x1, y1, x2, y2, col, c);
        DrawLine(x2, y2, x3, y3, col, c);
        DrawLine(x3, y3, x1, y1, col, c);
    }

    void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool gradient = false, short col = 0x000F, short c = 0x2588) {

        if (!gradient) {

            DrawLine(x1, y1, x2, y2, col, c, true, x3, y3);
            DrawLine(x2, y2, x3, y3, col, c, true, x1, y1);
            DrawLine(x3, y3, x1, y1, col, c, true, x2, y2);
        }

        else {

            DrawLine(x1, y1, x2, y2, col, c);
            DrawLine(x1, y1, x3, y3, col, c);

            float x_i, y_i, x_0, y_0, dx_0, dy_0, step_0, dx, dy, step;
            short i = 1, j = 1;

            dx = x3 - x2;
            dy = y3 - y2;

            dx_0 = x1 - x2;
            dy_0 = y1 - y2;

            if (abs(dx) >= abs(dy))
                step = abs(dx);
            else
                step = abs(dy);

            if (abs(dx_0) >= abs(dy_0))
                step_0 = abs(dx_0);
            else
                step_0 = abs(dy_0);

            step = !(step < step_0) ? step_0 : step;

            dx = dx / step;
            dy = dy / step;

            dx_0 = dx_0 / step;
            dy_0 = dy_0 / step;

            x_i = x2;
            x_0 = x2;
            y_i = y2;
            y_0 = y2;

            while (i <= step) {

                Draw(x_i, y_i, col, c);
                DrawLine(x_0, y_0, x_i, y_i, col, c);

                x_i += dx;
                y_i += dy;
                x_0 += dx_0;
                y_0 += dy_0;
                i += 1;
            }
        }
    }

    void DrawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, short col = 0x000F, short c = 0x2588) {

        DrawLine(x1, y1, x2, y2, col, c);
        DrawLine(x2, y2, x3, y3, col, c);
        DrawLine(x3, y3, x4, y4, col, c);
        DrawLine(x4, y4, x1, y1, col, c);
    }

    void DrawCircle(float cx, float cy, float r, short col = 0x000F, short c = 0x2588) {

        int x = 0, y = r;
        int p = 3 - 2 * r;
        if (!r) return;

        while (y >= x) {

            Draw(cx - x, cy - y, col, c);
            Draw(cx - y, cy - x, col, c);
            Draw(cx + y, cy - x, col, c);
            Draw(cx + x, cy - y, col, c);
            Draw(cx - x, cy + y, col, c);
            Draw(cx - y, cy + x, col, c);
            Draw(cx + y, cy + x, col, c);
            Draw(cx + x, cy + y, col, c);

            if (p < 0) p += 4 * x++ + 6;
            else p += 4 * (x++ - y--) + 10;
        }
    }

    void FillCircle(float cx, float cy, float r, bool gradient = false, short col = 0x000F, short c = 0x2588) {

        int x = 0, y = r;
        int p = 3 - 2 * r;
        if (!r) return;

        while (y >= x) {

            auto fillLine = [&](int sx, int ex, int ny) {

                for (int i = sx; i <= ex; ++i)
                    Draw(i, ny, col, c);
            };

            if (gradient) {

                DrawLine(cx, cy, cx - x, cy - y, col, c);
                DrawLine(cx, cy, cx - y, cy - x, col, c);
                DrawLine(cx, cy, cx + y, cy - x, col, c);
                DrawLine(cx, cy, cx + x, cy - y, col, c);
                DrawLine(cx, cy, cx - x, cy + y, col, c);
                DrawLine(cx, cy, cx - y, cy + x, col, c);
                DrawLine(cx, cy, cx + y, cy + x, col, c);
                DrawLine(cx, cy, cx + x, cy + y, col, c);

            }

            else {

                fillLine(cx - x, cx + x, cy - y);
                fillLine(cx - y, cx + y, cy - x);
                fillLine(cx - x, cx + x, cy + y);
                fillLine(cx - y, cx + y, cy + x);

            }

            if (p < 0) p += 4 * x++ + 6;
            else p += 4 * (x++ - y--) + 10;
        }
    }


    // ============================================================================================================================================ //

    void sleep(int t = 1000) {
        std::this_thread::sleep_for(std::chrono::milliseconds(t));
    }

    ~WindowsGameEngine() {

        SetConsoleActiveScreenBuffer(m_hConsole_init);
        delete[] m_bufScreen;
    }

    void gameStart() {

        // begin thread
        m_activeTh = true;
        std::thread gThread = std::thread(&WindowsGameEngine::gameThread, this);

        // wait for thread to finish executing
        gThread.join();

    }

    void gameEnd() { m_activeTh = false; }

    void gameThread() {

        // time instances
        auto t1 = std::chrono::system_clock::now();
        auto t2 = std::chrono::system_clock::now();
        float f_elapsed_t = 0;

        while (m_activeTh) {

            for (int i = 0; i < 256; ++i)
            {

                t2 = std::chrono::system_clock::now();
                std::chrono::duration<float> delta_t = t2 - t1;
                t1 = t2;
                f_elapsed_t = delta_t.count();

                m_newKeyState[i] = GetAsyncKeyState(i);

                m_keyStates[i].bPressed = false;
                m_keyStates[i].bReleased = false;

                if (m_newKeyState[i] != m_oldKeyState[i])
                {
                    if (m_newKeyState[i] & 0x8000) // (high bit) -- key is currently pressed down
                    {
                        m_keyStates[i].bPressed = !m_keyStates[i].bHeld;
                        m_keyStates[i].bHeld = true;
                    }

                    else // 0x0001 (low bit) -- key just transitioned from released to pressed
                    {
                        m_keyStates[i].bReleased = true;
                        m_keyStates[i].bHeld = false;
                    }
                }

                m_oldKeyState[i] = m_newKeyState[i];
            }

            INPUT_RECORD inputBuffer[32];
            DWORD events = 0;
            GetNumberOfConsoleInputEvents(m_hConsole_in, &events);
            if (events > 0)
                ReadConsoleInput(m_hConsole_in, inputBuffer, events, &events);

            for (DWORD i = 0; i < events; ++i) {

                switch (inputBuffer[i].EventType)
                {
                
                case FOCUS_EVENT:
                {
                    // per Windows: internal event, should be ignored
                    m_bConsoleInFocus = inputBuffer[i].Event.FocusEvent.bSetFocus;
                }
                break;

                case MOUSE_EVENT:
                {
                    
                    switch (inputBuffer[i].Event.MouseEvent.dwEventFlags)
                    {
                    
                    case MOUSE_MOVED:
                    {
                        m_mouseX = inputBuffer[i].Event.MouseEvent.dwMousePosition.X;
                        m_mouseY = m_nScreenHeight - inputBuffer[i].Event.MouseEvent.dwMousePosition.Y - 1;
                    }
                    break;

                    case 0:
                    {
                        for (int m = 0; m < 5; ++m)
                            m_newMouseState[m] = (inputBuffer[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
                    }
                    break;

                    default:
                        break;
                    }
                }
                break;


                default:
                    break;
                }
                
            }

            for (int m = 0; m < 5; ++m) {

                m_mouseStates[m].bPressed = false;
                m_mouseStates[m].bReleased = false;

                if (m_newMouseState[m] != m_oldMouseState[m]) {

                    if (m_newMouseState[m])
                    {
                        m_mouseStates[m].bPressed = true;
                        m_mouseStates[m].bHeld = true;

                    }
                    else
                    {
                        m_mouseStates[m].bReleased = true;
                        m_mouseStates[m].bHeld = false;
                    }
                }
                
                m_oldMouseState[m] = m_newMouseState[m];                
            }


            userUpdateBuffer(f_elapsed_t);
            writeBuffer();
        }
    }
};