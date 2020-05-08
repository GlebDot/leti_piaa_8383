#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include<iomanip>


struct Node{
    char symbol; //символ
    int sufLink; //суффикс ссылка
    int parent; //родитель
    std::vector<int> pos; //позиции в строке-образце
    std::map<char, int> moves; //ходы из вершины
    std::map<char, int> children; //дети вершины     

    Node() : symbol('0'), sufLink(-1), parent(-1) {
    }
    Node(char symb, int link, int parent) : symbol(symb),  
    sufLink(link), parent(parent) {
    }
};

class Bor {
private:
    std::vector<Node> patternsBor; //бор
    std::vector<int> positions; //массив позиций
    int initialPatternLength; //исходный размер строки-образца
    int localPatternNumber; //количество подстрок без джокеров в строке-образце
    int borStepPos; //текущая вершина
    char joker; //символ-джокер

    int maxSufLink; //макимальная длина суффикс-ссылки
    int maxEndLink; //максимальная длина конечной ссылки

    int step(int node, char symb); //функция перехода
    int getLink(int node); //цункция получения ссылки
    void addStringToBor(const std::string &str, int index); //функция добавлени строки к бору
    void nodeCheck(int index); //функция проверки вершины
    void printAnswers(); //функция печати ответов
public:
    Bor(); //конструктор
    ~Bor()=default; //деструктор

    void printBor();//печать бора
    void initBor(char joker, int textLength, const std::string pattern); //функция инициализации бора
    void findPatterns(const std::string &text); //функция нахождения вхождений
};


int main() {
    Bor bor;
    std::string text;
    std::string pattern;
    char joker;

    std::cin >> text;
    std::cin >> pattern;
    std::cin >> joker;
    bor.initBor(joker, text.length(), pattern);

    bor.findPatterns(text);
    bor.printBor();
    
    return 0;
}



Bor::Bor() : borStepPos(0), maxEndLink(0), maxSufLink(0) {
    patternsBor.push_back(Node('0', 0, 0));
}

void Bor::initBor(char joker, int textLength, const std::string pattern) {
    //накопитель строки
    std::string patternTemp;

    //зануление веткора позиций
    positions.clear();
    positions.resize(textLength, 0);


    localPatternNumber = 0;
    initialPatternLength = pattern.length();

    int i = 0;
    //проход по образцу
    for(; i < pattern.length(); i++) {
        //если символ в паттерне не джокер, то сохраним его
        if (pattern[i] != joker) {
            patternTemp.push_back(pattern[i]);
        } else {
        //если символ джокер, то добавить получившуюся строку в бор и очистить накопитель
            addStringToBor(patternTemp, i);
            patternTemp.clear();
        }
    }
    //добавлеяем строку, которая получилась в конце
    addStringToBor(patternTemp, i);
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
    std::cout << "-------ANSWERS-------" << std::endl;
    std::cout << std::setw(10) << "Position" << std::endl;
    for(int i = 0; i < positions.size(); i++) {
        if (positions[i] == localPatternNumber) {
            std::cout << std::setw(10) << i + 1 << std::endl;
        }
    }
    std::cout << "Max suf link: " << maxSufLink << std::endl;
    std::cout << "Max end link: " << maxEndLink << std::endl;
}

//добавляем строку в бор
void Bor::addStringToBor(const std::string &str, int index) {
    //если длина строки 0, то ничего не делаем
    if (str.length() == 0) {
        return;
    }
    int curPos;
    curPos = 0;
    for(int i = 0; i < str.length(); i++) {
        //если символ уже есть, то проходим по нему
        if(patternsBor[curPos].children[str[i]] != 0) {
            curPos = patternsBor[curPos].children[str[i]];
        } else {
            //если символа нет, то добавляем
            patternsBor.push_back(Node(str[i], -1, curPos));
            patternsBor[curPos].children[str[i]] = patternsBor.size() - 1;
            curPos = patternsBor.size() - 1;
        }
    }
    localPatternNumber++;
    //сохраняем номер позиции для строки
    patternsBor[curPos].pos.push_back(index);
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
    //если суффикс ссылки еще нет
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
        if (!patternsBor[pos].pos.empty()) {
            std::cout << "In finish state at(" << patternsBor[pos].symbol << ", " << pos << "0" << std::endl; 
            ++currentEndLinks;
            //для каждой терминальной позиции увеличить значение в векторе
            for(auto &startPos : patternsBor[pos].pos) {
                int posIndex;
                posIndex = index - startPos + 1;
                if(posIndex >= 0 && posIndex + initialPatternLength - 1 < positions.size()) {
                    positions[posIndex]++;
                }
            }
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