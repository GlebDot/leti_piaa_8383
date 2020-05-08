#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>


struct Node{
    char symbol;
    int patternOffset;
    int sufLink;
    int parent;
    std::vector<int> pos;
    std::map<char, int> moves;  
    std::map<char, int> children;      
    Node() : symbol('0'), patternOffset(0), sufLink(-1), parent(-1) {
    }
    Node(char symb, int link, int parent) : symbol(symb),  
    patternOffset(0), sufLink(link), parent(parent) {
    }
};

class Bor {
private:
    std::string text;
    std::vector<Node> patternsBor;
    std::vector<std::pair<int, int>> answers;
    std::vector<int> positions;
    int initialPatternLength;
    int localPatternNumber;
    int borStepPos;
    char joker;

    int step(int node, char symb);
    int getLink(int node);
    void nodeCheck(int index);
    void addStringToBor(const std::string &str, int index);
public:
    Bor();
    ~Bor()=default;
    void initBor();
    void findPatterns();
};

bool answerComp(const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
    if (p1.first == p2.first) {
        return p1.second < p2.second;
    }

    return p1.first < p2.first;
}

int main() {
    Bor bor;
    bor.initBor();
    bor.findPatterns();
    
    return 0;
}



Bor::Bor() {
    patternsBor.push_back(Node('0', 0, 0));
    borStepPos = 0;
}

void Bor::initBor() {
    std::string pattern;
    std::string patternTemp;
    
    std::cin >> text;
    std::cin >> pattern;
    std::cin >> joker;

    positions.clear();
    positions.resize(text.length(), 0);
    localPatternNumber = 0;
    initialPatternLength = pattern.length();

    int i = 0;
    for(; i < pattern.length(); i++) {
        if (pattern[i] != joker) {
            patternTemp.push_back(pattern[i]);
        } else {
            addStringToBor(patternTemp, i);
            patternTemp.clear();
        }
    }
    addStringToBor(patternTemp, i);
}

void Bor::addStringToBor(const std::string &str, int index) {
    if (str.length() == 0) {
        return;
    }
    int curPos;
    curPos = 0;
    for(int i = 0; i < str.length(); i++) {
        if(patternsBor[curPos].children[str[i]] != 0) {
            curPos = patternsBor[curPos].children[str[i]];
        } else {
            patternsBor.push_back(Node(str[i], -1, curPos));
            patternsBor[curPos].children[str[i]] = patternsBor.size() - 1;
            curPos = patternsBor.size() - 1;
            if (patternsBor[curPos].parent == 0) {
                patternsBor[curPos].sufLink = 0;
            }
        }
    }
    localPatternNumber++;
    patternsBor[curPos].patternOffset = str.length();
    patternsBor[curPos].pos.push_back(index);
}

void Bor::findPatterns() {
    borStepPos = 0;

    for (int i = 0; i < text.length(); i++) {
        borStepPos = step(borStepPos, text[i]);
        nodeCheck(i);
    }
    for(int i = 0; i < positions.size(); i++) {
        if (positions[i] == localPatternNumber) {
            std::cout << i + 1 << std::endl;
        }
    }
}

int Bor::getLink(int node) {
    if (patternsBor[node].sufLink == -1) {
        if (node == 0 || patternsBor[node].parent == 0) {
            patternsBor[node].sufLink = 0;
        } else {
            patternsBor[node].sufLink = step(getLink(patternsBor[node].parent), patternsBor[node].symbol);
        }
    }

    return patternsBor[node].sufLink;
}

void Bor::nodeCheck(int index) {
    for (int pos = borStepPos; pos != 0; pos = getLink(pos)) {
        if (!patternsBor[pos].pos.empty()) {
            for(auto &startPos : patternsBor[pos].pos) {
                int posIndex;
                posIndex = index - startPos + 1;
                if(posIndex >= 0 && posIndex + initialPatternLength - 1 < positions.size()) {
                    positions[posIndex]++;
                }
            }
        }
    }   
}

int Bor::step(int node, char symb) {
    std::map<char, int>::iterator mapIt;
    mapIt = patternsBor[node].moves.find(symb);
    if (mapIt == patternsBor[node].moves.end()) {
        mapIt = patternsBor[node].children.find(symb);
        if (mapIt != patternsBor[node].children.end()) {
            patternsBor[node].moves[symb] = patternsBor[node].children[symb];
        } else {
            if (node == 0) {
                patternsBor[node].moves[symb] = 0;
            } else {
                patternsBor[node].moves[symb] = step(getLink(node), symb);
            }
        }
    }

    return patternsBor[node].moves[symb];
}