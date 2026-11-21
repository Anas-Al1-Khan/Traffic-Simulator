#ifndef EMERGENCY_QUEUE_H
#define EMERGENCY_QUEUE_H

#include "ManualMinHeap.h"

struct Vehicle {
    int id;
    int priority; // 0 = Emergency, 1 = Normal
    int arrivalTime;
};

class EmergencyQueue {
private:
    ManualMinHeap priorityHeap;
public:
    EmergencyQueue(int capacity = 20);
    void addVehicle(int id, bool isEmergency, int currentTime);
    Vehicle extractNextVehicle();
    bool hasEmergency() const;
    bool isEmpty() const;
    int getQueueLength() const;
};

#endif#pragma once
