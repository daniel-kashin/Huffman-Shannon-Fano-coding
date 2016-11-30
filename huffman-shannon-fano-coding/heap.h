#ifndef HEAP_H
#define HEAP_H

#include <vector> 

class Heap 
{
public:

    class Node  
    {
    public:

        Node(wchar_t value, int frequency) {
            this->value = value;
            this->frequency = frequency;
            this->left = this->right = nullptr;
        }

        bool isLeaf();

        Node() : Node(0, 0) { }

        wchar_t value;
        int frequency;
        Node *left;
        Node *right;
    };


    Heap(std::vector<std::pair<wchar_t, int>> frequences, const int n);

    ~Heap();


    Node* extractMin();

    void insert(Node* value);

    void heapifyDown(int index);

    


    Node** storage;

    int currentSize;

    const int capacity;
};

#endif