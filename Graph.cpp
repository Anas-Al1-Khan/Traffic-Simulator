#include<iostream>
#include"Graph.h"
using namespace std;

Road::Road(int destId, int weight) {
	destinationId = destId;
	this->weight = weight;
	next = nullptr;
}

Graph::Graph(int n) {
	intersections = n;
	adjList = new Road*[n];
	for (int i = 0; i < n; i++)
	{
		adjList[i] = nullptr;
	}
}

Graph::~Graph() {
	for (int i = 0; i < intersections; i++) {
		Road* current = adjList[i];
		while (current) {
			Road* temp = current;
			current = current->next;
			delete temp; // Prevent memory leaks (Instructor Rule)
		}
	}
	delete[] adjList;
}

void Graph::addRoad(int src, int dest, int weight) {
	// Using an undirected graph : Add a Road in Both directions
	Road* a = new Road(dest, weight);
	a->next = adjList[src];
	adjList[src] = a;

	Road* b = new Road(src, weight);
	b->next = adjList[dest];
	adjList[dest] = b;
}

void Graph::printGraph() const {
	for (int i = 0; i < intersections; i++)
	{
		cout << " Intersection " << i << " : ";
		Road* current = adjList[i];
		while (current) {
			cout << " -> " << current->destinationId << " ( w : " << current->weight << " ) ";
			current = current->next;
		}
		cout << endl;
	}
}

void Graph::Dijkstra(int start, int end, int* parent) {
	// 1. Initialize distance and Visited Arrays

	int* dist = new int[intersections];
	bool* visited = new bool[intersections];
	
	for (int i = 0; i < intersections; i++)
	{
		dist[i] = INT_MAX;									// Infinity
		visited[i] = false;									
		parent[i] = -1;										// -1 means no path
	}

	dist[start] = 0;

	// 2. Find the shortest path for all vertices

	for (int count = 0; count < intersections - 1; count++)
	{
		// Find the unvisited vertex with minimum distance

		int minDist = INT_MAX;
		int u = -1;
		for (int i = 0; i < intersections; i++)
		{
			if (!visited[i] && dist[i] <= minDist) {
				minDist = dist[i];
				u = i;
			}
		}

		// If we can't reach any ore nodes then break

		if (u == -1 || dist[u] == INT_MAX) break;

		// If we reached our destination, stop early
		if (u == end) break;

		// 3. Relax neighbors of picked vertex

		Road* current = adjList[u];
		while (current != nullptr) {
			int v = current->destinationId;
			int weight = current->weight;

			// If not visited and new path is shorter

			if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
				dist[v] = dist[u] + weight;
				parent[v] = u; // Recording the path
			}

			current = current->next;
		}

	}

	delete[] dist;
	delete[] visited;

}

void Graph::printPath(int start, int end, int* parent) {
	if (parent[end] == -1 && start != end) {
		std::cout << "No path exists from " << start << " to " << end << "\n";
		return;
	}

	// Reconstruct path using a simple array stack (No STL!)
	int* path = new int[intersections];
	int pathLength = 0;

	int current = end;
	while (current != -1) {
		path[pathLength++] = current;
		current = parent[current];
	}

	std::cout << "Shortest Path: ";
	for (int i = pathLength - 1; i >= 0; i--) {
		std::cout << path[i];
		if (i > 0) std::cout << " -> ";
	}
	std::cout << "\n";

	delete[] path;
}