#include <iostream>;
#include <fstream>;
#include <string>;
#include <map>;
#include <List>;
#include <vector>;

using namespace std;

vector <bool> code;
map <char, vector <bool> > table;

class Node {
public:
	int a;
	char c;
	Node *left, *right;
	Node() {};
	Node(Node *L, Node *R) {
		this->left = L;
		this->right = R;
		a = L->a + R->a;
	}
};

struct MyCompare {
	bool operator() (Node *l, Node *r) const {
		return l->a < r->a;
	}
};

void print(Node *root, unsigned k = 0) { 
	if (root != NULL) {
		print(root->left, k + 3);
		for (unsigned i = 0; i < k; i++) {
			cout << "   ";
		}
		if (root->c)
			cout << root->a << " (" << root->c << ")" << "\n";
		else
			cout << root->a << "\n";
		print(root->right, k + 3);
	}
}

void BuildTable(Node *root) {
	if (root->left) {
		code.push_back(0);
		BuildTable(root->left);
	}
	if (root->right) {
		code.push_back(1);
		BuildTable(root->right);
	}
	if (root->c) {
		table[root->c] = code;
		code.pop_back();
	}
}

int main() {
	map <char, int> m;
	map <char, int> ::iterator i;
	list <Node*> t;
	ifstream f("1.txt");
	while (!f.eof()) {
		char c;
		f >> c;
		m[c]++;
	}

	for (i = m.begin(); i != m.end(); ++i) {
		Node* p = new Node;
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);
	}

	while (t.size() != 1) {
		t.sort(MyCompare());
		Node *SonL = t.front();
		t.pop_front();
		Node *SonR = t.front();
		t.pop_front();

		Node *parent = new Node(SonL, SonR);
		t.push_back(parent);
	}

	Node *root = t.front();
	print(root);
	BuildTable(root);
	f.clear();
	f.seekg(0);
	ofstream g("output.txt");
	int count = 0;
	char buf = 0;

	while (!f.eof()) {
		char c;
		f >> c;
		vector <bool> x = table[c];
		for (int j = 0; j < x.size(); j++) {
			buf = buf | (x[j] << (7 - count));
			count++;
			if (count == 8) {
				count = 0;
				g << buf;
				buf = 0;
			}
		}
	}

	f.close();
	return 0;
}