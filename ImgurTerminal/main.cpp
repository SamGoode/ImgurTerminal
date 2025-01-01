#include <iostream>
#include <Windows.h>

#include "ImgurRequest.h"
#include "JPEG.h"



void setBackgroundColor(int rgba) {
    int r = (rgba >> 24) & 0xFF;
    int g = (rgba >> 16) & 0xFF;
    int b = (rgba >> 8) & 0xFF;

    std::cout << "\x1b[48;2;" + std::to_string(r) + ';' + std::to_string(g) + ';' + std::to_string(b) + 'm';
}

void setForegroundColor(int rgba) {
    int r = (rgba >> 24) & 0xFF;
    int g = (rgba >> 16) & 0xFF;
    int b = (rgba >> 8) & 0xFF;

    std::cout << "\x1b[38;2;" + std::to_string(r) + ';' + std::to_string(g) + ';' + std::to_string(b) + 'm';
}

void setupConsole() {
    SetConsoleOutputCP(437);

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

    DWORD consoleMode;
    GetConsoleMode(hout, &consoleMode);
    SetConsoleMode(hout, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

void setConsoleDimensions(int _width, int _height) {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

    SHORT height = (_height / 2) + 10;
    if (height > 500) {
        height = 500;
    }

    SHORT width = _width;

    SetConsoleScreenBufferSize(hout, { width, 9999 });

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(hout, false, &cfi);

    cfi.dwFontSize.Y = 2;
    cfi.dwFontSize.X = 1;

    SetCurrentConsoleFontEx(hout, false, &cfi);

    SMALL_RECT dim = { 0, 0, width - 1, height - 1 };

    if (!SetConsoleWindowInfo(hout, true, &dim)) {
        cfi.dwFontSize.Y = 16;
        cfi.dwFontSize.X = 8;

        SetCurrentConsoleFontEx(hout, false, &cfi);

        SetConsoleWindowInfo(hout, true, &dim);
    }
}

int main() {
    std::cout << "Insert Link: ";
    
    std::string link;
    std::cin >> link;

    std::string fileName;
    fileName = link.substr(link.find(".com/") + 5);

    ImgurRequest request = ImgurRequest(fileName);

    std::string data;
    request.GetImageData(data);

    JPEG jpg;
    jpg.getHeaders(data);
    jpg.getDataSOF(data);
    jpg.printImageInfo();

    int imgHeight = jpg.getImageHeight();
    int imgWidth = jpg.getImageWidth();

    int* pixelData = new int[imgHeight * imgWidth];
    jpg.decode(data, pixelData);

    int scaling = 1;

    setupConsole();

    setConsoleDimensions(imgWidth/scaling, imgHeight/scaling);

    std::cout << "\x1b[?25l\x1b[1;1H";

    for (int y = 0; y < (imgHeight/scaling); y += 2) {
        for (int x = 0; x < (imgWidth/scaling); x++) {
            int top = pixelData[y * scaling * imgWidth + (x * scaling)];
            setBackgroundColor(top);
            
            int bot = 0x00000000;
            if ((y + 1) * scaling < imgHeight) {
                bot = pixelData[(y + 1) * scaling * imgWidth + (x * scaling)];
            }
            setForegroundColor(bot);

            std::cout << (unsigned char)0xDC;
        }

        std::cout << std::endl;
    }

    setBackgroundColor(0x00000000);
    setForegroundColor(0xFFFFFF00);

    delete[] pixelData;

    return 0;
}

