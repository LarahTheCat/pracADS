#include <iostream>
using namespace std;

const int T = 3; // Minimum degree of B-Tree (adjustable)

// BTreeNode class
class BTreeNode {
public:
    int *keys;           // Array of keys
    BTreeNode **children; // Array of child pointers
    int numKeys;         // Current number of keys
    bool isLeaf;         // True if node is a leaf

    BTreeNode(bool leaf) {
        isLeaf = leaf;
        keys = new int[2 * T - 1];
        children = new BTreeNode *[2 * T];
        numKeys = 0;
    }

    void traverse() {
        for (int i = 0; i < numKeys; i++) {
            if (!isLeaf)
                children[i]->traverse();
            cout << keys[i] << " ";
        }
        if (!isLeaf)
            children[numKeys]->traverse();
    }

    BTreeNode *search(int key);

    void insertNonFull(int key);
    void splitChild(int i, BTreeNode *child);
    void remove(int key);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPredecessor(int idx);
    int getSuccessor(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
};

// BTree class
class BTree {
private:
    BTreeNode *root;

public:
    BTree() {
        root = NULL;
    }

    void traverse() {
        if (root)
            root->traverse();
    }

    BTreeNode *search(int key) {
        return root ? root->search(key) : NULL;
    }

    void insert(int key) {
        if (!root) {
            root = new BTreeNode(true);
            root->keys[0] = key;
            root->numKeys = 1;
        } else {
            if (root->numKeys == 2 * T - 1) {
                BTreeNode *newRoot = new BTreeNode(false);
                newRoot->children[0] = root;
                newRoot->splitChild(0, root);

                int i = (newRoot->keys[0] < key) ? 1 : 0;
                newRoot->children[i]->insertNonFull(key);

                root = newRoot;
            } else {
                root->insertNonFull(key);
            }
        }
    }

    void remove(int key) {
        if (!root) {
            cout << "The tree is empty\n";
            return;
        }

        root->remove(key);

        if (root->numKeys == 0) {
            BTreeNode *temp = root;
            root = root->isLeaf ? NULL : root->children[0];
            delete temp;
        }
    }
};

// Search for a key in the B-Tree
BTreeNode *BTreeNode::search(int key) {
    int i = 0;

    // Find the first key greater than or equal to key
    while (i < numKeys && key > keys[i])
        i++;

    // If the key is present, return this node
    if (i < numKeys && keys[i] == key)
        return this;

    // If the node is a leaf, the key is not present
    if (isLeaf)
        return NULL;

    // Recur on the appropriate child
    return children[i]->search(key);
}

// Insert a key into a non-full node
void BTreeNode::insertNonFull(int key) {
    int i = numKeys - 1;

    if (isLeaf) {
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
        numKeys++;
    } else {
        while (i >= 0 && keys[i] > key)
            i--;

        if (children[i + 1]->numKeys == 2 * T - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < key)
                i++;
        }
        children[i + 1]->insertNonFull(key);
    }
}

// Split a full child
void BTreeNode::splitChild(int i, BTreeNode *child) {
    BTreeNode *newNode = new BTreeNode(child->isLeaf);
    newNode->numKeys = T - 1;

    for (int j = 0; j < T - 1; j++)
        newNode->keys[j] = child->keys[j + T];

    if (!child->isLeaf) {
        for (int j = 0; j < T; j++)
            newNode->children[j] = child->children[j + T];
    }

    child->numKeys = T - 1;

    for (int j = numKeys; j >= i + 1; j--)
        children[j + 1] = children[j];

    children[i + 1] = newNode;

    for (int j = numKeys - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = child->keys[T - 1];
    numKeys++;
}

// Remove a key from the node
void BTreeNode::remove(int key) {
    int idx = 0;
    while (idx < numKeys && keys[idx] < key)
        idx++;

    if (idx < numKeys && keys[idx] == key) {
        if (isLeaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
        cout<<"\n>> Deleted sucessfully !!"<<endl;
    } else {
        if (isLeaf) {
            cout << "\n>> The key " << key << " is not in the tree\n";
            return;
        }

        bool flag = (idx == numKeys);

        if (children[idx]->numKeys < T)
            fill(idx);

        if (flag && idx > numKeys)
            children[idx - 1]->remove(key);
        else
            children[idx]->remove(key);
    }
}

// Remove a key from a leaf node
void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < numKeys; i++)
        keys[i - 1] = keys[i];
    numKeys--;
}

