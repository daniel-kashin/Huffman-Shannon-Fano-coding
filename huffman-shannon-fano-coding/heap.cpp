#include "heap.h"
#include <climits>
#include <iostream>

Heap::
Heap(std::vector<std::pair<wchar_t, int>> frequencies,
    const int size) : capacity(size)
{
    storage = new HeapNode*[size];
    for (int i = 0; i < size; ++i) {
        storage[i] = new Heap::HeapNode(frequencies[i].first, frequencies[i].second);
    }

    currentSize = size;

    for (int i = (size - 2) / 2; i >= 0; --i) {
        heapifyDown(i);
    }
}

Heap::HeapNode*
Heap::
extractMin()
{
    if (currentSize >= 1) {

        Heap::HeapNode* min = storage[0];

        storage[0] = storage[currentSize - 1];

        --currentSize;

        if (currentSize > 1) {
            heapifyDown(0);
        }

        return min;
    }
    else {
        throw std::out_of_range(" ");
    }
}

void
Heap::
insert(Heap::HeapNode* newNode) {
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

Heap::HeapNode*
Heap::
buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences, const int size) {
    HeapNode *left;
    HeapNode *right;
    HeapNode *top;

    // Step 1: Create a min heap of capacity equal to size.  Initially, there are
    // modes equal to size.
    Heap* heap = new Heap(frequences, size);

    // Iterate while size of heap doesn't become 1
    while (heap->currentSize>1)
    {
        // Step 2: Extract the two minimum freq items from min heap
        left = heap->extractMin();
        right = heap->extractMin();

        // Step 3:  Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = new Heap::HeapNode(NULL, left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        heap->insert(top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return heap->extractMin();
}

bool
Heap::
isLeaf(Heap::HeapNode* root)
{
    return !(root->left) && !(root->right);
}

void
Heap::
printCodes(HeapNode* root, int arr[], int top)
{
    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (isLeaf(root))
    {
        printf("%c: ", root->value);
        printArr(arr, top);
    }
}

void 
Heap::
printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

void
Heap::
HuffmanCodes(std::vector<std::pair<wchar_t, int>> frequences, int size)
{
    Heap::HeapNode* root = Heap::buildHuffmanTree(frequences, size);

    // Print Huffman codes using the Huffman tree built above
    int arr[1000], top = 0;
    printCodes(root, arr, top);
}