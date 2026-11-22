#pragma once
#include "Graph.h"

// The shared interface we agreed on
struct Vehicle {
    int id;
    bool isEmergency;
    int startNode;
    int destNode;

    int path[10];       // The route (e.g., 0, 2, 3, 4)
    int pathLength;     // How many nodes in the route
    int currentIndex;   // Where the vehicle is currently on the path
    bool isFinished;    // Has it reached the destination?
};

class Simulation {
private:
    Graph* graph;
    Vehicle* vehicles;  // Dynamic array of vehicles
    int maxVehicles;
    int vehicleCount;

public:
    Simulation(Graph* g, int maxCapacity);
    ~Simulation();

    int getVehicleCount() const { return vehicleCount;  }
    bool isEmergencyVehicle(int index) const { return vehicles[index].isEmergency; }
    int getVehicleLocation(int index) const;
    void spawnVehicle(int start, int end, bool emergency);
    void advanceTime(); // Moves vehicles to their next node
    void printState() const; // For testing in the console
};
