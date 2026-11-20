#include "Simulation.h"
#include <iostream>

Simulation::Simulation(Graph* g, int maxCapacity) {
    graph = g;
    maxVehicles = maxCapacity;
    vehicles = new Vehicle[maxVehicles];
    vehicleCount = 0;
}

Simulation::~Simulation() {
    delete[] vehicles;
}

void Simulation::spawnVehicle(int start, int end, bool emergency) {
    if (vehicleCount >= maxVehicles) {
        std::cout << "Cannot spawn vehicle: Simulation full!\n";
        return;
    }

    Vehicle& v = vehicles[vehicleCount];
    v.id = vehicleCount + 1;
    v.isEmergency = emergency;
    v.startNode = start;
    v.destNode = end;
    v.isFinished = false;
    v.currentIndex = 0;

    int parent[10];
    graph->Dijkstra(start, end, parent);

    int tempPath[10];
    int length = 0;
    int curr = end;

    if (parent[end] == -1 && start != end) {
        std::cout << "Vehicle " << v.id << " cannot spawn: No path to destination.\n";
        return;
    }

    while (curr != -1) {
        tempPath[length++] = curr;
        curr = parent[curr];
    }

    v.pathLength = length;
    for (int i = 0; i < length; i++) {
        v.path[i] = tempPath[length - 1 - i];
    }

    vehicleCount++;
    std::cout << "Spawned Vehicle " << v.id << " (Emergency: " << (emergency ? "Yes" : "No") << ")\n";
}

void Simulation::advanceTime() {
    for (int i = 0; i < vehicleCount; i++) {
        Vehicle& v = vehicles[i];
        if (v.isFinished) continue;

        v.currentIndex++;

        if (v.currentIndex >= v.pathLength - 1) {
            v.isFinished = true;
            std::cout << "Vehicle " << v.id << " completed its trip.\n";
        }
    }
}

void Simulation::printState() const {
    std::cout << "\n--- Current Simulation State ---\n";
    for (int i = 0; i < vehicleCount; i++) {
        const Vehicle& v = vehicles[i];
        if (v.isFinished) continue;

        std::cout << "Vehicle " << v.id << " is at node " << v.path[v.currentIndex]
            << " heading to " << v.path[v.currentIndex + 1] << "\n";
    }
    std::cout << "--------------------------------\n";
}