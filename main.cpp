#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

const std::vector<std::pair<int, int>> neighbours = {{0,  -1},
                                                     {1,  0},
                                                     {0,  1},
                                                     {-1, 0}};

class WordNode {
public:
    bool isWord = false;
    bool isMore = false;
    std::map<char, WordNode> nextLetter;

    void insertWord(std::string text) {
        if (text[0] != '\0') {
            char c = text[0] >= 'a' ? text[0] - 'a' + 'A' : text[0];
            isMore = true;
            nextLetter[c].insertWord(text.substr(1));
        } else {
            isWord = true;
        }
    }

    void printWords(int indent = 0) {
        for (auto &x : nextLetter) {
            for (int i = 0; i < indent; i++) std::cout << " ";

            std::cout << x.first << " : " << x.second.isWord << std::endl;

            x.second.printWords(indent + 1);
        }
    }

    bool search(std::string text) {
        if (text[0] == '\0')
            return isWord;

        char c = text[0] >= 'a' ? text[0] - 'a' + 'A' : text[0];

        return nextLetter[c].search(text.substr(1));
    }
};

void
searchRectangle(std::vector<std::string> wordMatrix, WordNode dictionary, int i, int j, std::string currString = "") {
    char c = wordMatrix[i][j];
    c = c >= 'a' ? c - 'a' + 'A' : c;

    WordNode nextDictionary = dictionary.nextLetter[c];

    if (nextDictionary.isWord) std::cout << "Found: " << currString.c_str() << c << std::endl;

    if (nextDictionary.isMore) {
        for (std::pair<int, int> pair : neighbours) {
            if ((i + pair.first < 0) || (i + pair.first >= wordMatrix.size()) || (j + pair.second < 0) ||
                (j + pair.second >= wordMatrix[0].length()))
                continue;

            searchRectangle(wordMatrix, nextDictionary, i + pair.first, j + pair.second, currString + c);
        }
    }
}

int main() {

    std::string s;

    std::vector<std::string> words, rectangleRaw;

    std::ifstream wordList("..\\english3.txt");
    while (std::getline(wordList, s)) words.push_back(s);

    std::ifstream wordGrid("..\\grid.txt");
    while (std::getline(wordGrid, s)) rectangleRaw.push_back(s);

    WordNode dictionary = WordNode();

    for (const std::string& word : words) dictionary.insertWord(word);

    dictionary.insertWord("Kotlin");
    dictionary.insertWord("fun");
    dictionary.insertWord("file");
    dictionary.insertWord("line");
    dictionary.insertWord("null");

    for (int i = 0; i < rectangleRaw.size(); ++i) {
        for (int j = 0; j < rectangleRaw[0].length(); ++j) {
            searchRectangle(rectangleRaw, dictionary, i, j);
        }
    }

    return 0;
}