#ifndef MANUALCIRCULARQUEUE_H
#define MANUALCIRCULARQUEUE_H

#include <cstddef>      // std::size_t
#include <stdexcept>    // std::invalid_argument

template <typename T>
class ManualCircularQueue
{
public:
    /**
     * @param capacity maximum number of elements (must be > 0).
     */
    explicit ManualCircularQueue(int capacity)
        : m_data(capacity > 0 ? new T[static_cast<std::size_t>(capacity)] : nullptr)
        , m_capacity(capacity)
        , m_front(0)
        , m_rear(0)
        , m_count(0)
    {
        if (capacity <= 0)
            throw std::invalid_argument("ManualCircularQueue: capacity must be > 0");
    }

    // ---------- Rule of Three: we own a heap array ----------

    ~ManualCircularQueue()
    {
        delete[] m_data;          // m_data may be nullptr -> delete[] on nullptr is safe
    }

    ManualCircularQueue(const ManualCircularQueue& other)
        : m_data(new T[static_cast<std::size_t>(other.m_capacity)])
        , m_capacity(other.m_capacity)
        , m_front(other.m_front)
        , m_rear(other.m_rear)
        , m_count(other.m_count)
    {
        // Deep copy: the copy must NOT share the array with the original,
        // otherwise both destructors would delete[] the same memory.
        for (int i = 0; i < m_capacity; ++i)
            m_data[i] = other.m_data[i];
    }

    ManualCircularQueue& operator=(const ManualCircularQueue& other)
    {
        if (this == &other)       // self-assignment guard: q = q;
            return *this;

        // Allocate BEFORE deleting, so a bad_alloc leaves *this intact.
        T* newData = new T[static_cast<std::size_t>(other.m_capacity)];
        for (int i = 0; i < other.m_capacity; ++i)
            newData[i] = other.m_data[i];

        delete[] m_data;
        m_data = newData;
        m_capacity = other.m_capacity;
        m_front = other.m_front;
        m_rear = other.m_rear;
        m_count = other.m_count;
        return *this;
    }

    // (Rule of Five note: move ctor/assignment are intentionally not
    //  implemented for this prototype — copying is cheap here.)

    // ---------- Core queue operations ----------

    /** Adds a copy of `value` at the rear. @return false if the queue is full. */
    bool enqueue(const T& value)
    {
        if (isFull())
            return false;

        m_data[m_rear] = value;               // write into the free slot
        m_rear = nextIndex(m_rear);           // advance, wrapping with %
        ++m_count;
        return true;
    }

    /** Removes the front element into `outValue`. @return false if empty. */
    bool dequeue(T& outValue)
    {
        if (isEmpty())
            return false;

        outValue = m_data[m_front];           // read the oldest element
        m_front = nextIndex(m_front);         // advance, wrapping with %
        --m_count;
        return true;
    }

    /** Copies the front element without removing it. @return false if empty. */
    bool peek(T& outValue) const
    {
        if (isEmpty())
            return false;
        outValue = m_data[m_front];
        return true;
    }

    // ---------- Queries ----------

    bool isEmpty()  const { return m_count == 0; }
    bool isFull()   const { return m_count == m_capacity; }
    int  size()     const { return m_count; }
    int  capacity() const { return m_capacity; }

    /** Resets to an empty queue. O(1) — no need to touch the array contents. */
    void clear()
    {
        m_front = 0;
        m_rear = 0;
        m_count = 0;
    }

private:
    /** The modulo trick: 0,1,2,...,capacity-1,0,1,... */
    int nextIndex(int i) const { return (i + 1) % m_capacity; }

    T* m_data;        // raw heap array, owned by this class
    int m_capacity;    // fixed maximum size
    int m_front;       // index of the oldest element
    int m_rear;        // index of the next free slot
    int m_count;       // current number of elements
};

#endif // MANUALCIRCULARQUEUE_H#pragma once
