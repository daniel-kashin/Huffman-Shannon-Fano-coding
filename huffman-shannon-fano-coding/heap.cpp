#include "heap.h"

#include <vector>
#include <climits>
#include <iostream>



Heap::Heap(std::vector<std::pair<wchar_t, int>> frequencies, const int size) : capacity(size)
{
    storage = new Node*[size];
    for (int i = 0; i < size; ++i) {
        storage[i] = new Heap::Node(frequencies[i].first, frequencies[i].second, nullptr, nullptr);
    }

    currentSize = size;

    for (int i = (size - 2) / 2; i >= 0; --i) {
        heapifyDown(i);
    }
}

Heap::Node::
Node(wchar_t value, int frequency, Heap::Node* left, Heap::Node* right)
{
    this->value = value;
    this->frequency = frequency;
    this->left = left;
    this->right = right;
}

Heap::Node*
Heap::
extractMin()
{
    if (currentSize >= 1) {

        Heap::Node* min = storage[0];

        storage[0] = storage[currentSize - 1];

        --currentSize;

        if (currentSize > 1) {
            heapifyDown(0);
        }

        return min;
    }
    else {
        return nullptr;
    }
}

void
Heap::
insert(Heap::Node* newNode) {
    currentSize++;

    int i = currentSize - 1;
    while (i && newNode->frequency < storage[(i - 1) / 2]->frequency)
    {
        storage[i] = storage[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    storage[i] = newNode;
}

void Heap::heapifyDown(int index)
{
    int smallest = index;
    int left = index * 2 + 1;
    int right = index * 2 + 2;

    if (left < currentSize && storage[left]->frequency < storage[smallest]->frequency) {
        smallest = left;
    }

    if (right < currentSize && storage[right]->frequency < storage[smallest]->frequency) {
        smallest = right;
    }

    if (smallest != index) {
        std::swap(storage[smallest], storage[index]);
        heapifyDown(smallest);
    }
}

Heap::~Heap()
{
    delete[] storage;
}

bool
Heap::Node::
isLeaf()
{
    return !(this->left) && !(this->right);
}

