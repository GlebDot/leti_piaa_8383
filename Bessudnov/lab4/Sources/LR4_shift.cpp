#include <iostream>
#include <string>
#include <vector>

class StringSearcher {
private:
	std::string text; //������-�����
	std::vector<int> answers; //������ ��� ������ ��������� ���������
	std::vector<int> prefixies; //������ ��� �������� ���������


	void prefix(const std::string &str); //������� ��� ���������� �������-�������

	void printPrefixies(); //������� ������ ���������
public:
	StringSearcher(); //������������
	StringSearcher(std::string text);

	void printAnswerKMP(); //������� ������ ����������� ���
	void printAnswerShift(); //������� ������ �������� ����� �� �����������

	void KMP(const std::string &pattern); //������� ���-���������
	void shiftCheck(const std::string &checkString); //������� �������� ����� �� �����������
};

StringSearcher::StringSearcher(std::string text) : text(text) {

}

StringSearcher::StringSearcher() {
	std::cin >> text;
}

void StringSearcher::KMP(const std::string &pattern) {
	answers.clear();
	prefix(pattern);


	std::cout << "KMP algorithm start" << std::endl;
	for (int k = 0, i = 0; k != text.length();) {
		if (text.at(k) == pattern.at(i)) {
			std::cout << "Text at index " << k << " and pattern at index " << i << " are equal" << std::endl;
			k++;
			i++;
			if (i == pattern.length()) {
				std::cout << "FOUND PATTERN" << std::endl;
				i = 0;
				answers.push_back(k - pattern.length());
			}
		} else {
			if (i == 0) {
				k++;
			} else {
				std::cout << "Reset i to " << prefixies.at(i - 1) << std::endl;
				i = prefixies.at(i - 1);
			}
		}
	}
}

void StringSearcher::shiftCheck(const std::string &checkString) {
	answers.clear();
	std::cout << "Shift check start" << std::endl;
	if (checkString.length() != text.length()) {
		std::cout << "Strings have different length" << std::endl;
		return;
	}
	text += text;
	KMP(checkString);
}

void StringSearcher::prefix(const std::string &str) {
	std::cout << "\n\nCalculating prefix function for string:" << std::endl;
	std::cout << str << std::endl;
	prefixies.resize(str.size());
	int k;
	for (int i = 1; i < prefixies.size(); i++) {
		k = prefixies.at(i - 1);
		while (k > 0 && str.at(i) != str.at(k)) {
			k = prefixies.at(k - 1);
		}
		if (str.at(k) == str.at(i)) {
			++k;
		}
		prefixies.at(i) = k;
	}

	printPrefixies();
}

void StringSearcher::printAnswerKMP() {
	std::cout << "\n" << std::endl;
	if (answers.empty()) {
		std::cout << "No patterns were found" << std::endl;
	} else {
		std::cout << "Patterns are starting from: ";
		for (auto &index : answers) {
			std::cout << index << " ";
		}

		std::cout << std::endl;
	}
}

void StringSearcher::printAnswerShift() {
	std::cout << "\n" << std::endl;
	if (answers.empty()) {
		std::cout << "Strings are not shifted";
	} else {
		std::cout << "First shifted index: " << answers[0] << std::endl;
	}
}

void StringSearcher::printPrefixies() {
	std::cout << "Prefixies:" << std::endl;
	for (auto &pref : prefixies) {
		std::cout << pref << " ";
	}
	std::cout << "\n" << std::endl;
}


int main() {
	std::string temp;
	std::cin >> temp;
	StringSearcher stringSearcher(temp);
	std::cin >> temp;

//	stringSearcher.KMP(temp);
//	stringSearcher.printAnswerKMP();

	stringSearcher.shiftCheck(temp);
	stringSearcher.printAnswerShift();

	return 0;
}