#pragma once

#include <string>

#include "QuantTable.h"
#include "HuffmanTable.h"
#include "HuffmanTree.h"


int getBytesAsInt(const std::string& data, int index, int bytes);

struct QTComponent {
    int ID;
    int sampFactor;
    int quantID;
};


class JPEG {
private:
    enum Header {
        NA,
        SOI,
        APP0,
        APP2,
        DQT,
        SOF,
        DHT,
        SOS,
        EOI
    };

    const std::string headerNames[9] = {
        "Unknown",
        "Start of Image",
        "Application0",
        "Application2",
        "Define Quantization Table",
        "Start of Frame",
        "Define Huffman Table",
        "Start of Scan",
        "End of Image"
    };

private:
    Header headers[64];
    int headerPositions[64];
    int headerCount = 0;

    int imageHeight;
    int imageWidth;
    int componentCount;
    QTComponent components[3];

    QuantTable qTables[2];
    HuffmanTable hTables[2][2];

    std::string ECS;
    float IDCT[8][8];

public:
    JPEG();

    int getImageHeight() { return imageHeight; }
    int getImageWidth() { return imageWidth; }

    void getHeaders(const std::string& data);
    void printHeaders();

    void getDataSOF(const std::string& data);
    void printImageInfo();

    void parseImageData(const std::string& data, int startIndex);

    int decodeRLC(int size, int bits);

    void inverseDCT(int DCT[64], int output[64]);

    void buildMCU(class BitStream& stream, class QuantTable& qTable, class HuffmanTable& hTableDC, class HuffmanTable& hTableAC, int& lastDcCoeff, int output[64]);

    void decode(const std::string& data, int* output);
};