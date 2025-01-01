#pragma once

#include <string>

class QuantTable {
public:
    int length;
    int destination;
    int tableID;

    unsigned char table[64];

public:
    QuantTable() {}
    QuantTable(const std::string& data, int startIndex);
    QuantTable(const QuantTable& other);

    QuantTable& operator=(const QuantTable& other);

    void print();
};