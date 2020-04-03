#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <algorithm>
#include <queue>

enum Colors { Black, White };

class Vertex {
public:
	char name;
	double heuristic;
	double pathLength;
	double cost;
	Colors color;
	Vertex() : color(Colors::Black) {}
	Vertex(char a) : name(a), color(Colors::Black) {}

	bool operator< (const Vertex &v) const {
		return this->name < v.name;
	}

	void evaluateCost() {
		cost = pathLength + heuristic;
	}

};

typedef struct Edge {
	Vertex start;
	Vertex end;
	double weigth;

	Edge() {}

	Edge(Vertex start, Vertex end, double weigth) : start(start), end(end), weigth(weigth) {}

	bool operator< (const Edge &e) const {
		return this->weigth < e.weigth;
	}

}Edge;

struct edgeComp_Q {
	bool operator() (Edge *e1, Edge *e2) {
		if (e1->end.cost == e2->end.cost) {
			return e1->end.heuristic > e2->end.heuristic;
		}
		return (e1->end.cost > e2->end.cost);
	}
};

bool edgeComp_V(const Edge &e1, const Edge &e2) {
	if (e1.weigth == e2.weigth) {
		return e1.end.name < e2.end.name;
	}
	return e1.weigth < e2.weigth;
}

class Graph {
private:
	std::map<Vertex, std::vector<Edge>>::iterator graphIt;
	std::vector<Edge>::iterator edgeIt;
	std::map<Vertex, std::vector<Edge>> graph;
	std::map<char, double> heuristics;

	std::priority_queue<Edge*, std::vector<Edge*>, edgeComp_Q> edgeQueue;
	std::map<char, char> path;
	std::string pathName;

	Vertex start;
	Vertex end;

	void generatePath();

public:
	Graph() {
		std::cin >> start.name;
		std::cin >> end.name;
		Vertex v1;
		Vertex v2;
		double weigth;
		while (std::cin >> v1.name && std::cin >> v2.name && std::cin >> weigth) {
			graph[v1].push_back(Edge(v1, v2, weigth));
			graph[v2];
		}
	}

	void greedPath();
	void aStarPath();
};

void Graph::generatePath() {
	pathName.clear();
	char currentSymbol;
	currentSymbol = end.name;

	while (currentSymbol != start.name) {
		pathName += currentSymbol;
		currentSymbol = path[currentSymbol];
	}

	pathName += currentSymbol;

	std::reverse(pathName.begin(), pathName.end());

	std::cout << pathName;
}

void Graph::greedPath() {

	for (auto &node : graph) {
		std::sort(node.second.begin(), node.second.end(), edgeComp_V);
	}

	Edge current;
	current.end = start;

	for (;;) {

		if (current.end.name == end.name) {
			generatePath();
			return;
		}

		for (edgeIt = graph[current.end].begin(); edgeIt < graph[current.end].end(); edgeIt++) {
			if (edgeIt->end.color == Colors::Black) {
				edgeIt->end.color = Colors::White;
				current.start = current.end;
				current.end = edgeIt->end;
				path[current.end.name] = current.start.name;
				break;
			}
		}

		if (edgeIt == graph[current.end].end()) {
			current.end = current.start;
		}
	}
}

void Graph::aStarPath() {

	std::map<char, Colors> visitedVerts;
	Edge *current;

	for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
		heuristics[graphIt->first.name] = abs(graphIt->first.name - end.name);
		visitedVerts[graphIt->first.name] = Colors::Black;
	}
	visitedVerts[start.name] = Colors::White;

	for (auto &edge : graph[start]) {
		edge.end.heuristic = heuristics[edge.end.name];
		edge.end.pathLength = edge.weigth;
		edge.end.evaluateCost();
		edgeQueue.push(&edge);
	}

	while (!edgeQueue.empty()) {
		current = edgeQueue.top();
		edgeQueue.pop();
		if (visitedVerts[current->end.name] == Colors::White) {
			continue;
		}
		visitedVerts[current->end.name] = Colors::White;
		path[current->end.name] = current->start.name;

		if (current->end.name == end.name) {
			generatePath();
			return;
		}

		for (auto &edge : graph[current->end.name]) {
			if (visitedVerts[edge.end.name] == Colors::Black) {
				edge.end.heuristic = heuristics[edge.end.name];
				edge.end.pathLength = current->end.pathLength + edge.weigth;
				edge.end.evaluateCost();
				edgeQueue.push(&edge);
			}
		}
	}
}

int main() {
	Graph graph;
	//graph.greedPath();
	graph.aStarPath();
	return 0;
}