// Remove a key from a non-leaf node
void BTreeNode::removeFromNonLeaf(int idx) {
    int key = keys[idx];

    if (children[idx]->numKeys >= T) {
        int pred = getPredecessor(idx);
        keys[idx] = pred;
        children[idx]->remove(pred);
    } else if (children[idx + 1]->numKeys >= T) {
        int succ = getSuccessor(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ);
    } else {
        merge(idx);
        children[idx]->remove(key);
    }
}

// Get predecessor of a key
int BTreeNode::getPredecessor(int idx) {
    BTreeNode *cur = children[idx];
    while (!cur->isLeaf)
        cur = cur->children[cur->numKeys];
    return cur->keys[cur->numKeys - 1];
}

// Get successor of a key
int BTreeNode::getSuccessor(int idx) {
    BTreeNode *cur = children[idx + 1];
    while (!cur->isLeaf)
        cur = cur->children[0];
    return cur->keys[0];
}

// Fill a child node
void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->numKeys >= T)
        borrowFromPrev(idx);
    else if (idx != numKeys && children[idx + 1]->numKeys >= T)
        borrowFromNext(idx);
    else {
        if (idx != numKeys)
            merge(idx);
        else
            merge(idx - 1);
    }
}

// Borrow a key from the previous sibling
void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx - 1];

    for (int i = child->numKeys - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    if (!child->isLeaf) {
        for (int i = child->numKeys; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!child->isLeaf)
        child->children[0] = sibling->children[sibling->numKeys];

    keys[idx - 1] = sibling->keys[sibling->numKeys - 1];

    child->numKeys++;
    sibling->numKeys--;
}

// Borrow a key from the next sibling
void BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];

    child->keys[child->numKeys] = keys[idx];

    if (!child->isLeaf)
        child->children[child->numKeys + 1] = sibling->children[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->numKeys; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->isLeaf) {
        for (int i = 1; i <= sibling->numKeys; i++)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->numKeys++;
    sibling->numKeys--;
}

// Merge two children
void BTreeNode::merge(int idx) {
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];

    child->keys[T - 1] = keys[idx];

    for (int i = 0; i < sibling->numKeys; i++)
        child->keys[i + T] = sibling->keys[i];

    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->numKeys; i++)
            child->children[i + T] = sibling->children[i];
    }

    for (int i = idx + 1; i < numKeys; i++)
        keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= numKeys; i++)
        children[i - 1] = children[i];

    child->numKeys += sibling->numKeys + 1;
    numKeys--;

    delete sibling;
}

// Main function with a dynamic menu
int main() {
    BTree bTree;
    int choice, key;
    cout<<"======== B-Tree Implementation ========"<<endl;

    do {
        cout << "\n--------- B-Tree Menu ---------\n";
        cout << "1. Insert a key\n";
        cout << "2. Search for a key\n";
        cout << "3. Delete a key\n";
        cout << "4. Display B-Tree\n";
        cout << "5. Exit";
        cout << "\n--------------------------------"<<endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter the key to insert: ";
            cin >> key;
            bTree.insert(key);
            break;

        case 2:
            cout << "Enter the key to search: ";
            cin >> key;
            if (bTree.search(key))
                cout << "\n>> Key " << key << " is found in the B-Tree.\n";
            else
                cout << "\n>>Key " << key << " is not found in the B-Tree.\n";
            break;

        case 3:
            cout << "Enter the key to delete: ";
            cin >> key;
            bTree.remove(key);
            break;

        case 4:
            cout << "\n-> Keys in B-Tree: ";
            bTree.traverse();
            cout << endl;
            break;

        case 5:
            cout << "Exiting...\n";
            break;

        default:
            cout << ">> Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}

