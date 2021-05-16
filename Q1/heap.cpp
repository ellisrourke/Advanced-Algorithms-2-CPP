#include <iostream>
#include <vector>

using namespace std;

struct HeapNode {
    HeapNode* left;
    HeapNode* right;
};
// hwoever a more efficient way to implement instead of tree fashion
// is by using an array as there will never be a gap

class MinHeap {
    // how many elements can be in the heap
    int total_capacity = 0;
    // the heap array itself
    int* heap;
    // how many elements are in the heap
    int length = 0;

public:
    MinHeap() {
        this->total_capacity = 10;
        heap = new int[total_capacity];
    }
    MinHeap(int c) {
        this->total_capacity = c;
        heap = new int[total_capacity];
    }

    void print() {
        for ( int i = 0; i < length; i++)
            cout << heap[i] << " ";
        cout << endl;
    }

    // get the index
    int getLeftChildIdx(int p_idx) {return 2*p_idx + 1; };
    int getRightChildIdx(int p_idx) {return 2*p_idx + 2; };
    int getParentIdx(int c_idx) {return (c_idx - 1) / 2; };

    // check if a heap item has a certain branch or parent
    bool hasLeftChild(int idx) { return getLeftChildIdx(idx) < length; };
    bool hasRightChild(int idx) { return getRightChildIdx(idx) < length; };
    bool hasParent(int idx) { return getParentIdx(idx) < length; };

    // get the number from the heap
    int leftChild(int idx) { return heap[getLeftChildIdx(idx)]; };
    int rightChild(int idx) { return heap[getRightChildIdx(idx)]; };
    int parent(int idx) { return heap[getParentIdx(idx)]; };

    void swap(int i, int j) {
        int temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    // if total capacity is reached:
    // creates a larger array and copies all elements over to it
    void ensureExtraCapacity() {
        if (length == total_capacity) {
            total_capacity *= 2;
            int* temp = new int[total_capacity];
            for (int i = 0; i < length; i++)
                temp[i] = heap[i];
            delete[] heap;
            heap = temp;
            cout << "Capacity added." << endl;
        }
    }

    // gets first val... so the head
    int peek() {
        try {
            if (length == 0)
                throw 10;

            return heap[0];

        } catch (int x) {
            cout << "Error " << x << ": array is empty." << endl;
            return -1;
        }
    }

    // gets smallest val, and removes it from the array
    int pop() {
        try {
            if (length == 0)
                throw 10;

            int item = heap[0];
            heap[0] = heap[length-1];
            length--;
            heapifyDown();

            return item;

        } catch (int x) {
            cout << "Error " << x << ": array is empty." << endl;
            return -1;
        }
    }

    void add(int x) {
        ensureExtraCapacity();
        heap[length] = x;
        length++;
        heapifyUp();
    }

    void heapifyUp() {
        int i = length - 1;
        while (hasParent(i) && parent(i) > heap[i]) {
            swap(getParentIdx(i), i);
            i = getParentIdx(i);
        }
        cout << "heap is successfully ordered." << endl;
    }

    void heapifyDown() {
        int i = 0;
        while (hasLeftChild(i)) {
            int smallerChild = getLeftChildIdx(i);

            if (hasRightChild(i) && rightChild(i) < leftChild(i))
                smallerChild = getRightChildIdx(i);

            if (heap[i] < heap[smallerChild])
                break;
            else
                swap(i, smallerChild);

            i = smallerChild;
        }
        cout << "heap is successfully ordered." << endl;
    }


};

int main() {
    MinHeap mH(3);
    vector<int> vec{12, 4,10,5,29,3,59,48,92,32,1,54,76,32,87,21,65,23,65,32}, vec2{42,67,32,76,98,231,65,2,5};
    // cout << vec.size();
    for (auto& i : vec)
        mH.add(i);


    cout << mH.pop() << endl;
    cout << mH.pop() << endl;
    cout << mH.pop() << endl;
    cout << mH.pop() << endl;
    cout << mH.pop() << endl;

    //testing capacity check and increase
    //for (auto& i : vec2)
    //    mH.add(i);
//
   // mH.print();

    return 0;
}