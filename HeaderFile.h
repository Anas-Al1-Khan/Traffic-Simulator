#pragma once

class Node {
public:
	int value;
	Node* next;

	Node(int n) : value(n), next(nullptr) {}
};