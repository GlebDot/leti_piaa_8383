#include "pch.h"
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <algorithm>
#include <queue>
#include <windows.h>

//перечисление цветов
enum Colors { Black, White };

//класс вершины
class Vertex {  
public:
	char name; //имя точки
	double heuristic; //эврестическое значение точки
	double pathLength; //длина пути до точки
	double cost; //общая стоимость перехода в точку
	Colors color; //цвет точки
	Vertex() : color(Colors::Black) {}
	Vertex(char a) : name(a), color(Colors::Black) {}

	bool operator< (const Vertex &v) const {
		return this->name < v.name;
	}

	void evaluateCost() {
		cost = pathLength + heuristic;
	}

};

//класс ребра
typedef struct Edge {
	Vertex start; //начальная точка
	Vertex end; //конечная точка
	double weigth; //вес

	Edge() {}

	Edge(Vertex start, Vertex end, double weigth) : start(start), end(end), weigth(weigth) {}

	bool operator< (const Edge &e) const {
		return this->weigth < e.weigth;
	}
	
}Edge;

//компаратор для очереди ребер
struct edgeComp_Q {
	bool operator() (Edge *e1, Edge *e2) {
		if (e1->end.cost == e2->end.cost) {
			return e1->end.heuristic > e2->end.heuristic;
		}
		return (e1->end.cost > e2->end.cost);
	}
};

//компаратор для вектора ребер
bool edgeComp_V(const Edge &e1, const Edge &e2){
	if (e1.weigth == e2.weigth) {
		return e1.end.name < e2.end.name;
	}
	return e1.weigth < e2.weigth;
}

//класс графа
class Graph {
private:
	std::map<Vertex, std::vector<Edge>>::iterator graphIt; //итератор для графа
	std::map<char, double>::iterator heuristicsIt; //итератор для эврестической карты
	std::vector<Edge>::iterator edgeIt; // итератор для вектора ребер

	std::map<Vertex, std::vector<Edge>> graph; //граф
	std::map<char, double> heuristics; //карта эвристик

	std::priority_queue<Edge*, std::vector<Edge*>, edgeComp_Q> edgeQueue; //очередь с приоритетом
	std::map<char, char> path; //карта для заполнения пути
	std::string pathName; //строка с путем

	Vertex start; //начальная вершина
	Vertex end; //конечная вершина

	HANDLE hConsole; //управление консолью

	void printHeuristics() {
		std::cout << "HEURISTICS\ndot:\tvalue:" << std::endl;
		for (heuristicsIt = heuristics.begin(); heuristicsIt != heuristics.end(); heuristicsIt++) {
			std::cout << heuristicsIt->first << "\t" << heuristicsIt->second << std::endl;
		}

		std::cout << "\n" << std::endl;
	}

	void printGreedMessage() {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | 0);
		std::cout << "GREED ALGORITHM INFORMATION\n\n" << std::endl;
		SetConsoleTextAttribute(hConsole, 7 | 0);
	}

	void printGreedIteration(Edge &current) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | 0);
		std::cout << "Current edge: " << current.start.name << "<--->" << current.end.name << std::endl;
		SetConsoleTextAttribute(hConsole, 7 | 0);

		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			for (auto &edge : graphIt->second) {
				if (edge.end.name == current.end.name && edge.start.name == current.start.name) {
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | 0);
					std::cout << edge.start.name << "<--->" << edge.end.name << " (" << edge.weigth << ")" << std::endl;
					SetConsoleTextAttribute(hConsole, 7 | 0);
				} else {
					std::cout << edge.start.name << "<--->" << edge.end.name << " (" << edge.weigth << ")" << std::endl;
				}
			}
		}

		std::cout << "<------------------------------------------------------------->\n" << std::endl;
	}

	void printAStarMessage() {
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | 0);
		std::cout << "A STAR ALGORITHM INFORMATION\n\n" << std::endl;
		SetConsoleTextAttribute(hConsole, 7 | 0);
	}

	void printAStarIteration(std::priority_queue<Edge*, std::vector<Edge*>, edgeComp_Q> queue, Edge current) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | 0);
		std::cout << "Current edge: " << current.start.name << "<--->" << current.end.name << " (" << current.weigth << ") "
			<< "\tdot: " << current.end.name << " (cost: " << current.end.cost << ", pathLength: "
			<< current.end.pathLength << ")" << std::endl;
		SetConsoleTextAttribute(hConsole, 7 | 0);

		std::cout << "QUEUE:" << std::endl;
		Edge *cur;
		while (!queue.empty()) {
			cur = queue.top();
			queue.pop();
			std::cout << "Current edge: " << cur->start.name << "<--->" << cur->end.name << " (" << cur->weigth << ") "
				<< "\tdot: " << cur->end.name << " (cost: " << cur->end.cost << ", pathLength: "
				<< cur->end.pathLength << ")" << std::endl;
		}
		std::cout << "\n" << std::endl;

		std::cout << "GRAPH:" << std::endl;
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			for (auto &edge : graphIt->second) {
				if (edge.end.name == current.end.name && edge.start.name == current.start.name) {
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | 0);
					std::cout << edge.start.name << "<--->" << edge.end.name << std::endl;
					SetConsoleTextAttribute(hConsole, 7 | 0);
				} else {
					std::cout << edge.start.name << "<--->" << edge.end.name << std::endl;
				}
			}
		}

		std::cout << "<------------------------------------------------------------->\n" << std::endl;
	}

	void generatePath();
	void autofillHeuristics() {
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			heuristics[graphIt->first.name] = abs(graphIt->first.name - end.name);
		}

		printHeuristics();
	}

	void handfillHeuristics() {
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			std::cout << "Enter heuristic for " << graphIt->first.name << ": ";
			std::cin.clear();
			std::cin >> heuristics[graphIt->first.name];
		}

		double minHeuristic;
		minHeuristic = heuristics.begin()->second;
		for (heuristicsIt = heuristics.begin(); heuristicsIt != heuristics.end(); heuristicsIt++) {
			if (heuristicsIt->second < minHeuristic) {
				minHeuristic = heuristicsIt->second;
			}
		}

		for (heuristicsIt = heuristics.begin(); heuristicsIt != heuristics.end(); heuristicsIt++) {
			heuristicsIt->second += minHeuristic;
		}

		printHeuristics();
	}

public:
	Graph() {
		
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::cout << "Enter start: ";
		std::cin >> start.name;
		std::cout << "Enter end: ";
		std::cin >> end.name;
		Vertex v1;
		Vertex v2;
		double weigth;
		std::cout << "Enter edges (from, to, weigth)\nTo end input press *" << std::endl;
		while (true) {
			std::cin >> v1.name;
			if (v1.name == '*') break;
			std::cin >> v2.name >> weigth;
			graph[v1].push_back(Edge(v1, v2, weigth));
			graph[v2];
		}

		handfillHeuristics();

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

	std::cout << pathName << std::endl;
}

void Graph::greedPath() {

	printGreedMessage();

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

		printGreedIteration(current);
	}
}

void Graph::aStarPath() {

	printAStarMessage();

	std::map<char, Colors> visitedVerts;
	Edge *current;


	for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
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
		printAStarIteration(edgeQueue, *current);

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
	graph.greedPath();
	graph.aStarPath();
	return 0;
}