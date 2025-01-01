#include "HuffmanTree.h"

#include <iostream>

#include "HuffmanTable.h"
#include "BitStream.h"


HuffmanTree::HuffmanTree(const HuffmanTable& hTable) {
    const unsigned char* bitLengths = hTable.getBitLengths();
    const int elementCount = hTable.getElementCount();
    const unsigned char* elements = hTable.getElements();

    rootNode = new BranchNode();
    nodes[0] = rootNode;
    nodeCount = 1;

    for (int elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        unsigned char element = elements[elementIndex];
        int bitLength = 0;

        int sum = 0;
        for (int i = 0; i < 16; i++) {
            sum += (int)bitLengths[i];

            if (elementIndex < sum) {
                bitLength = i + 1;
                break;
            }
        }

        BranchNode* currentNode = dynamic_cast<BranchNode*>(rootNode);
        for (int i = 0; i < bitLength - 1; i++) {
            if (!currentNode->firstChild) {
                BranchNode* newNode = new BranchNode();
                newNode->parent = currentNode;
                registerNode(newNode);

                currentNode->firstChild = newNode;
                currentNode = newNode;
                continue;
            }

            if (!currentNode->firstChild->isFull()) {
                currentNode = dynamic_cast<BranchNode*>(currentNode->firstChild);
                continue;
            }

            if (!currentNode->secondChild) {
                BranchNode* newNode = new BranchNode();
                newNode->parent = currentNode;
                registerNode(newNode);

                currentNode->secondChild = newNode;
                currentNode = newNode;
                continue;
            }

            if (!currentNode->secondChild->isFull()) {
                currentNode = dynamic_cast<BranchNode*>(currentNode->secondChild);
                continue;
            }
        }

        if (currentNode->firstChild == nullptr) {
            LeafNode* newNode = new LeafNode();
            newNode->parent = currentNode;
            registerNode(newNode);

            currentNode->firstChild = newNode;
            newNode->value = element;
        }
        else {
            LeafNode* newNode = new LeafNode();
            newNode->parent = currentNode;
            registerNode(newNode);

            currentNode->secondChild = newNode;
            newNode->value = element;
        }
    }
}

unsigned char HuffmanTree::getElement(std::string bits) {
    Node* currentNode = rootNode;
    for (int i = 0; i < bits.length(); i++) {
        BranchNode* currentBranch = dynamic_cast<BranchNode*>(currentNode);
        if (!currentBranch) {
            std::cout << "Error: Invalid bitstream." << std::endl;
            return 0;
        }

        if (bits[i] == '0' && currentBranch->firstChild) {
            currentNode = currentBranch->firstChild;
            continue;
        }
        if (bits[i] == '1' && currentBranch->secondChild) {
            currentNode = currentBranch->secondChild;
            continue;
        }

        std::cout << "Error: Invalid bitstream." << std::endl;
        return 0;
    }

    LeafNode* elementNode = dynamic_cast<LeafNode*>(currentNode);
    if (elementNode) {
        return elementNode->value;
    }

    std::cout << "Error: Invalid bitstream." << std::endl;
    return 0;
}

unsigned char HuffmanTree::getCodeFromStream(BitStream& stream) {
    Node* currentNode = rootNode;

    while (true) {
        LeafNode* elementNode = dynamic_cast<LeafNode*>(currentNode);
        if (elementNode) {
            return elementNode->value;
        }

        BranchNode* branch = dynamic_cast<BranchNode*>(currentNode);
        if (!branch) {
            std::cout << "Error: Invalid node." << std::endl;
            return 0;
        }

        bool bit = stream.getBit();
        if (!bit) {
            currentNode = branch->firstChild;
        }
        else {
            currentNode = branch->secondChild;
        }
    }
}