#include <iostream>
#include <fstream>

typedef struct listTag list;

struct listTag {
    int value;
    list* next = nullptr;
};

typedef struct treeTag tree;

struct treeTag {
    std::string word;
    list* include;
    std::pair<tree*, tree*> next = {nullptr, nullptr};
};


tree* getTree(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "file " << filename << " missed";
        return nullptr;
    }

    auto result = new tree;
    std::string word;
    char currentChar;
    int stringCounter = 0;

    auto addWord = [result](const std::string& word, int stringCounter) {
        auto current = result;
        while (true) {
            if (current->word.empty()) {
                current->word = word;
                current->include = new list;
                current->include->value = stringCounter;
                break;
            }

            if (current->word == word) {
                auto currentInclude = current->include;
                while (currentInclude->next != nullptr) {
                    currentInclude = currentInclude->next;
                }

                if (currentInclude->value != stringCounter) {
                    currentInclude->next = new list;
                    currentInclude->next->value = stringCounter;
                }

                break;
            }

            auto next = (word < current->word) ? current->next.first : current->next.second;

            if (next == nullptr) {
                ((word < current->word) ? current->next.first : current->next.second) = next = new tree;
            }
            current = next;
        }
    };

    while (file >> std::noskipws >> currentChar) {
        if (!(currentChar == ' ' || currentChar == '\n')) {
            word += currentChar;
            continue;
        }

        addWord(word, stringCounter);

        word.clear();
        if (currentChar == '\n')
            stringCounter++;
    }

    addWord(word, stringCounter);

    file.close();

    return result;
}

tree* find(tree* textTree, const std::string& word) {
    auto current = textTree;

    while (true) {
        if (current->word == word)
            return current;

        auto next = (word < current->word) ? current->next.first : current->next.second;

        if (next == nullptr)
            return nullptr;
        current = next;
    }
}

int main() {
    auto textTree = getTree("data.txt");

    if (textTree == nullptr)
        return -1;

    std::string word;
    std::cout << "Enter word:\n";
    std::cin >> word;

    auto result = find(textTree, word);

    if (result == nullptr)
        std::cout << "No such word in the text";
    else {
        std::cout << "Word " << result->word << " situated in the next strings: ";
        list* current = result->include;

        while (current != nullptr) {
            std::cout << current->value + 1 << ((current->next != nullptr) ? ", " : ".");
            current = current->next;
        }
    }

    return 0;
}