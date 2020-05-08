#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include<iomanip>



struct Node{
    char symbol; //символ
    int patternEnd; //номер образца
    int patternOffset; //длина образца
    int sufLink; //суффикс ссфылка
    int parent; //родитель
    std::map<char, int> moves; //ходы из вершины  
    std::map<char, int> children; //дети вершины

    Node() : symbol('0'), patternEnd(0), patternOffset(0), sufLink(-1), parent(-1) {
    }
    Node(char symb, int link, int parent) : symbol(symb), patternEnd(0), 
    patternOffset(0), sufLink(link), parent(parent) {
    }
};

class Bor {
private:
    std::vector<Node> patternsBor; //бор
    std::vector<std::pair<int, int>> answers; //ответы
    int borStepPos; //текущая вершина

    int maxSufLink; //макимальная длина суффикс-ссылки
    int maxEndLink; //максимальная длина конечной ссылки

    int step(int node, char symb); //функция перехода
    int getLink(int node); //функция получения ссылки
    void nodeCheck(int index); //функция проверки вершины
    void printAnswers(); //функция печати ответов
public:
    Bor();
    ~Bor()=default;
    void printBor(); //печать бора
    void addStringToBor(const std::string &str, int index);//добавить строку в бор
    void findPatterns(const std::string &text);//найти все вхождения
};

//компаратор для ответов
bool answerComp(const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
    if (p1.first == p2.first) {
        return p1.second < p2.second;
    }

    return p1.first < p2.first;
}

int main() {
    Bor bor;
    std::string text;
    std::string currentPattern;
    int patternsNumber;

    std::cin >> text;
    std::cin >> patternsNumber;
    for(int i = 0; i < patternsNumber; i++) {
        std::cin >> currentPattern;
        bor.addStringToBor(currentPattern, i + 1);
    }

    bor.findPatterns(text);
    bor.printBor();

    return 0;
}



Bor::Bor() : borStepPos(0), maxEndLink(0), maxSufLink(0) {
    patternsBor.push_back(Node('0', 0, 0));
}

//печать бора
void Bor::printBor() {
    std::cout << "--------Bor--------" << std::endl;
    std::cout << std::setw(8) << "Symbol";
    std::cout << std::setw(8) << "Parent";
    std::cout << std::setw(8) << "Link";
    std::cout << std::setw(8) << "Sons" << std::endl;
    for (auto &node : patternsBor) {
        std::cout << std::setw(8) << node.symbol;
        std::cout << std::setw(8) << node.parent;
        std::cout << std::setw(8) << node.sufLink;
        std::cout << std::setw(4);

        std::map<char, int>::iterator mapIt;
        
        for(mapIt = node.children.begin(); mapIt != node.children.end(); mapIt++) {
            std::cout << "(" << mapIt->first << ", " << mapIt->second << ") ";
        }

        std::cout << std::endl;
    }
}

//печать ответов
void Bor::printAnswers() {
    std::sort(answers.begin(), answers.end(), answerComp);

    std::cout << "-------ANSWERS-------" << std::endl;
    std::cout << std::setw(6) << "Pos" << std::setw(6) << "PNum" << std::endl;
    for (auto &answer : answers) {
        std::cout << std::setw(6) << answer.first << std::setw(6) << answer.second << std::endl;
    }
    std::cout << "Max suf link: " << maxSufLink << std::endl;
    std::cout << "Max end link: " << maxEndLink << std::endl;
}


void Bor::addStringToBor(const std::string &str, int index) {
    int curPos;
    curPos = 0;
    for(int i = 0; i < str.length(); i++) {
        if(patternsBor[curPos].children[str[i]] != 0) {
            //если переход по символу возможен
            curPos = patternsBor[curPos].children[str[i]];
        } else {
            //если переход невозможен, создаем новую вершину
            patternsBor.push_back(Node(str[i], -1, curPos));
            patternsBor[curPos].children[str[i]] = patternsBor.size() - 1;
            curPos = patternsBor.size() - 1;
        }
    }
    //установить номер образца
    patternsBor[curPos].patternEnd = index; 
    //установить длину образца
    patternsBor[curPos].patternOffset = str.length();
}

