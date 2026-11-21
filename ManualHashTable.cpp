#include "ManualHashTable.h"
#include <iostream>

// ------------------------------ Constructor --------------------------------
ManualHashTable::ManualHashTable(int bucketCount) {
    // Guard against nonsense input so we never allocate a 0-length array.
    if (bucketCount <= 0) bucketCount = 101;

    this->bucketCount = bucketCount;
    this->size = 0;

    // One contiguous array of pointers; every slot starts as an empty chain.
    buckets = new HashNode * [bucketCount];
    for (int i = 0; i < bucketCount; ++i) {
        buckets[i] = nullptr;
    }
}

// ------------------------------- Destructor --------------------------------
// Must free EVERY node in EVERY chain, then the bucket array itself.
// (Forgetting this is the #1 memory leak in student hash tables.)
ManualHashTable::~ManualHashTable() {
    for (int i = 0; i < bucketCount; ++i) {
        HashNode* cur = buckets[i];
        while (cur != nullptr) {
            HashNode* next = cur->next;   // save next BEFORE deleting cur
            delete cur;
            cur = next;
        }
    }
    delete[] buckets;
    buckets = nullptr;
}

// -------------------------------- Hashing ----------------------------------
int ManualHashTable::hash(int key) const {
    int h = key % bucketCount;
    // In C++, (-7) % 5 == -2. Normalize so the index is always valid.
    if (h < 0) h += bucketCount;
    return h;
}

// --------------------------------- Insert ----------------------------------
// Prepend to the bucket's chain => O(1) even in the worst case.
bool ManualHashTable::insert(int key, int value) {
    const int idx = hash(key);

    // Walk the chain: if the key is already there, just update its payload.
    for (HashNode* cur = buckets[idx]; cur != nullptr; cur = cur->next) {
        if (cur->key == key) {
            cur->value = value;
            return false;               // not a new entry
        }
    }

    // Key not found -> create a node and push it to the FRONT of the chain.
    HashNode* node = new HashNode;
    node->key = key;
    node->value = value;
    node->next = buckets[idx];
    buckets[idx] = node;

    ++size;
    return true;
}

// ---------------------------------- Find -----------------------------------
bool ManualHashTable::find(int key, int& outValue) const {
    const int idx = hash(key);

    for (HashNode* cur = buckets[idx]; cur != nullptr; cur = cur->next) {
        if (cur->key == key) {
            outValue = cur->value;
            return true;
        }
    }
    return false;
}

// --------------------------------- Remove ----------------------------------
// Classic linked-list unlink: must handle the "node is the head" case.
bool ManualHashTable::remove(int key) {
    const int idx = hash(key);

    HashNode* prev = nullptr;
    HashNode* cur = buckets[idx];

    while (cur != nullptr) {
        if (cur->key == key) {
            if (prev == nullptr) {
                buckets[idx] = cur->next;   // removing the head of the chain
            }
            else {
                prev->next = cur->next;     // bypass the middle node
            }
            delete cur;
            --size;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;                            // key was never stored
}

// ------------------------------ Diagnostics --------------------------------
int ManualHashTable::longestChain() const {
    int worst = 0;
    for (int i = 0; i < bucketCount; ++i) {
        int len = 0;
        for (HashNode* cur = buckets[i]; cur != nullptr; cur = cur->next) {
            ++len;
        }
        if (len > worst) worst = len;
    }
    return worst;
}

void ManualHashTable::printTable() const {
    std::cout << "--- HashTable (buckets=" << bucketCount
        << ", size=" << size << ") ---\n";
    for (int i = 0; i < bucketCount; ++i) {
        if (buckets[i] == nullptr) continue;   // skip empty buckets for readability
        std::cout << "  [" << i << "] ";
        for (HashNode* cur = buckets[i]; cur != nullptr; cur = cur->next) {
            std::cout << "(" << cur->key << "->" << cur->value << ")";
            if (cur->next) std::cout << " -> ";
        }
        std::cout << "\n";
    }
}