#include <iostream>
#include <vector>
using namespace std;

void computeLPS(const string &pattern, vector<int> &lps) {
    int length = 0;
    lps[0] = 0;   
    int i = 1;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMPAlgorithm(const string &text, const string &pattern) {
    int n = text.length();
    int m = pattern.length();
    vector<int> lps(m);
    computeLPS(pattern, lps);

    int i = 0; 
    int j = 0; 
    bool found = false; 

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            cout << ">> Pattern found at index " << i - j << endl;
            found = true;
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1]; 
            } else {
                i++;
            }
        }
    }

    if (!found) {
        cout << "\n>> Pattern not found" << endl;
    }
}

int main() {
	int opt;
	cout << "======= String Matching Algorithm =======\n" << endl;
	string text;
	    cout << "-> Enter the string: ";
	    cin >> text;
	do{
		string pattern;
	    cout << "-> Enter the pattern to search: ";
	    cin >> pattern;
	    KMPAlgorithm(text, pattern);
		
		cout<<"\nPress 1 to search more patterns or any other key to exit ... ";
		cin>>opt;
		cout<<"--------------------------------------------------\n"<<endl;
	}while(opt == 1);

    return 0;
}