void Bor::findPatterns(const std::string &text) {
    borStepPos = 0;

    for (int i = 0; i < text.length(); i++) {
        std::cout << "<----------------->" << std::endl;
        std::cout << "Making step by symbol: " << text[i] << std::endl;
        //делаем шаг
        borStepPos = step(borStepPos, text[i]);
        std::cout << std::endl;
        //делаем проверку
        nodeCheck(i);
        
        std::cout << "<----------------->" << std::endl;
        std::cout << std::endl;
    }
    //печатаем ответы
    printAnswers();
}

int Bor::getLink(int node) {
    //если ссылки нету
    if (patternsBor[node].sufLink == -1) {
        std::cout << "Link from (" << patternsBor[node].symbol << ", " << node << ") is not set" << std::endl;
        //если находимся в корне, или корень - это родитель
        if (node == 0 || patternsBor[node].parent == 0) {
            std::cout << "In the root, or parent is root" << std::endl;
            patternsBor[node].sufLink = 0;
        //иначе делаем шаг из суффикс ссылки родителя
        } else {
            patternsBor[node].sufLink = step(getLink(patternsBor[node].parent), patternsBor[node].symbol);
        }
    }

    std::cout << "Link from (" << patternsBor[node].symbol << ", " << node << ") is " 
    << patternsBor[node].sufLink << std::endl << std::endl;
    //возвращаем суффикс ссылку
    return patternsBor[node].sufLink;
}

void Bor::nodeCheck(int index) {
    //для подсчета текущей длины суффиксных и конечных ссылок
    int currentSufLinks = 0;
    int currentEndLinks = 0;
    std::cout << "Checking (" << patternsBor[index].symbol << ", " << index << ")" << std::endl;

    for (int pos = borStepPos; pos != 0; pos = getLink(pos), ++currentSufLinks) {
        //если символ терминальный
        if (patternsBor[pos].patternEnd) {
            std::cout << "In finish state at(" << patternsBor[pos].symbol << ", " << pos << "0" << std::endl; 

            answers.push_back(std::pair<int, int>(index - patternsBor[pos].patternOffset + 2
            , patternsBor[pos].patternEnd));

            ++currentEndLinks;
        }
    }  

    //если текущая длина суффикс или конечной ссылки больше, то обновим значение
    maxEndLink = (currentEndLinks > maxEndLink) ? currentEndLinks : maxEndLink;
    maxSufLink = (currentSufLinks > maxSufLink) ? currentSufLinks : maxSufLink;
}

int Bor::step(int node, char symb) {
    std::map<char, int>::iterator mapIt;
    mapIt = patternsBor[node].moves.find(symb);

    //если нет шага по текущему символу
    if (mapIt == patternsBor[node].moves.end()) {
        std::cout << "No move from " << patternsBor[node].symbol << " to " << symb << std::endl;
        std::cout << "...Finding..." << std::endl;

        mapIt = patternsBor[node].children.find(symb);
        // если был найден прямой переход по символу
        if (mapIt != patternsBor[node].children.end()) {
            std::cout << "Got child with symbol " << symb << std::endl;
            
            //прямой переход
            patternsBor[node].moves[symb] = patternsBor[node].children[symb];
        //прямого прееходанет, идем по суффикс ссылке
        } else {
            std::cout << "...No child, go by link..." << std::endl;

            //если в корне, то движемся в корень
            if (node == 0) {
                std::cout << "...In the root..." << std::endl;

                patternsBor[node].moves[symb] = 0;
            //иначе движемся по суффикс сылке символа
            } else {
                patternsBor[node].moves[symb] = step(getLink(node), symb);
            }
        }
    }

    std::cout << "...Move found..." << std::endl;
    std::cout << "From " << patternsBor[node].symbol << " to " << symb << " is " 
    << patternsBor[node].moves[symb] << std::endl << std::endl;

    //возвращаем прееход
    return patternsBor[node].moves[symb];
}