#include <iostream>
#include "ManualMinHeap.h"
#include "ManualHashTable.h"
#include "Stats.h"

static void check(bool condition, const char* label) {
    std::cout << (condition ? "[PASS] " : "[FAIL] ") << label << "\n";
}

int main() {
    std::cout << "===== DAY 5: Edge Cases & Stats Tests =====\n\n";

    // TEST 1: Empty Hash Table
    std::cout << "[TEST 1] Empty Hash Table\n";
    {
        ManualHashTable ht(5);
        int out;
        check(ht.getSize() == 0, "Size is 0");
        check(!ht.find(100, out), "Find on empty table returns false");
        check(!ht.remove(100), "Remove on empty table returns false");
    }

    // TEST 2: Full / Overflowing Heap
    std::cout << "\n[TEST 2] Heap Resizing & Edge Cases\n";
    {
        ManualMinHeap heap(2); // Start very small to force resize
        heap.insert(5);
        heap.insert(3);
        heap.insert(10); // Triggers resize
        heap.insert(1);

        check(heap.getSize() == 4, "Heap resized correctly, size is 4");
        check(heap.extractMin() == 1, "Extracted 1 (Min)");
        check(heap.extractMin() == 3, "Extracted 3");
        check(heap.extractMin() == 5, "Extracted 5");
        check(heap.extractMin() == 10, "Extracted 10");
        check(heap.isEmpty() == true, "Heap is now empty");
    }

    // TEST 3: Average Wait Time
    std::cout << "\n[TEST 3] Average Wait Time\n";
    {
        int times[] = { 10, 20, 30, 40 };
        double avg = Stats::calculateAverageWaitTime(times, 4);
        check(avg == 25.0, "Average of {10,20,30,40} is 25.0");

        double emptyAvg = Stats::calculateAverageWaitTime(nullptr, 0);
        check(emptyAvg == 0.0, "Average of empty array is 0.0");
    }

    std::cout << "\n===== DAY 5 TESTS COMPLETE =====\n";
    return 0;
}