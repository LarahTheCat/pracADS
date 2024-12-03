#include <iostream>
#include <string>

using namespace std;

struct TrieNode {
    TrieNode* children[26]; 
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
    }
};

class Trie {
private:
    TrieNode* root;
    
    void printWordsHelper(TrieNode* node, string currentWord, bool& isEmpty) {
        if (node->isEndOfWord) {
            cout << currentWord << endl;
            isEmpty = false;  
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != NULL) {
                printWordsHelper(node->children[i], currentWord + char(i + 'a'), isEmpty);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* current = root;
        for (size_t i = 0; i < word.length(); i++) {
            int index = word[i] - 'a'; 
            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true; 
    }

    bool search(const string& word) {
        TrieNode* current = root;
        for (size_t i = 0; i < word.length(); i++) {
            int index = word[i] - 'a'; 
            if (current->children[index] == NULL) {
                return false; 
            }
            current = current->children[index];
        }
        return current->isEndOfWord; 
    }
    
    void printWords() {
        bool isEmpty = true; 
        printWordsHelper(root, "", isEmpty);

        if (isEmpty) {
            cout << "\n>> The Trie is empty." << endl;
        }
    }
};

int main() {
    Trie trie;
    int choice;
    string word;
	cout<<"------------ TRIE DATA STRUCTURE ------------"<<endl;
    do {
        cout << "\n--------- Trie Menu ---------\n";
        cout << "1. Insert a word into the Trie\n";
        cout << "2. Search for a word in the Trie\n";
        cout << "3. List all words in the Trie\n";
        cout << "4. Exit";
        cout << "\n-----------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "-> Enter the word to insert: ";
                cin >> word;
                trie.insert(word);
                cout << "\n>> Word inserted successfully.\n";
                break;

            case 2:
                cout << "-> Enter the word to search: ";
                cin >> word;
                if (trie.search(word)) {
                    cout << "\n>> Word found in the Trie.\n";
                } else {
                    cout << "\n>> Word not found in the Trie.\n";
                }
                break;

            case 3:
                cout << "\n>> List of all words in the Trie:\n";
                trie.printWords();
                break;

            case 4:
                cout << "Exiting the program.\n";
                break;

            default:
                cout << "\n>> Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);

    return 0;
}

