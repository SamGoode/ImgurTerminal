#pragma once

#include <string>


class HuffmanTree {
private:
    struct Node {
        Node* parent = nullptr;

        virtual ~Node() {}

        virtual bool isFull() = 0;
    };

    struct BranchNode : public Node {
        Node* firstChild = nullptr;
        Node* secondChild = nullptr;

        BranchNode() {}
        BranchNode(const Node& base) {}

        virtual bool isFull() override {
            if (!firstChild || !secondChild) { return false; }
            return firstChild->isFull() && secondChild->isFull();
        }
    };

    struct LeafNode : public Node {
        unsigned char value;

        virtual bool isFull() override { return true; }
    };

private:
    Node* nodes[512];
    Node* rootNode = nullptr;
    int nodeCount;

public:
    HuffmanTree(const class HuffmanTable& hTable);

    ~HuffmanTree() {
        for (int i = 0; i < nodeCount; i++) { delete nodes[i]; }
    }

    void registerNode(Node* newNode) {
        nodes[nodeCount] = newNode;
        nodeCount++;
    }

    unsigned char getElement(std::string bits);

    unsigned char getCodeFromStream(class BitStream& stream);
};