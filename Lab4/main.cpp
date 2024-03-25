#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>



#define N 7


class Tree {
public:
	std::pair<Tree*, Tree*> *subtree = new std::pair<Tree*, Tree*>;
	std::string data;
	int length;
	int depth = 0;

	Tree(const std::string &str) {
		data = str;
	}

	Tree*& left() {
		return subtree->first;
	}

	Tree*& right() {
		return subtree->second;
	}
};

void fillTree(const std::vector<Tree*> trees, const std::string filename) {
	std::ifstream file(filename);

	for (int tree_index = 0; tree_index < N; tree_index++) {
		for (int index = 0; index < N; index++) {
			int val;
			file >> val;

			if (val == 0)
				continue;

			auto tree = trees[tree_index];

			if (tree->left() == nullptr)
				tree->left() = trees[index];
			else
				tree->right() = trees[index];
		}
	}
}

int countDepth(Tree* tree, int *maxDepth, int current = 0) {
	if (tree == nullptr)
		return 0;

	if (*maxDepth < current)
		*maxDepth = current;

	tree->depth = current;
	countDepth(tree->left(), maxDepth, current + 1);
	countDepth(tree->right(), maxDepth, current + 1);

	return tree->depth;
}

int calculate(Tree* tree) {
	if (tree == nullptr)
		return 0;

	tree->length = calculate(tree->left()) + calculate(tree->right()) + tree->data.size();

	return tree->length;
}

void fillMatrix(Tree* tree, std::vector<std::vector<int>>& matrix, int position, int add) {
	if (tree == nullptr)
		return;

	matrix[tree->depth][position] = tree->length;
	//matrix[tree->depth][position] = tree->data.size() - 1;
	int newAdd = add / 2;

	fillMatrix(tree->left(), matrix, position - add + newAdd, newAdd);
	fillMatrix(tree->right(), matrix, position + add - newAdd, newAdd);
}

void print(Tree* head, int maxDepth) {
	int maxLength = pow(2, maxDepth + 1) - 1;
	std::vector<std::vector<int>> matrix(maxDepth + 1, std::vector<int>(maxLength, 0));

	fillMatrix(head, matrix, maxLength / 2, maxLength / 2);

	for (int row = 0; row < maxDepth + 1; row++) {
		for (int col = 0; col < maxLength; col++) {
			int value = matrix[row][col];

			if (value == 0)
				std::cout << ' ';
			else
				std::cout << value;
		}
		std::cout << '\n';
	}
}

int main() {
	std::vector<Tree*> trees;

	for (int index = 0; index < N; index++) {
		std::string str = "";

		for (int _ = 0; _ < index + 1; _++)
			str += "a";

		trees.push_back(new Tree(str));
	}

	fillTree(trees, "input.txt");
	auto head = trees[4];
	head->length = calculate(head);

	int maxDepth = 0;
	countDepth(head, &maxDepth);
 	print(head, maxDepth);

	return 0;
}