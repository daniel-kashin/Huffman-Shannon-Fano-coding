#pragma once

#include <vector> 

class Heap 
{
public:

    class Node  
    {
    public:

        Node(wchar_t value, int frequency, Node* left, Node* right);

        Node(wchar_t value, Node* left, Node* right) : Node(value, 0, left, right) {}

        Node() : Node(0, 0, nullptr, nullptr) {}



        bool isLeaf();


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