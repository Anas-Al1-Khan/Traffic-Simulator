#ifndef MANUAL_MIN_HEAP_H
#define MANUAL_MIN_HEAP_H

// A min-heap built from scratch using a raw array.
// No STL containers.
class ManualMinHeap {
public:
    explicit ManualMinHeap(int initialCapacity = 10);
    ~ManualMinHeap();

    // Prevent copying to avoid double-free of the raw array
    ManualMinHeap(const ManualMinHeap&) = delete;
    ManualMinHeap& operator=(const ManualMinHeap&) = delete;

    void insert(int value);
    int extractMin();
    int peekMin() const;
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;

private:
    int* heapArray;
    int capacity;
    int size;

    void resize();
    void percolateUp(int index);
    void percolateDown(int index);
    void swap(int& a, int& b);
};

#endif#pragma once
