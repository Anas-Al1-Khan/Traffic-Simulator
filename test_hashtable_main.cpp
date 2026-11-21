#include "ManualHashTable.h"
#include <iostream>

// Helper: assert-style check that prints PASS/FAIL so the grader can see it.
static void check(bool condition, const char* label) {
    std::cout << (condition ? "[PASS] " : "[FAIL] ") << label << "\n";
}

int main() {
    std::cout << "===== DAY 1: Manual Hash Table Tests =====\n\n";

    // --- TEST 1: Basic insert + find -------------------------------------
    std::cout << "[TEST 1] Basic insert / find\n";
    {
        ManualHashTable ht(101);
        ht.insert(1001, 0);
        ht.insert(1002, 1);
        ht.insert(1003, 2);

        int out = -1;
        check(ht.find(1002, out) && out == 1, "find(1002) returns value 1");
        check(!ht.find(9999, out), "find(9999) correctly fails");
        check(ht.getSize() == 3, "size == 3");
    }

    // --- TEST 2: FORCED COLLISIONS ---------------------------------------
    // With only 7 buckets, keys 1, 8, 15, 22, 29 ALL hash to bucket 1.
    // This proves separate chaining actually works.
    std::cout << "\n[TEST 2] Forced collisions (7 buckets)\n";
    {
        ManualHashTable ht(7);
        for (int i = 0; i < 5; ++i) {
            int key = 1 + i * 7;                 // 1, 8, 15, 22, 29
            ht.insert(key, key * 10);
        }
        ht.insert(3, 30);                        // lands in bucket 3, no collision
        ht.printTable();

        check(ht.longestChain() == 5, "one chain has length 5 (all 5 colliding keys)");

        int out = -1;
        check(ht.find(22, out) && out == 220, "collided key 22 still found in O(chain)");
        check(ht.find(3, out) && out == 30, "non-colliding key 3 found");
    }

    // --- TEST 3: Duplicate key updates, does not grow --------------------
    std::cout << "\n[TEST 3] Duplicate key update\n";
    {
        ManualHashTable ht(11);
        check(ht.insert(42, 1) == true, "first insert(42) returns true");
        check(ht.insert(42, 2) == false, "second insert(42) returns false (update)");
        int out = -1;
        check(ht.find(42, out) && out == 2, "value was updated to 2");
        check(ht.getSize() == 1, "size still 1");
    }

    // --- TEST 4: Removal, including removing a CHAIN HEAD ---------------
    std::cout << "\n[TEST 4] Removal (head of chain + middle)\n";
    {
        ManualHashTable ht(7);
        ht.insert(1, 10);   // bucket 1
        ht.insert(8, 20);   // bucket 1  (8 % 7 == 1)
        ht.insert(15, 30);  // bucket 1
        // chain is now: 15 -> 8 -> 1   (prepend order)

        check(ht.remove(15), "remove head of chain (15) succeeds");
        check(ht.longestChain() == 2, "chain length dropped to 2");

        check(ht.remove(1), "remove tail of chain (1) succeeds");
        int out = -1;
        check(!ht.find(1, out), "removed key 1 is gone");
        check(ht.find(8, out) && out == 20, "surviving key 8 is intact");
        check(!ht.remove(12345), "removing absent key returns false");
    }

    // --- TEST 5: EMPTY TABLE EDGE CASE -----------------------------------
    std::cout << "\n[TEST 5] Empty table edge case\n";
    {
        ManualHashTable ht(5);
        int out = -1;
        check(ht.getSize() == 0, "empty table size == 0");
        check(!ht.find(1, out), "find on empty table returns false");
        check(!ht.remove(1), "remove on empty table returns false");
        check(ht.longestChain() == 0, "longest chain on empty table == 0");
    }   // destructor runs here -> valgrind should report 0 leaks

    // --- TEST 6: Negative keys ------------------------------------------
    std::cout << "\n[TEST 6] Negative keys\n";
    {
        ManualHashTable ht(13);
        ht.insert(-5, 55);
        int out = -1;
        check(ht.find(-5, out) && out == 55, "negative key -5 handled correctly");
    }

    std::cout << "\n===== DAY 1 TESTS COMPLETE =====\n";
    return 0;
}