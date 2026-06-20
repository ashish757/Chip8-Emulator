//
// Created by Ashish Raj Singh on 20/06/26.
//

#include <iostream>
#include <string>
#include <array>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>

std::string getRomPath() {
    char filename[MAX_PATH] = {0};

    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "CHIP-8 ROMs\0*.ch8\0ALL Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "ch8";

    if (GetOpenFileNameA(&ofn)) {
        return std::string(filename);
    }

    return "";
}

#elif defined(__APPLE__)
std::string getRomPath() {
    std::array<char, 128> buf;
    std::string res = "";
    FILE* pipe = popen("osascript -e 'POSIX path of (choose file of type {\"ch8\"} with prompt \"Select a CHIP-8 ROM\")' 2>/dev/null", "r");
    if (!pipe) return "";
    while (fgets(buf.data(), buf.size(), pipe) != nullptr) {
        res += buf.data();
    }
    pclose(pipe);
    if (!res.empty() && res.back() == '\n') {
        res.pop_back();
    }
    return res;
}

#else

std::string getRomPath() {
    std::cout << "Native file dialog not implemented for this OS.\n";
    return "";
}

#endif
