#include "QuantTable.h"

#include <iostream>

#include "JPEG.h"


QuantTable::QuantTable(const std::string& data, int startIndex) {
    int index = startIndex;

    length = getBytesAsInt(data, index, 2);
    index += 2;

    int byte = getBytesAsInt(data, index, 1);
    destination = byte >> 4;
    tableID = byte & 0x0F;
    index++;

    std::copy(&data[index], &data[index + 64], table);
    index += 64;
}

QuantTable::QuantTable(const QuantTable& other) {
    length = other.length;
    destination = other.destination;
    tableID = other.tableID;

    for (int i = 0; i < 64; i++) {
        table[i] = other.table[i];
    }
}

QuantTable& QuantTable::operator=(const QuantTable& other) {
    length = other.length;
    destination = other.destination;
    tableID = other.tableID;

    for (int i = 0; i < 64; i++) {
        table[i] = other.table[i];
    }

    return *this;
}

void QuantTable::print() {
    std::cout << "Header length: " << length << std::endl;
    std::cout << "Destination: " << destination << std::endl;
    std::cout << "ID: " << tableID << std::endl;

    std::cout << "Table data: " << std::endl;
    for (int i = 0; i < 8; i++) {
        for (int n = 0; n < 8; n++) {
            std::cout << (int)table[i + n] << ' ';
        }
        std::cout << std::endl;
    }
}