#include "ManualMinHeap.h"
#include <iostream>

ManualMinHeap::ManualMinHeap(int initialCapacity) {
    if (initialCapacity <= 0) initialCapacity = 10;
    capacity = initialCapacity;
    size = 0;
    heapArray = new int[capacity];
}

ManualMinHeap::~ManualMinHeap() {
    delete[] heapArray;
    heapArray = nullptr;
}

void ManualMinHeap::resize() {
    capacity *= 2;
    int* newArray = new int[capacity];
    for (int i = 0; i < size; ++i) {
        newArray[i] = heapArray[i];
    }
    delete[] heapArray;
    heapArray = newArray;
}

void ManualMinHeap::swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void ManualMinHeap::insert(int value) {
    if (size == capacity) {
        resize();
    }
    heapArray[size] = value;
    percolateUp(size);
    size++;
}

void ManualMinHeap::percolateUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heapArray[index] < heapArray[parent]) {
            swap(heapArray[index], heapArray[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

int ManualMinHeap::extractMin() {
    if (isEmpty()) {
        std::cerr << "Error: Heap is empty.\n";
        return -1; // Or throw an exception
    }
    int minVal = heapArray[0];
    heapArray[0] = heapArray[size - 1];
    size--;
    percolateDown(0);
    return minVal;
}

void ManualMinHeap::percolateDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && heapArray[left] < heapArray[smallest]) {
            smallest = left;
        }
        if (right < size && heapArray[right] < heapArray[smallest]) {
            smallest = right;
        }
        if (smallest != index) {
            swap(heapArray[index], heapArray[smallest]);
            index = smallest;
        }
        else {
            break;
        }
    }
}

int ManualMinHeap::peekMin() const {
    if (isEmpty()) return -1;
    return heapArray[0];
}

bool ManualMinHeap::isEmpty() const { return size == 0; }
bool ManualMinHeap::isFull() const { return size == capacity; }
int ManualMinHeap::getSize() const { return size; }