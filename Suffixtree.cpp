#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class SuffixTree {
private:
    struct Node {
        map<char, Node*> children;  
        int start, *end;  
        int suffixLink;   

        Node(int start, int* end) : start(start), end(end), suffixLink(-1) {}
    };

    string text;
    vector<Node*> nodes;
    int size;

    void buildTree() {
        int n = text.size();
        int* end = new int(-1);  // For representing the end of a string in the tree
        Node* root = new Node(-1, end);
        nodes.push_back(root);

        int activeNodeIndex = 0;  // Track the index of the active node
        int activeEdge = -1;
        int activeLength = 0;
        int remainder = 0;  // Number of suffixes to be added

        for (int i = 0; i < n; ++i) {
            remainder++;
            while (remainder > 0) {
                if (activeLength == 0) activeEdge = i;

                if (nodes[activeNodeIndex]->children.find(text[activeEdge]) == nodes[activeNodeIndex]->children.end()) {
                    nodes[activeNodeIndex]->children[text[activeEdge]] = new Node(i, end);
                    nodes.push_back(nodes[activeNodeIndex]->children[text[activeEdge]]);
                    remainder--;
                } else {
                    Node* nextNode = nodes[activeNodeIndex]->children[text[activeEdge]];

                    int edgeLength = *nextNode->end - nextNode->start + 1;
                    if (activeLength >= edgeLength) {
                        activeNodeIndex = getNodeIndex(nextNode);  // Get node index explicitly
                        activeEdge += edgeLength;
                        activeLength -= edgeLength;
                        continue;
                    }

                    if (text[nextNode->start + activeLength] == text[i]) {
                        activeLength++;
                        break;
                    }

                    // Split the edge
                    int* splitEnd = new int(nextNode->start + activeLength - 1);
                    Node* splitNode = new Node(nextNode->start, splitEnd);
                    nodes.push_back(splitNode);

                    // Add the new split node and children
                    nodes[activeNodeIndex]->children[text[activeEdge]] = splitNode;
                    splitNode->children[text[i]] = new Node(i, end);
                    nodes.push_back(splitNode->children[text[i]]);
                    nextNode->start += activeLength;
                    splitNode->children[text[nextNode->start]] = nextNode;
                    remainder--;
                }
            }
        }
    }

    // Helper function to get the index of a node in the vector
    int getNodeIndex(Node* node) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i] == node) {
                return i;
            }
        }
        return -1; 
    }

    void displayTree(Node* node, int level) {
        for (map<char, Node*>::iterator it = node->children.begin(); it != node->children.end(); ++it) {
            for (int i = 0; i < level; ++i) cout << "  ";
            cout << text.substr(it->second->start, *it->second->end - it->second->start + 1) << endl;
        	displayTree(it->second, level + 1);
        }
    }

public:
    SuffixTree(const string& s) : text(s), size(s.size()) {
        buildTree();
    }

    void printTree() {
        Node* root = nodes[0];
        displayTree(root, 0);
    }

    void setText(const string& s) {
        text = s;
        nodes.clear();
        buildTree();
    }
};

int main() {
    SuffixTree* st = NULL;
    string inputString;
    int choice;
    bool exit = false;

    while (!exit) {
        cout << "\n--- Suffix Tree Menu ---\n";
        cout << "1. Build Suffix Tree\n";
        cout << "2. Display Suffix Tree\n";
        cout << "3. Set New String\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (st != NULL) {
                    cout << "Suffix tree already created. To create a new one, use option 3.\n";
                    break;
                }
                cout << "Enter a string to build the suffix tree: ";
                cin >> inputString;
                st = new SuffixTree(inputString);
                cout << "Suffix tree built successfully.\n";
                break;

            case 2:
                if (st != NULL) {
                    cout << "Displaying Suffix Tree:\n";
                    st->printTree();
                } else {
                    cout << "No suffix tree exists. Please build one first (Option 1).\n";
                }
                break;

            case 3:
                cout << "Enter a new string: ";
                cin >> inputString;
                if (st != NULL) {
                    st->setText(inputString);
                } else {
                    st = new SuffixTree(inputString);
                }
                cout << "Suffix tree updated with new string.\n";
                break;

            case 4:
                exit = true;
                cout << "Exiting program.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    delete st;
    return 0;
}

