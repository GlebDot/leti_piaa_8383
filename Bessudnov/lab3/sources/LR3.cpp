#include "pch.h"
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>

//перечисление цветов
enum Colors { White, Black };

//класс ребра
class Edge {
public:
	char start; //начальная вершина
	char end; //конечная вершина
	int flow; //поток через ребро
	int capacity; //пропускная способность 
	int residualCapacity; //остаточная пропускная способность
	Colors color; //цвет ребра

	Edge(char start, char end, int capacity) : start(start), end(end), capacity(capacity), residualCapacity(capacity) {
		color = Colors::Black;
		flow = 0;
	}

	void evaluateResidualCapacity() {
		residualCapacity = capacity - flow;
	}


};

//компоратор для сортировки ребер по остаточной пропускной способности
bool edgeComp(Edge* e1, Edge* e2) {
	return e1->residualCapacity > e2->residualCapacity;
}

//компоратор для сортировки ребер по имени
bool edgeCompOut(Edge* e1, Edge* e2) {
	return e1->end < e2->end;
}

//класс графа
class Graph {
private:
	std::map<char, std::vector<Edge*>> graph; //граф
	std::map<char, std::vector<Edge*>>::iterator graphIt; //итератор графа
public:
	//получение ребра по двум точкам
	Edge* operator()(char start, char end) {
		for (auto& edge : graph[start]) {
			if (edge->end == end) {
				return edge;
			}
		}

		return nullptr;
	}

	//нахождение ребра с максимальной остаточной пропускной способностью
	Edge* findMaxEdge(char start);
	
	//функция добавления ребра в граф
	void pushEdge(Edge* edge) {
		graph[edge->start].push_back(edge);
		graph[edge->end];
	}

	//высчитывание максимального потока
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

	//перекрасить граф в черный цвет
	void recolorize() {
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			for (auto &edge : graphIt->second) {
				edge->color = Colors::Black;
			}
		}
	}

	//печать графа
	void printGraph() {
		std::cout << "Residual Network:\n" << std::endl;
		std::cout << std::setw(7) << "Name" << std::setw(7) << "Flow" << std::setw(10)
			<< "Capacity" << std::setw(10) << "Residual" << std::endl;
		for (graphIt = graph.begin(); graphIt != graph.end(); graphIt++) {
			std::sort(graphIt->second.begin(), graphIt->second.end(), edgeCompOut);
			for (auto &edge : graphIt->second) {
				if (edge->capacity > 0) {
					std::cout << edge->start << "<--->" << edge->end;
					std::cout << std::setw(7) << ((edge->flow > 0) ? edge->flow : 0);
					std::cout << std::setw(10) << edge->capacity;
					std::cout << std::setw(10) << edge->residualCapacity << std::endl;
				}
			}
		}
		std::cout << "\n<--------------------------->\n\n" << std::endl;
	}
};

Edge* Graph::findMaxEdge(char start) {
	if (graph[start].empty()) {
		return nullptr;
	}

	//сортировка ребер, исходящих из данной вершины
	std::sort(graph[start].begin(), graph[start].end(), edgeComp);

	for (auto &edge : graph[start]) {
		if (edge->color == Colors::Black && edge->residualCapacity > 0) {
			return edge;
		}
	}

	return nullptr;
}

//класс для нахождения пути 
class PathFinder {
private:
	std::vector<std::pair<char, char>> currentPath; //путь
	std::vector<std::pair<char, char>>::iterator pathIt; //итератор пути
	int minCapacity; //минимальная остаточная пропускная способность пути

	//воссоздание пути
	void constructPath(std::stack<Edge*> edgeStack, char start, char end);
	void printPath();
	//нахождение минимальной остаточной пропускной способности в пути
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

	//функция нахождения пути
	void findPath(Graph &graph, char start, char end);

	bool isPath() {
		return !currentPath.empty();
	}
};

void PathFinder::printPath() {
	std::cout << "Current path:" << std::endl;
	for (pathIt = currentPath.end() -1; pathIt != currentPath.begin(); pathIt--) {
		std::cout << pathIt->first << "<--->" << pathIt->second << std::endl;
	}
	std::cout << pathIt->first << "<--->" << pathIt->second << std::endl;
	std::cout << "Minimum residual capacity: " << minCapacity << "\n\n" << std::endl;
}

void PathFinder::constructPath(std::stack<Edge*> edgeStack, char start, char end) {
	char symb;
	symb = end;
	Edge *currentEdge;
	while (!edgeStack.empty()) {
		currentEdge = edgeStack.top();
		edgeStack.pop();
		currentPath.push_back(std::pair<char, char>(currentEdge->start, currentEdge->end));
	}
	printPath();
}

//клас сети
class Network {
private:
	PathFinder pathFinder; //объект для нахождения путей
	Graph residualNetwork; //остаточная сеть
	int maxFlow; //максимальный поток
	char source; //исток
	char sink; //сток

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
	//функция нахождения максимального потока
	void findMaxFlow();
};

void Network::printNetwork() {
	maxFlow = residualNetwork.maxFlow(sink);
	std::cout << "<_____________ANSWER_________________>" << std::endl;
	std::cout << "Max flow: " << maxFlow << "\n" << std::endl;
	residualNetwork.printGraph();
}

//нахождение пути в графе
void PathFinder::findPath(Graph &graph, char start, char end) {
	//очистка предыдущего пути и перекраска графа
	currentPath.clear();
	graph.recolorize();

	//стэк пути
	std::stack<Edge*> edgeStack;

	//кладем в стэк начальное значение для запуска алгоритма
	Edge *currentEdge = graph.findMaxEdge(start);
	if (currentEdge == nullptr) {
		currentPath.clear();
		return;
	}

	graph(currentEdge->start, currentEdge->end)->color = Colors::White;
	edgeStack.push(currentEdge);

	//если попложенное ребро является концом, то выходим
	if (currentEdge->end == end) {
		setMinCapacity(edgeStack);
		constructPath(edgeStack, start, end);
		return;
	}

	for (;;) {
		currentEdge = graph.findMaxEdge(edgeStack.top()->end);
		//если не смогли найти ребро из данной вершины
		if (currentEdge == nullptr) {
			edgeStack.pop();
			//если стэк опустел, то пути нет
			if (edgeStack.empty()) {
				currentPath.clear();
				return;
			}
		} else {
			//ребро было найдено
			graph(currentEdge->start, currentEdge->end)->color = Colors::White;

			//если дошли до конца, то выходим
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

//поиск максимального потока
void Network::findMaxFlow() {
	for (;;) {
		//ищем путь
		pathFinder.findPath(residualNetwork, source, sink);

		//если путь не был найден, то цикл заканчивается
		if (!pathFinder.isPath()) {
			printNetwork();
			return;
		}

		//если путь был найден, то изменяем поток и остаточную пропускную способность у ребер в пути
		for (auto& edge : pathFinder.getPath()) {
			residualNetwork(edge.first, edge.second)->flow += pathFinder.getMinCapacity();
			residualNetwork(edge.first, edge.second)->evaluateResidualCapacity();

			residualNetwork(edge.second, edge.first)->flow -= pathFinder.getMinCapacity();
			residualNetwork(edge.second, edge.first)->evaluateResidualCapacity();

		}

		residualNetwork.printGraph();
	}


}

int main() {
	Network network;
	network.findMaxFlow();
}
