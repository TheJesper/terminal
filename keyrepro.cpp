// Repro for microsoft/terminal#18120 - prints KEY_EVENTs with ENHANCED_KEY state.
// Also appends every line to keyrepro.log next to the exe for evidence capture.
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>

int main(int argc, char** argv)
{
    std::ofstream log(argc > 1 ? argv[1] : "keyrepro.log", std::ios::app);
    INPUT_RECORD rec;
    DWORD count;
    HANDLE in = ::GetStdHandle(STD_INPUT_HANDLE);
    std::cout << "Press keys (Esc to quit)...\n";
    while (true)
    {
        ::ReadConsoleInputW(in, &rec, 1, &count);
        if (rec.EventType == KEY_EVENT)
        {
            const auto& k = rec.Event.KeyEvent;
            std::ostringstream line;
            line << "down: " << k.bKeyDown
                 << ", ENHANCED_KEY: " << !!(k.dwControlKeyState & ENHANCED_KEY)
                 << std::hex
                 << ", ctrl: 0x" << k.dwControlKeyState
                 << ", vcod: 0x" << k.wVirtualKeyCode
                 << ", scod: 0x" << k.wVirtualScanCode;
            std::cout << line.str() << '\n';
            log << line.str() << std::endl;
            if (k.wVirtualKeyCode == VK_ESCAPE && !k.bKeyDown)
                break;
        }
    }
    return 0;
}
