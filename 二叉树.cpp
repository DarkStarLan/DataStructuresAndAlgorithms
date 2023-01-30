#include<iostream>
using namespace std;
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<stack>
#include<queue>
#include<string>

//�ָ��ַ���  ��Ҫ�ֶ��ͷ�
string* split(string str, const string s, int& size) {
	if (s.length() >= str.length()) return nullptr;
	int pos = str.find(str), t = 0, now = 0;
	while (pos != -1) {
		++size;
		pos = str.find(s, pos + s.length());
	}
	if (size > 0) --size;
	if (size < 1) return nullptr;
	string* ret = new string[size];
	pos = str.find(s);
	while (pos != -1) {
		ret[t++] = str.substr(now, pos - now);
		now = pos + s.length();
		pos = str.find(s, pos + s.length());
	}
	return ret;
}

struct Node {
	friend class Tree;
	Node() = default;
	Node(const int v) : val(v) { }
	Node(const Node& other) : Node(other.val) { }

	Node* left = nullptr, * right = nullptr;
	Node* parent = nullptr;  //���������ʹ�ã�ָ�򸸽ڵ�
	int val = 0;
private:
	bool addLeft(const int value) {
		if (this->left) return false;
		this->left = new Node(value);
		return true;
	}
	bool addRight(const int value) {
		if (this->right) return false;
		this->right = new Node(value);
		return true;
	}
};

