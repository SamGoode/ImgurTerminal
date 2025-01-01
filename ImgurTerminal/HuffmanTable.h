#pragma once

#include <string>


class HuffmanTable {
public:
    int length;
    int classType;
    int tableID;

private:
    unsigned char bitLengths[16];
    int elementCount;
    unsigned char* elements = nullptr;

    class HuffmanTree* hTree = nullptr;

public:
    HuffmanTable() {}
    HuffmanTable(const std::string& data, int startIndex);
    ~HuffmanTable() { delete[] elements; delete hTree; }
    HuffmanTable(const HuffmanTable& other);

    HuffmanTable& operator=(const HuffmanTable& other);

    const unsigned char* getBitLengths() const { return bitLengths; }

    int getElementCount() const { return elementCount; }

    const unsigned char* getElements() const { return elements; }

    void print();

    unsigned char getCodeFromStream(class BitStream& stream);
};