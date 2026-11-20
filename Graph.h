#pragma once
#include <iostream>
#include <climits>

class Road {												// Apparently an edge of a graph (Road)
public:

	int destinationId;
	int weight;
	Road* next;

	Road(int destinationId, int weight);
};

class Graph {
private:
	int intersections;
	Road** adjList;
public:
	Graph(int n);
	~Graph();

	void addRoad(int src, int dest, int weight);
	void printGraph() const;
	int getIntersections() const { return intersections; }
	void Dijkstra(int start, int end, int* parent);
	void printPath(int start, int end, int* parent);	
};

