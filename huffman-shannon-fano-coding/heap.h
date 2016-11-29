#include <vector> 

class Heap {
    
public:
   
    class HeapNode
    {
    public:
        
        HeapNode(wchar_t value, int frequency) {
            this->value = value;
            this->frequency = frequency;
            this->left = this->right = nullptr;
        }

        HeapNode() : HeapNode(0,0) { }

        wchar_t value;
        int frequency;
        HeapNode *left;
        HeapNode *right;
    };
    
    Heap(std::vector<std::pair<wchar_t, int>> frequences, const int n);

    ~Heap();

    static Heap::HeapNode * buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences, const int size);

    static void HuffmanCodes(std::vector<std::pair<wchar_t, int>> frequences, int size);

    /**
    * Getting the minimum value from the heap and also extracting it from the heap.
    * After this function, the heap should be modified and be in proper "heap" state.
    *
    * If heap currently has 0 elements stored, then the std::length_error exception
    * should be thrown with informative message.
    *
    * Complexity: O(log(current_size))
    *
    * Returns:
    *  minimum value that is in the heap.
    */
    HeapNode* extractMin();

    /**
    * Inserting the value in the heap.
    * After this function, the heap should be modified and be in proper "heap" state.
    *
    * If heap currently has "capacity" elements stored, then the std::length_error exception
    * should be thrown with informative message.
    *
    * Complexity: O(log(current_size + 1))
    */
    void insert(HeapNode* value);

    static bool isLeaf(HeapNode* root);

    static void printArr(int arr[], int n);
    
    static void printCodes(HeapNode* root, int arr[], int top);

    // commented for testing
    //private:
    /**
    * Method to perform heapify operation to the specified element in terms of checking
    * the "heap" state for it children's values. In other words we need to "bubble-down"
    * the specified element swapping it with one of the child nodes if necessary. After the
    * function execution, the heap storage should be in proper "heap" state (children nodes
    * are equal or greater than parent node).
    *
    * Hint: this method should be used in "extract_min" function.
    *
    * Complexity: O(log(current_size))
    */
    void heapifyDown(int index);

   

    HeapNode** storage;

    int currentSize;

    const int capacity;
};