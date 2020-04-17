#include <iostream>
#include <string>
#include <vector>

void prefix(std::string str, std::vector<int> &vec);
void KMP(std::string text, std::string pattern, std::vector<int> &answer);

int main() {
	std::vector<int> answer;
	std::string text;
	std::string pattern;

	std::cin >> pattern;
	std::cin >> text;

	KMP(text, pattern, answer);

	if (answer.empty()) {
		std::cout << "-1";
	} else {
		std::cout << answer[0];
		for (int i = 1; i < answer.size(); i++) {
			std::cout << "," << answer[i];
		}
	}
	return 0;
}

void prefix(std::string str, std::vector<int> &vec) {
	vec.resize(str.size());
	int k;
	for (int i = 1; i < vec.size(); i++) {
		k = vec.at(i - 1);
		while (k > 0 && str.at(i) != str.at(k)) {
			k = vec.at(k - 1);
		}
		if (str.at(k) == str.at(i)) {
			++k;
		}
		vec[i] = k;
	}
}

void KMP(std::string text, std::string pattern, std::vector<int> &answer) {
	std::vector<int> prefixes;
	prefix(pattern, prefixes);

	for (int k = 0, i = 0; k != text.length();) {
		if (text.at(k) == pattern[i]) {
			k++;
			i++;
			if (i == pattern.length()) {
				i = 0;
				answer.push_back(k - pattern.length());
			}
		} else {
			if (i == 0) {
				k++;
			} else {
				i = prefixes.at(i - 1);
			}
		}
	}
}