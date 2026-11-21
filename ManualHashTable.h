#ifndef MANUAL_HASH_TABLE_H
#define MANUAL_HASH_TABLE_H

// ---------------------------------------------------------------------------
// ManualHashTable
//   A hash table built from scratch (NO std::unordered_map / std::map).
//   Collision resolution: SEPARATE CHAINING.
//   Layout: one dynamically allocated array of HashNode* ("buckets").
//           Each bucket is the head of a singly linked list of entries.
//
//   Average case: insert / find / remove = O(1)
//   Worst case  : O(n)  (every key hashes to the same bucket)
//   Memory      : O(n + m), n = entries, m = bucket count
// ---------------------------------------------------------------------------

struct HashNode {
    int        key;    // Vehicle ID (the thing we look up by)
    int        value;  // payload: e.g. index of the Vehicle in the fleet array
    HashNode* next;   // next node in the same bucket's chain
};

class ManualHashTable {
public:
    // bucketCount should ideally be a prime number to spread keys evenly.
    explicit ManualHashTable(int bucketCount = 101);
    ~ManualHashTable();

    // We own raw pointers -> copying would cause a double free.
    // Deleting these is a deliberate design decision (explain this to the grader).
    ManualHashTable(const ManualHashTable&) = delete;
    ManualHashTable& operator=(const ManualHashTable&) = delete;

    // Returns true if a NEW key was inserted.
    // Returns false if the key already existed (its value is UPDATED instead).
    bool insert(int key, int value);

    // Looks up a key. Returns true and writes to outValue if found.
    bool find(int key, int& outValue) const;

    // Removes a key. Returns true if it was present.
    bool remove(int key);

    int getSize()        const { return size; }
    int getBucketCount() const { return bucketCount; }

    // --- Diagnostics used by our Day-1 collision tests ---
    int  longestChain() const;   // length of the worst bucket (shows collisions)
    void printTable()   const;   // dumps every bucket and its chain

private:
    // Maps any int key (including negatives) into [0, bucketCount).
    int hash(int key) const;

    HashNode** buckets;      // array of chain heads
    int        bucketCount;  // number of buckets (array length)
    int        size;         // number of stored entries
};

#endif // MANUAL_HASH_TABLE_H#pragma once
