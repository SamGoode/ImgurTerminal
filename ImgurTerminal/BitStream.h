#pragma once

class BitStream {
private:
    int bitPos;
    const char* data;

public:
    BitStream(const char* _data) {
        bitPos = 0;
        data = _data;
    }

    bool getBit() {
        int byteIndex = bitPos / 8;
        int bitShift = (7 - (bitPos % 8));
        bitPos++;
        
        unsigned char byte = data[byteIndex];
        bool bit = (byte >> bitShift) & 0x01;
        return bit;
    }

    int getBits(int count) {
        if (count < 1 || count > 32) {
            return -1;
        }

        int bits = 0;
        for (int i = 0; i < count; i++) {
            bits <<= 1;
            bits += getBit();
        }

        return bits;
    }
};