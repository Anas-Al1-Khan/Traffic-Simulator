#include "EmergencyQueue.h"

// We need a way to map the integer in the heap back to the Vehicle.
// To keep it strict to the syllabus (manual structures), we will use a parallel array.
// Note: In a full project, you'd likely store Vehicle objects or pointers in the heap.
// For simplicity here, we encode the data into a single int: 
// (Priority * 10000) + (ArrivalTime * 100) + ID.
// Priority 0 (Emergency) will always be smaller than Priority 1 (Normal).

EmergencyQueue::EmergencyQueue(int capacity) : priorityHeap(capacity) {}

void EmergencyQueue::addVehicle(int id, bool isEmergency, int currentTime) {
    // Lower number = higher priority in our Min-Heap
    int priority = isEmergency ? 0 : 1;

    // Encode: [Priority (1 digit)] [ArrivalTime (2 digits)] [ID (2 digits)]
    // Example: Emergency (0), Time 5, ID 12 -> 00512
    int encodedValue = (priority * 10000) + ((currentTime % 100) * 100) + (id % 100);
    priorityHeap.insert(encodedValue);
}

Vehicle EmergencyQueue::extractNextVehicle() {
    int encoded = priorityHeap.extractMin();
    Vehicle v;

    // Decode
    v.priority = encoded / 10000;
    v.arrivalTime = (encoded % 10000) / 100;
    v.id = encoded % 100;

    return v;
}

bool EmergencyQueue::hasEmergency() const {
    if (priorityHeap.isEmpty()) return false;
    // If the smallest item in the heap has priority 0, an emergency is present
    return (priorityHeap.peekMin() / 10000) == 0;
}

bool EmergencyQueue::isEmpty() const { return priorityHeap.isEmpty(); }
int EmergencyQueue::getQueueLength() const { return priorityHeap.getSize(); }
