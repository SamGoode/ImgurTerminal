#pragma once

#include <string>

class ImgurRequest {
private:
    std::string host = "i.imgur.com";
    std::string path;

    std::string userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:133.0) Gecko/20100101 Firefox/133.0";

public:
    ImgurRequest() {
        path = "";
    }

    ImgurRequest(std::string _path) {
        path = _path;
    }

    void GetImageData(std::string& data);
};