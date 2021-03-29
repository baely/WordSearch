#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

const std::vector<std::pair<int, int>> neighbours = {{0,-1},{1,0},{0,1},{-1,0}};

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
};

void
searchRectangle(std::vector<std::string> wordMatrix, WordNode dictionary, int i, int j, std::string currString = "") {
    // Get the current cell character and capitalise it if needed.
    char c = wordMatrix[i][j];
    c = c >= 'a' ? c - 'a' + 'A' : c;

    // Load the dictionary for character c.
    WordNode nextDictionary = dictionary.nextLetter[c];

    // If flagged as a word, let us know
    if (nextDictionary.isWord) std::cout << "Found: " << currString.c_str() << c << std::endl;

    // If there are deeper words, run the algo on each neighbour, keeping track of the characters
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
    // Load files
    std::string s;

    std::vector<std::string> words, rectangleRaw;

    std::ifstream wordList("..\\english3.txt");
    while (std::getline(wordList, s)) words.push_back(s);

    std::ifstream wordGrid("..\\grid.txt");
    while (std::getline(wordGrid, s)) rectangleRaw.push_back(s);

    /**
     * Algo start here
     */

    WordNode dictionary = WordNode();

    // Load the wordlist into new dictionary
    for (const std::string& word : words) dictionary.insertWord(word);

    // Run the algorithm starting on each cell in the search grid
    for (int i = 0; i < rectangleRaw.size(); ++i) {
        for (int j = 0; j < rectangleRaw[i].length(); ++j) {
            searchRectangle(rectangleRaw, dictionary, i, j);
        }
    }

    return 0;
}
