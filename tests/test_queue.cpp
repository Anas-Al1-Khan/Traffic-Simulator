#include "ManualCircularQueue.h"

#include <cassert>
#include <iostream>

static int g_checks = 0;

static void check(bool condition, const char* what)
{
    ++g_checks;
    if (condition) {
        std::cout << "[PASS] " << what << '\n';
    }
    else {
        std::cout << "[FAIL] " << what << '\n';
        std::exit(1);               // abort immediately on failure
    }
}

int main()
{
    // ---- Test 1: basic FIFO order ----
    {
        ManualCircularQueue<int> q(4);
        check(q.isEmpty(), "new queue is empty");
        check(q.enqueue(10) && q.enqueue(20) && q.enqueue(30), "enqueue 3 items");

        int v = 0;
        check(q.peek(v) && v == 10, "peek returns 10 without removing");
        check(q.dequeue(v) && v == 10, "dequeue 1st item -> 10");
        check(q.dequeue(v) && v == 20, "dequeue 2nd item -> 20");
        check(q.size() == 1, "size is 1 after two dequeues");
    }

    // ---- Test 2: modulo wrap-around (the whole point) ----
    {
        ManualCircularQueue<int> q(4);
        q.enqueue(1); q.enqueue(2); q.enqueue(3); q.enqueue(4);
        check(q.isFull(), "queue of 4 is full");

        int v = 0;
        q.dequeue(v); q.dequeue(v);         // front moves past the end
        check(q.enqueue(5) && q.enqueue(6), "enqueue after wrap-around succeeds");
        // Logical order must now be 3,4,5,6 even though memory is rotated.
        check(q.dequeue(v) && v == 3, "wrap-around order: 3");
        check(q.dequeue(v) && v == 4, "wrap-around order: 4");
        check(q.dequeue(v) && v == 5, "wrap-around order: 5");
        check(q.dequeue(v) && v == 6, "wrap-around order: 6");
        check(q.isEmpty(), "queue empty after draining");
    }

    // ---- Test 3: overflow / underflow are rejected, not crashed ----
    {
        ManualCircularQueue<int> q(2);
        q.enqueue(1); q.enqueue(2);
        check(!q.enqueue(3), "enqueue on full queue returns false");

        int v = 0;
        q.dequeue(v); q.dequeue(v);
        check(!q.dequeue(v), "dequeue on empty queue returns false");
        check(!q.peek(v), "peek on empty queue returns false");
    }

    // ---- Test 4: clear() resets in O(1) ----
    {
        ManualCircularQueue<int> q(3);
        q.enqueue(7); q.enqueue(8);
        q.clear();
        check(q.isEmpty() && q.size() == 0, "clear() empties the queue");
        q.enqueue(9);
        int v = 0;
        check(q.dequeue(v) && v == 9, "queue reusable after clear()");
    }

    // ---- Test 5: Rule of Three — deep copy must not double-delete ----
    {
        ManualCircularQueue<int> original(3);
        original.enqueue(100); original.enqueue(200);

        ManualCircularQueue<int> copy(original);   // copy constructor
        int v = 0;
        check(copy.dequeue(v) && v == 100, "copy has independent contents");

        check(original.size() == 2, "original untouched by changes to copy");

        ManualCircularQueue<int> assigned(1);
        assigned = original;                       // copy assignment
        check(assigned.size() == 2, "copy assignment copies size");
        assigned = assigned;                       // self-assignment must be safe
        check(assigned.size() == 2, "self-assignment is safe");
    }

    std::cout << "\nAll " << g_checks << " checks passed.\n";
    return 0;
}