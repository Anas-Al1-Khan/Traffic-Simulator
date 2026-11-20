#include <iostream>
#include "Graph.h"

using namespace std;

int main() {
    // 6 Intersections (0 to 5)
    Graph g(6);

    // Build a road network
    g.addRoad(0, 1, 4);
    g.addRoad(0, 2, 2);
    g.addRoad(1, 3, 5);
    g.addRoad(2, 3, 1); // Fast route from 0 to 3 is 0->2->3 (cost 3) vs 0->1->3 (cost 9)
    g.addRoad(3, 4, 3);

    // Node 5 is completely disconnected for edge-case testing
    // g.addRoad(4, 5, 2); 

    cout << "--- Graph Structure ---" << endl;
    g.printGraph();

    int* parent = new int[6];

    cout << "\n--- Test 1: Normal Path (0 to 4) ---" << endl;
    g.Dijkstra(0, 4, parent);
    g.printPath(0, 4, parent);
    // Expected: 0 -> 2 -> 3 -> 4

    cout << "\n--- Test 2: Edge Case - Unreachable Node (0 to 5) ---" << endl;
    g.Dijkstra(0, 5, parent);
    g.printPath(0, 5, parent);
    // Expected: No path exists from 0 to 5

    delete[] parent;
    return 0;
}