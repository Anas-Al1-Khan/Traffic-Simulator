#pragma once
#include <iostream>
#include <climits>

struct Road {
    int destinationId;
    int weight;
    Road* next;

    // Constructor
    Road(int d, int w) {
        destinationId = d;
        weight = w;
        next = nullptr;
    }
};

class Graph {
private:
    int intersections;
    Road** adjList;

public:
    Graph(int n);
    ~Graph();

    void addRoad(int src, int dest, int weight);
    void closeRoad(int src, int dest);
    void printGraph() const;
    int getNumIntersections() const { return intersections; }

    void Dijkstra(int start, int end, int* parent);
    void printPath(int start, int end, int* parent);
};