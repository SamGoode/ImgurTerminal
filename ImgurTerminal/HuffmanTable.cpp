#include "HuffmanTable.h"

#include <iostream>

#include "HuffmanTree.h"

HuffmanTable::HuffmanTable(const std::string& data, int startIndex) {
    int index = startIndex;

    length = ((int)(unsigned char)data[index] << 8) + (int)(unsigned char)data[index + 1];
    index += 2;

    unsigned char byte = data[index];
    classType = byte >> 4;
    tableID = 0x0F & byte;
    index++;

    elementCount = 0;
    for (int i = 0; i < 16; i++) {
        bitLengths[i] = data[index + i];
        elementCount += (int)data[index + i];
    }
    index += 16;

    if (elementCount + 19 != length) {
        std::cout << "Error: number of elements in Huffman table does not match its remaining length.";
        return;
    }

    elements = new unsigned char[elementCount];
    for (int i = 0; i < elementCount; i++) {
        elements[i] = data[index + i];
    }
    index += elementCount;

    hTree = new HuffmanTree(*this);
}

HuffmanTable::HuffmanTable(const HuffmanTable& other) {
    length = other.length;
    classType = other.classType;
    tableID = other.tableID;

    for (int i = 0; i < 16; i++) {
        bitLengths[i] = other.bitLengths[i];
    }

    elementCount = other.elementCount;
    elements = new unsigned char[elementCount];
    for (int i = 0; i < elementCount; i++) {
        elements[i] = other.elements[i];
    }

    hTree = new HuffmanTree(*this);
}

HuffmanTable& HuffmanTable::operator=(const HuffmanTable& other) {
    length = other.length;
    classType = other.classType;
    tableID = other.tableID;

    for (int i = 0; i < 16; i++) {
        bitLengths[i] = other.bitLengths[i];
    }

    elementCount = other.elementCount;
    delete[] elements;
    elements = new unsigned char[elementCount];
    for (int i = 0; i < elementCount; i++) {
        elements[i] = other.elements[i];
    }

    delete hTree;
    hTree = new HuffmanTree(*this);

    return *this;
}

void HuffmanTable::print() {
    std::cout << "Header length: " << length << std::endl;
    std::cout << "Class: " << classType << std::endl;
    std::cout << "ID: " << tableID << std::endl;

    std::cout << "Bit lengths: ";
    for (int i = 0; i < 16; i++) {
        std::cout << (int)bitLengths[i] << ",";
    }
    std::cout << std::endl;

    std::cout << "Element count: " << elementCount << std::endl;
    std::cout << "Elements: ";
    for (int i = 0; i < elementCount; i++) {
        std::cout << (int)elements[i] << ",";
    }
    std::cout << std::endl;
}

unsigned char HuffmanTable::getCodeFromStream(BitStream& stream) {
    return hTree->getCodeFromStream(stream);
}
