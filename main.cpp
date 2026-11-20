#include <iostream>
#include "Graph.h"
#include "Simulation.h"

using namespace std;

int main() {
    // 1. Setup the Map
    Graph map(6);
    map.addRoad(0, 1, 4);
    map.addRoad(0, 2, 2);
    map.addRoad(1, 3, 5);
    map.addRoad(2, 3, 1);
    map.addRoad(3, 4, 3);
    map.addRoad(4, 5, 2);

    // 2. Setup the Simulation (Max 10 vehicles)
    Simulation sim(&map, 10);

    // 3. Spawn Vehicles
    sim.spawnVehicle(0, 4, false); // Normal car
    sim.spawnVehicle(0, 5, true);  // Ambulance (Emergency)

    // 4. Simulate Time (Ticks)
    cout << "\n--- Tick 1 ---" << endl;
    sim.advanceTime();
    sim.printState();

    cout << "\n--- Tick 2 ---" << endl;
    sim.advanceTime();
    sim.printState();

    cout << "\n--- Tick 3 ---" << endl;
    sim.advanceTime();
    sim.printState();

    cout << "\n--- Tick 4 ---" << endl;
    sim.advanceTime();
    sim.printState();

    return 0;
}