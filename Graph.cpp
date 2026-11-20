#include "Graph.h"
#include <climits>

Graph::Graph(int n) {
    intersections = n;
    adjList = new Road * [n];
    for (int i = 0; i < n; i++) adjList[i] = nullptr;
}

Graph::~Graph() {
    for (int i = 0; i < intersections; i++) {
        Road* curr = adjList[i];
        while (curr) {
            Road* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete[] adjList;
}

void Graph::addRoad(int src, int dest, int weight) {
    Road* a = new Road(dest, weight);
    a->next = adjList[src];
    adjList[src] = a;

    Road* b = new Road(src, weight);
    b->next = adjList[dest];
    adjList[dest] = b;
}

void Graph::printGraph() const {
    for (int i = 0; i < intersections; i++) {
        std::cout << "Intersection " << i << " : ";
        Road* curr = adjList[i];
        while (curr) {
            std::cout << " -> " << curr->destinationId << " ( w : " << curr->weight << " ) ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
}

void Graph::Dijkstra(int start, int end, int* parent) {
    int* dist = new int[intersections];
    bool* visited = new bool[intersections];

    for (int i = 0; i < intersections; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1; // -1 means no path yet
    }

    dist[start] = 0;

    for (int count = 0; count < intersections - 1; count++) {
        int minDist = INT_MAX;
        int u = -1;
        for (int i = 0; i < intersections; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        Road* curr = adjList[u];
        while (curr != nullptr) {
            int v = curr->destinationId;
            int weight = curr->weight;

            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Record the path
            }
            curr = curr->next;
        }
    }

    delete[] dist;
    delete[] visited;
}