#include "pch.h"
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

enum Colors { White, Black };

class Edge {
public:
	char start;
	char end;
	int flow;
	int capacity;
	int residualCapacity;
	Colors color;

	Edge(char start, char end, int capacity) : start(start), end(end), capacity(capacity), residualCapacity(capacity) {
		color = Colors::Black;
		flow = 0;
	}

	void evaluateResidualCapacity() {
		residualCapacity = capacity - flow;
	}

	
};

bool edgeComp(Edge* e1, Edge* e2) {
	return e1->residualCapacity > e2->residualCapacity;
}

bool edgeCompOut(Edge* e1, Edge* e2) {
	return e1->end < e2->end;
}

class Graph {
private:
	std::map<char, std::vector<Edge*>> graph;
	std::map<char, std::vector<Edge*>>::iterator graphIt;
public:
	Edge* operator()(char start, char end) {
		for (auto& edge : graph[start]) {
			if (edge->end == end) {
				return edge;
			}
		}

		return nullptr;
	}

	Edge* findMaxEdge(char start);

	void pushEdge(Edge* edge) {
		graph[edge->start].push_back(edge);
		graph[edge->end];
	}

	int maxFlow(char sink) {
		int maxFlow = 0;
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			for (auto &edge : graphIt->second) {
				if (edge->end == sink) {
					maxFlow += edge->flow;
				}
			}
		}

		return maxFlow;
	}

	void recolorize() {
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			for (auto &edge : graphIt->second) {
				edge->color = Colors::Black;
			}
		}
	}

	void printGraph() {
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			std::sort(graphIt->second.begin(), graphIt->second.end(), edgeCompOut);
			for (auto &edge : graphIt->second) {
				if (edge->capacity > 0) {
					std::cout << edge->start << " " << edge->end << " " << ((edge->flow > 0) ? edge->flow : 0) << std::endl;
				}
			}
		}
	}
};

Edge* Graph::findMaxEdge(char start) {
	if (graph[start].empty()) {
		return nullptr;
	}

	std::sort(graph[start].begin(), graph[start].end(), edgeComp);

	for (auto &edge : graph[start]) {
		if (edge->color == Colors::Black && edge->residualCapacity > 0) {
			return edge;
		}
	}

	return nullptr;
}

class PathFinder {
private:
	std::vector<std::pair<char, char>> currentPath;
	int minCapacity;

	void constructPath(std::stack<Edge*> edgeStack, char start, char end);
	void setMinCapacity(std::stack<Edge*> edgeStack) {
		minCapacity = edgeStack.top()->residualCapacity;
		while (!edgeStack.empty()) {
			if (edgeStack.top()->residualCapacity < minCapacity) {
				minCapacity = edgeStack.top()->residualCapacity;
			}

			edgeStack.pop();
		}
	}

public:
	std::vector<std::pair<char, char>> getPath() {
		return currentPath;
	}
	
	int getMinCapacity() {
		return minCapacity;
	}

	void findPath(Graph &graph, char start, char end);

	bool isPath() {
		return !currentPath.empty();
	}
};

void PathFinder::constructPath(std::stack<Edge*> edgeStack, char start, char end) {
	char symb;
	symb = end;
	Edge *currentEdge;
	while (!edgeStack.empty()) {
		currentEdge = edgeStack.top();
		edgeStack.pop();
		currentPath.push_back(std::pair<char, char>(currentEdge->start, currentEdge->end));
	}
}

class Network {
private:
	PathFinder pathFinder;
	Graph residualNetwork;
	int maxFlow;
	char source;
	char sink;

public:
	Network() : maxFlow(0) {
		int edgesAmount;
		std::cin >> edgesAmount;
		std::cin >> source;
		std::cin >> sink;

		char v1;
		char v2;
		int capacity;

		for (int i = 0; i < edgesAmount; i++) {
			std::cin >> v1 >> v2 >> capacity;
			residualNetwork.pushEdge(new Edge(v1, v2, capacity));
			residualNetwork.pushEdge(new Edge(v2, v1, 0));
		}
	}

	void printNetwork();
	void findMaxFlow();
};

void Network::printNetwork() {
	maxFlow = residualNetwork.maxFlow(sink);
	std::cout << maxFlow << std::endl;
	residualNetwork.printGraph();
}

void PathFinder::findPath(Graph &graph, char start, char end) {
	currentPath.clear();
	graph.recolorize();

	std::stack<Edge*> edgeStack;

	Edge *currentEdge = graph.findMaxEdge(start);
	if (currentEdge == nullptr) {
		currentPath.clear();
		return;
	}

	graph(currentEdge->start, currentEdge->end)->color = Colors::White;
	edgeStack.push(currentEdge);

	if (currentEdge->end == end) {
		setMinCapacity(edgeStack);
		constructPath(edgeStack, start, end);
		return;
	}

	for (;;) {
		currentEdge = graph.findMaxEdge(edgeStack.top()->end);
		if (currentEdge == nullptr) {
			edgeStack.pop();
			if (edgeStack.empty()) {
				currentPath.clear();
				return;
			}
		} else {
			graph(currentEdge->start, currentEdge->end)->color = Colors::White;
			
			if (currentEdge->end == end) {
				edgeStack.push(currentEdge);
				setMinCapacity(edgeStack);
				constructPath(edgeStack, start, end);
				return;
			}

			edgeStack.push(currentEdge);
		}
	}

}

void Network::findMaxFlow() {
	for (;;) {
		pathFinder.findPath(residualNetwork, source, sink);

		if (!pathFinder.isPath()) {
			printNetwork();
			return;
		}

		for (auto& edge : pathFinder.getPath()) {
			residualNetwork(edge.first, edge.second)->flow += pathFinder.getMinCapacity();
			residualNetwork(edge.first, edge.second)->evaluateResidualCapacity();

			residualNetwork(edge.second, edge.first)->flow -= pathFinder.getMinCapacity();
			residualNetwork(edge.second, edge.first)->evaluateResidualCapacity();

		}
	}


}

int main() {
	Network network;
	network.findMaxFlow();
}