class Tree {
public:
	Tree() : root(), m_Size() { }
	Tree(Node* rt, const size_t s) : root(rt), m_Size(s) { }
	Tree(const int value) : root(new Node(value)), m_Size(1) { }
	//���ش�С
	inline size_t size() const { return this->m_Size; }
	//�����ڵ�
	bool addLeft(Node* n, const int val) {
		if (!n) return false;
		if (n->addLeft(val)) {
			++this->m_Size;
			return true;
		}
	}
	//����ҽڵ�
	bool addRight(Node* n, const int val) {
		if (!n) return false;
		if (n->addRight(val)) {
			++this->m_Size;
			return true;
		}
	}
	//���ø��ڵ�
	void setRoot(const int val) {
		if (this->root) this->root->val = val;
		else {
			this->root = new Node(val);
			++this->m_Size;
		}
	}
	//���ظ��ڵ�
	inline Node* Root() const { return this->root; }
	//(�ǵݹ�)�������  ͷ �� ��
	void preorder_each_non_recursive() {
		if (!this->m_Size) return;
		stack<Node*> sk;  //����һ��ջ
		sk.push(this->root);  //���ڵ���ջ
		while (!sk.empty()) {
			Node* temp = sk.top();
			sk.pop();
			if (temp->right) sk.push(temp->right);
			if (temp->left) sk.push(temp->left);
			cout << temp->val << ' ';
		}
		cout << endl;
	}
	//(�ǵݹ�)�������  ͷ ��
	void inorder_each_non_recursive() {
		stack<Node*> sk;
		Node* p = this->root;
		while (p) {  //������ڵ�ȫ�������ӽڵ�
			sk.push(p);
			p = p->left;
		}
		while (!sk.empty()) {
			Node* temp = sk.top();
			sk.pop();
			if (temp->right) {
				p = temp->right;
				while (p) {
					sk.push(p);
					p = p->left;
				}
			}
			cout << temp->val << ' ';
		}
		cout << endl;
	}
	//(�ǵݹ�)�������  ͷ �� ��
	void postorder_each_non_recursive() {
		if (!this->m_Size) return;
		stack<Node*> sk, re;
		sk.push(this->root);
		while (!sk.empty()) {
			Node* temp = sk.top();
			sk.pop();
			re.push(temp);
			if (temp->left) sk.push(temp->left);
			if (temp->right) sk.push(temp->right);
		}
		while (!re.empty()) {
			cout << re.top()->val << ' ';
			re.pop();
		}
		cout << endl;
	}
	//�������
	inline void preorder_each_recursive() {
		this->preT(this->root);
		cout << endl;
	}
	//�������
	inline void inorder_each_recursive() {
		this->inT(this->root);
		cout << endl;
	}
	//�������
	inline void postorder_each_recursive() {
		this->postT(this->root);
		cout << endl;
	}
	//������ȱ���
	void width_first_traversal() {
		queue<Node*> qe;
		qe.push(this->root);
		while (!qe.empty()) {
			Node* temp = qe.front();
			qe.pop();
			if (temp->left) qe.push(temp->left);
			if (temp->right) qe.push(temp->right);
			cout << temp->val << ' ';
		}
		cout << endl;
	}
	//��ȡ��������ڵĲ���
	int getMaxWidth() {
		if (!this->m_Size) return -1;
		queue<Node*> qe;  //����һ������
		qe.push(this->root);
		Node* tl = this->root, *nl = nullptr;  //��ǰ�����һ������һ���ڵ�
		size_t curLevel = 0;  //��ǰ��ڵ���
		size_t maxLevel = 0;  //���ڵ���
		int cur = 0;  //��ǰ��
		int max = 0;  //������Ĳ�
		while (!qe.empty()) {
			Node* temp = qe.front();
			qe.pop();
			++curLevel;
			if (temp->left) {
				qe.push(temp->left);
				nl = temp->left;
			}
			if (temp->right) {
				qe.push(temp->right);
				nl = temp->right;
			}
			if (temp == tl) {  //����
				if (maxLevel < curLevel) {
					maxLevel = curLevel;
					max = cur;
				}
				++cur;
				tl = nl;
				curLevel = 0;
			}
		}
		//return maxLevel;
		return max;
	}
	//�������������
	inline int getMaxDeepth() {
		return this->maxDeepth(this->root);
	}
	//�ж���ȫ������
	bool complete_binary_tree_non_recursive() {
		//1.�������� false
		if (!this->m_Size) return true;
		queue<Node*> qe;
		bool flag = false;
		qe.push(this->root);
		while (!qe.empty()) {
			Node* temp = qe.front();
			bool left = false, right = false;
			qe.pop();
			if (temp->left) {
				left = true;
				qe.push(temp->left);
			}
			if (temp->right) {
				right = true;
				qe.push(temp->right);
			}
			if (flag && (left || right)) return false;
			if (!left && right) return false;
			if (!left || !right) flag = true;
		}
		return true;
	}
	//�ж�����������
	inline bool binary_search_tree_recursive() {
		return this->bst(this->root).f;
	}
	//�ж�ƽ�������
	inline bool balanced_binary_tree_recursive() { return this->bbt(this->root).f; }
	//(�ǵݹ�)�ж���������
	bool full_binary_tree_non_recursive() {
		if (!this->m_Size) return true;
		queue<Node*> qe;
		qe.push(this->root);
		while (!qe.empty()) {
			Node* temp = qe.front();
			qe.pop();
			bool left = (temp->left != nullptr);
			bool right = (temp->right != nullptr);
			if (left && !right || !left && right) return false;
			if (!left && !right) continue;
			qe.push(temp->left);
			qe.push(temp->right);
		}
		return true;
	}
	//�ж���������
	bool full_binary_tree_recursive() {
		Info info = this->fbt(this->root);
		return (info.nodes == ((1 << info.height) - 1));  // return (2 ^ height) - 1
	}
	//(�ǵݹ�)��ȡ��͹�������
	const Node* lane_change_assist_non_recursive(Node* n1, Node* n2) {
		if (!this->m_Size) return nullptr;
		unordered_map<Node*, Node*> fatherMap;
		unordered_set<Node*> hash_set;
		fatherMap.insert(make_pair(this->root, this->root));
		this->process(this->root, fatherMap);
		Node* cur = n1;
		while (cur != this->root) {
			hash_set.insert(cur);
			cur = fatherMap.at(cur);
		}
		hash_set.insert(this->root);
		cur = n2;
		while (cur != this->root) {
			if (hash_set.count(cur)) return cur;
			cur = fatherMap.at(cur);
		}
		return this->root;
	}
	//��ȡ��͹�������
	inline const Node* lane_change_assist_recursive(Node* n1, Node* n2) {
		return this->lca(this->root, n1, n2);
	}
	//��ȡ��̽ڵ�
	const Node* successor_non_recursive(Node* n) {
		if (!n) return nullptr;
		if (n->right) {  //�����ӽڵ�
			return this->getMaxLeftNode(n->right);
		}
		else {
			Node* parent = n->parent;
			while (parent && parent->left != n) {
				n = parent;
				parent = n->parent;
			}
			return parent;
		}
	}
	//(�ǵݹ�)���л�
	const string serialize_non_recursive() {
		if (!this->m_Size) return "#_";
		string str;
		stack<Node*> sk;
		sk.push(this->root);
		while (!sk.empty()) {
			Node* temp = sk.top();
			sk.pop();
			if (!temp) {
				str += "#_";
				continue;
			}
			if (temp->right) sk.push(temp->right);
			else sk.push(nullptr);
			if (temp->left) sk.push(temp->left);
			else sk.push(nullptr);
			str += to_string(temp->val) + "_";
		}
		return str;
	}
	//���л�
	inline const string serialize_recursive() {
		return this->serialize(this->root);
	}
	//�����л�
	static Tree deserialize_recursive(const string data) {
		queue<string> qe = Tree::dealString(data);
		int size = qe.size();
		return Tree(Tree::deserialize(qe), size);
	}
	//��ֽ����
	inline static void print_origami(const int deepth) { Tree::origami(1, deepth, true); }
	//��������
	~Tree() {
		if (!this->m_Size) return;
		stack<Node*> sk;  //����һ��ջ
		sk.push(this->root);  //���ڵ���ջ
		while (!sk.empty()) {
			Node* temp = sk.top();
			sk.pop();
			if (temp->right) sk.push(temp->right);
			if (temp->left) sk.push(temp->left);
			//cout << "delete: " << temp->val << endl;
			delete temp;
		}
	}
private:
	Node* root = nullptr;
	size_t m_Size = 0;
	void preT(const Node* p) {
		if (!p) return;
		cout << p->val << ' ';
		if (p->left) this->preT(p->left);
		if (p->right) this->preT(p->right);
	}
	void inT(const Node* p) {
		if (!p) return;
		if (p->left) this->inT(p->left);
		cout << p->val << ' ';
		if (p->right) this->inT(p->right);
	}
	void postT(const Node* p) {
		if (!p) return;
		if (p->left) this->postT(p->left);
		if (p->right) this->postT(p->right);
		cout << p->val << ' ';
	}
	int maxDeepth(const Node* p) {
		if (!p) return 0;
		int max = 1;
		int leftMax = this->maxDeepth(p->left);
		int rightMax = this->maxDeepth(p->right);
		max += leftMax >= rightMax ? leftMax : rightMax;
		return max;
	}
	struct returnData {
		returnData(const bool b, const int Min, const int Max) : f(b), min(Min), max(Max) { }
		bool f = false;
		int min = 0, max = 0;
	};
	returnData bst(Node* n) {
		if (!n) return returnData(true, INT_MIN, INT_MAX);
		returnData leftData = bst(n->left);
		returnData rightData = bst(n->right);
		int min = n->val, max = n->val;
		if (leftData.min != INT_MIN || leftData.max != INT_MAX) {
			//min = min < leftData.min ? min : leftData.min;
			max = max > leftData.max ? max : leftData.max;
		}
		if (rightData.min != INT_MIN || rightData.max != INT_MAX) {
			min = min < rightData.min ? min : rightData.min;
			//max = max > rightData.max ? max : rightData.max;
		}
		bool b = true;
		if ((leftData.min != INT_MIN || leftData.max != INT_MAX)  //�ж�leftData�Ƿ�Ϊ��
			&& (!leftData.f || leftData.max >= n->val)) {
			b = false;
		}
		if ((rightData.min != INT_MIN || rightData.max != INT_MAX)  //�ж�rightData�Ƿ�Ϊ��
			&& (!rightData.f || rightData.min <= n->val)) {
			b = false;
		}
		return returnData(b, min, max);
	}
	struct data {
		data(const bool b, const int d) : f(b), deepth(d) { }
		bool f = false;
		int deepth = 0;
	};
	data bbt(Node* n) {
		if (!n) return data(true, 0);
		data d1 = this->bbt(n->left);
		data d2 = this->bbt(n->right);
		if (!d1.f || !d2.f) return data(false, 0);
		if (abs(d1.deepth - d2.deepth) >= 2) {
			return data(false, 0);
		}
		return data(true, max(d1.deepth, d2.deepth) + 1);
	}
	struct Info {
		Info(const int h, const int ns) : height(h), nodes(ns) { }
		int height = 0, nodes = 0;
	};
	Info fbt(Node* n) {
		if (!n) return Info(0, 0);
		Info leftInfo = fbt(n->left);
		Info rightInfo = fbt(n->right);
		int height = max(leftInfo.height, rightInfo.height) + 1;
		int nodes = leftInfo.nodes + rightInfo.nodes + 1;
		return Info(height, nodes);
	}
	void process(Node* n, unordered_map<Node*, Node*>& fatherMap) {
		if (!n) return;
		fatherMap.insert(make_pair(n->left, n));
		fatherMap.insert(make_pair(n->right, n));
		this->process(n->left, fatherMap);
		this->process(n->right, fatherMap);
	}
	Node* lca(Node* head, Node* n1, Node* n2) {
		if (!head || head == n1 || head == n2) return head;
		Node* left = this->lca(head->left, n1, n2);
		Node* right = this->lca(head->right, n1, n2);
		if (left && right) return head;
		return left ? left: right;
	}
	Node* getMaxLeftNode(Node* n) {
		if (!n) return nullptr;
		while (n->left) {
			n = n->left;
		}
		return n;
	}
	const string serialize(Node* n) {
		if (!n) return "#_";
		string str = to_string(n->val) + "_";
		str += this->serialize(n->left);
		str += this->serialize(n->right);
		return str;
	}
	static queue<string> dealString(string str) {
		int size = 0;
		string* strs = split(str, "_", size);
		queue<string> ret;
		for (int i = 0; i < size; ++i) ret.push(strs[i]);
		delete[] strs;
		return ret;
	}
	static Node* deserialize(queue<string>& qe) {
		string temp = qe.front();
		qe.pop();
		if (temp == "#") return nullptr;
		Node* head = new Node(atoi(temp.c_str()));
		head->left = Tree::deserialize(qe);
		head->right = Tree::deserialize(qe);
		return head;
	}
	static void origami(const int i, const int deepth, const bool down) {
		if (i > deepth) return;
		Tree::origami(i + 1, deepth, true);
		cout << (down ? "��" : "͹");
		Tree::origami(i + 1, deepth, false);
	}
};