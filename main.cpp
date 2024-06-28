#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

struct Node {
    int key;
    Node* parent;
    Node* child;
    Node* left;
    Node* right;
    int degree;
    bool mark;

    Node(int k) : key(k), parent(nullptr), child(nullptr), left(this), right(this), degree(0), mark(false) {}
};

class FibonacciHeap {
private:
    Node* min;
    int n;

    void link(Node* y, Node* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x;
        if (x->child == nullptr) {
            x->child = y;
            y->right = y;
            y->left = y;
        } else {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        x->degree++;
        y->mark = false;
    }

    void consolidate() {
        int D = static_cast<int>(log2(n)) + 1;
        vector<Node*> A(D, nullptr);

        vector<Node*> rootList;
        Node* x = min;
        if (x != nullptr) {
            do {
                rootList.push_back(x);
                x = x->right;
            } while (x != min);
        }

        for (Node* w : rootList) {
            x = w;
            int d = x->degree;
            while (A[d] != nullptr) {
                Node* y = A[d];
                if (x->key > y->key) {
                    swap(x, y);
                }
                link(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
        }

        min = nullptr;
        for (Node* node : A) {
            if (node != nullptr) {
                if (min == nullptr) {
                    min = node;
                    node->left = node;
                    node->right = node;
                } else {
                    node->left = min;
                    node->right = min->right;
                    min->right->left = node;
                    min->right = node;
                    if (node->key < min->key) {
                        min = node;
                    }
                }
            }
        }
    }

public:
    FibonacciHeap() : min(nullptr), n(0) {}

    void insert(int key) {
        Node* node = new Node(key);
        if (min == nullptr) {
            min = node;
        } else {
            node->left = min;
            node->right = min->right;
            min->right->left = node;
            min->right = node;
            if (key < min->key) {
                min = node;
            }
        }
        n++;
    }

    void merge(FibonacciHeap& other) {
        if (other.min == nullptr) {
            return;
        }
        if (min == nullptr) {
            min = other.min;
            n = other.n;
        } else {
            min->right->left = other.min->left;
            other.min->left->right = min->right;
            min->right = other.min;
            other.min->left = min;
            if (other.min->key < min->key) {
                min = other.min;
            }
            n += other.n;
        }
        other.min = nullptr;
        other.n = 0;
    }

    int extractMin() {
        Node* z = min;
        if (z != nullptr) {
            if (z->child != nullptr) {
                Node* x = z->child;
                do {
                    Node* next = x->right;
                    x->left = min;
                    x->right = min->right;
                    min->right->left = x;
                    min->right = x;
                    x->parent = nullptr;
                    x = next;
                } while (x != z->child);
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                min = nullptr;
            } else {
                min = z->right;
                consolidate();
            }
            n--;
            int minKey = z->key;
            delete z;
            return minKey;
        }
        return numeric_limits<int>::min(); // or throw an exception
    }

    void print(Node* node, string indent, bool last) {
        if (node == nullptr) return;
        cout << indent;
        if (last) {
            cout << "\\-- ";
            indent += "    ";
        } else {
            cout << "|-- ";
            indent += "|   ";
        }
        cout << node->key << endl;
        Node* child = node->child;
        if (child != nullptr) {
            do {
                print(child, indent, child->right == node->child);
                child = child->right;
            } while (child != node->child);
        }
    }

    void display() {
        if (min == nullptr) {
            cout << "The heap is empty." << endl;
            return;
        }
        Node* temp = min;
        bool first = true;
        do {
            print(temp, "", first);
            first = false;
            temp = temp->right;
        } while (temp != min);
        cout << endl;
    }
};

int main() {
    FibonacciHeap heap;

    heap.insert(10);
    heap.insert(3);
    heap.insert(15);
    heap.insert(6);
    heap.insert(1);

    cout << "Heap after insertions:" << endl;
    heap.display();

    cout << "Extracted min: " << heap.extractMin() << endl;
    cout << "Heap after extracting min:" << endl;
    heap.display();

    FibonacciHeap heap2;
    heap2.insert(2);
    heap2.insert(8);

    heap.merge(heap2);

    cout << "Heap after merging with another heap:" << endl;
    heap.display();

    return 0;
}
