#include <iostream>
#include "Graph.h"
#include "Simulation.h"

using namespace std;

int main() {
    Graph map(6);
    map.addRoad(0, 1, 4);
    map.addRoad(0, 2, 2);
    map.addRoad(1, 3, 5);
    map.addRoad(2, 3, 1);
    map.addRoad(3, 4, 3);
    map.addRoad(4, 5, 2);

    cout << "--- 1. Map Structure ---" << endl;
    map.printGraph();
    cout << "------------------------\n" << endl;

    Simulation sim(&map, 10);

    cout << "--- 2. Spawning Vehicles ---" << endl;
    sim.spawnVehicle(0, 4, false); // Normal car from 0 to 4
    sim.spawnVehicle(0, 5, true);  // Ambulance from 0 to 5

    cout << "\n--- 3. Simulation Ticks ---" << endl;
    for (int i = 1; i <= 4; i++) {
        cout << "Tick " << i << ":" << endl;
        sim.advanceTime();
        sim.printState();
        cout << endl;
    }

    return 0;
}