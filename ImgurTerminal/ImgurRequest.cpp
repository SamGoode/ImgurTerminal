#include "ImgurRequest.h"

#include <iostream>
#include <Windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")


void ImgurRequest::GetImageData(std::string& data) {
	if (path == "") {
        std::cout << "Error: Path string is empty." << std::endl;

        return;
    }

    int result = 0;

    // Attempt to connect to internet.
    InternetAttemptConnect(result);
    if (result < 0) {
        std::cout << "Error: InternetAttemptConnect failed." << std::endl;

        return;
    }

    // Attempt to connect to imgur host.
    InternetCheckConnection(("https://" + host + "/" + path).c_str(), NULL, result);
    if (result < 0) {
        std::cout << "Error: InternetCheckConnection failed" << std::endl;

        return;
    }

    // Open a connection to imgur.
    HINTERNET handle = InternetOpen(userAgent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (handle == NULL) {
        std::cout << "Error: InternetOpen failed" << std::endl;

        return;
    }

    HINTERNET connection = InternetConnect(handle, "i.imgur.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
    if (connection == NULL) {
        std::cout << "Error: InternetConnect failed" << std::endl;

        InternetCloseHandle(handle);
        return;
    }

    LPCSTR acceptTypes[2] = { "image/jpeg", NULL };

    // Send HTTP GET request.
    HINTERNET request = HttpOpenRequest(connection, "GET", path.c_str(), NULL, NULL, acceptTypes, 0, NULL);
    if (request == NULL) {
        std::cout << "Error: HttpOpenRequest failed" << std::endl;

        InternetCloseHandle(connection);
        InternetCloseHandle(handle);
        return;
    }

    bool requestResult = HttpSendRequest(request, NULL, 0, NULL, 0);
    if (!requestResult) {
        std::cout << "Error: HttpSendRequest failed" << std::endl;

        InternetCloseHandle(request);
        InternetCloseHandle(connection);
        InternetCloseHandle(handle);
        return;
    }


    // Store HTTP response into data string.
    data = "";

    const int bufferSize = 1024;
    char buffer[bufferSize];

    bool keepReading = true;
    DWORD bytesRead = -1;

    while (keepReading && bytesRead != 0) {
        keepReading = InternetReadFile(request, buffer, bufferSize, &bytesRead);
        data.append(buffer, bytesRead);
    }

    InternetCloseHandle(request);
    InternetCloseHandle(connection);
    InternetCloseHandle(handle);
}
