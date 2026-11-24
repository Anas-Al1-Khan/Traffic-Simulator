#ifndef MANUALCIRCULARQUEUE_H
#define MANUALCIRCULARQUEUE_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class ManualCircularQueue
{
public:
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

    ~ManualCircularQueue()
    {
        delete[] m_data;
    }

    // Copy constructor
    ManualCircularQueue(const ManualCircularQueue& other)
        : m_data(new T[static_cast<std::size_t>(other.m_capacity)])
        , m_capacity(other.m_capacity)
        , m_front(other.m_front)
        , m_rear(other.m_rear)
        , m_count(other.m_count)
    {
        for (int i = 0; i < m_capacity; ++i)
            m_data[i] = other.m_data[i];
    }

    // Copy assignment operator
    ManualCircularQueue& operator=(const ManualCircularQueue& other)
    {
        if (this == &other)
            return *this;

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

    // Queue operations
    bool enqueue(const T& value)
    {
        if (isFull())
            return false;

        m_data[m_rear] = value;
        m_rear = nextIndex(m_rear);
        ++m_count;

        return true;
    }

    bool dequeue(T& outValue)
    {
        if (isEmpty())
            return false;

        outValue = m_data[m_front];
        m_front = nextIndex(m_front);
        --m_count;

        return true;
    }

    bool peek(T& outValue) const
    {
        if (isEmpty())
            return false;

        outValue = m_data[m_front];
        return true;
    }

    // Queue status
    bool isEmpty() const { return m_count == 0; }
    bool isFull() const { return m_count == m_capacity; }
    int size() const { return m_count; }
    int capacity() const { return m_capacity; }

    void clear()
    {
        m_front = 0;
        m_rear = 0;
        m_count = 0;
    }

private:
    int nextIndex(int i) const
    {
        return (i + 1) % m_capacity;
    }

    T* m_data;
    int m_capacity;
    int m_front;
    int m_rear;
    int m_count;
};

#endif
