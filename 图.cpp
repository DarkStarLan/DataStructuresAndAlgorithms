#include<iostream>
using namespace std;
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<queue>
#include<stack>
#include<stdarg.h>

class Node;  //����
//���鼯
class UnionFindSets {
public:
	//�вι���
	UnionFindSets(vector<Node*> ns) : setMap() {
		for (auto i : ns) {
			vector<Node*> v;
			v.push_back(i);
			setMap.insert(make_pair(i, v));
		}
	}
	//�ж������ڵ��Ƿ���ͬһ������
	bool isSameSet(Node* from, Node* to) {
		return this->setMap.at(from) == this->setMap.at(to);
	}
	//�ϲ������ڵ����ڵļ���
	void Union(Node* from, Node* to) {
		vector<Node*> fromSet = this->setMap.at(from);
		vector<Node*> toSet = this->setMap.at(to);
		for (auto i : toSet) {
			fromSet.push_back(i);
			this->setMap.insert(make_pair(i, fromSet));
		}
	}
private:
	unordered_map<Node*, vector<Node*>> setMap;
};
//��
class Edge {
	friend class Graph;
	friend class Compare;
	friend class ReCompare;
	friend ostream& operator<<(ostream&, const Edge&);
	//friend void compositeEdges(vector<Edge*>);
public:
	//�вι���
	explicit Edge(Node* f, Node* t, const int Weight) : from(f), to(t), weight(Weight) { }
	//��������
	Edge(const Edge& other) = delete;
	//���رȽ�
	bool operator<(const Edge& other) { return this->weight < other.weight; }
	//��������
	~Edge() {
		//delete this->from;
		//delete this->to;
	}
private:
	Node* from = nullptr, * to = nullptr;  //������  �ִ��
	int weight = 0;  //Ȩ��
};
//�ȽϱߵĴ�С �º���
class Compare {
public:
	bool operator()(const Edge* e1, const Edge* e2) { return e1->weight < e2->weight; }
};
class ReCompare {
public:
	bool operator()(const Edge* e1, const Edge* e2) { return e1->weight > e2->weight; }
};
//�ڵ�
class Node {
	friend class Graph;
	friend class Edge;
public:
	//�вι���
	explicit Node(const int val) : value(val), in(0), out(0), nexts(), edges() { }
	//��������
	Node(const Node& other) = delete;
	//��ӽڵ�
	void addNext(Node* n, const int Weight = 0) {
		this->nexts.push_back(n);
		this->edges.push_back(new Edge(this, n, Weight));
		++this->out;
		++n->in;
	}
	void addNodesWithSameWeight(const int Weight, const size_t size, Node* ...) {
		va_list vl;
		va_start(vl, size);
		for (int i = 0; i < size; ++i) {
			Node* cur =  va_arg(vl, Node*);
			this->nexts.push_back(cur);
			this->edges.push_back(new Edge(this, cur, Weight));
			++this->out;
			cur->addIn();
		}
		va_end(vl);
	}
	//��ȡֵ
	inline const int getValue() const { return this->value; }
	//��������
	~Node() { }
private:
	int value, in, out;  //ֵ  ���  ����
	vector<Node*> nexts;  //�Լ��ܵ���ĵ�
	vector<Edge*> edges;  //�����Լ��ı�

	inline void addIn() { ++this->in; }
};
//ͼ
class Graph {
public:
	//�޲ι���
	Graph() : nodes(), edges() { }
	//��������
	Graph(const Graph& other) : nodes(other.nodes), edges(other.edges) { }
	//��ֵ
	const Graph& operator=(const Graph& other) {
		this->nodes = other.nodes;
		this->edges = other.edges;
	}
	//��ӽڵ�
	void addNode(Node* n) {
		this->nodes.push_back(n);
		for (Edge* e : n->edges) {
			this->edges.push_back(e);
		}
	}
	//��ӽڵ�
	void addNodes(Node** ns, const size_t size) {
		for (int i = 0; i < size; ++i) {
			this->nodes.push_back(ns[i]);
			for (Edge* e : ns[i]->edges) {
				this->edges.push_back(e);
			}
		}
	}
	//��ӡȫ����
	void printAllEdges() {
		for (auto i : this->edges) {
			cout << i->from << " ==> " << i->to << " : " << i->weight << ' ';
		}
		cout << endl;
	}
	//������ȱ���
	void breadth_first_search(Node* n) {
		if (this->nodes.empty()) return;
		queue<Node*> qe;
		unordered_set<Node*> visit;
		qe.push(n);
		visit.insert(n);
		while (!qe.empty()) {
			Node* temp = qe.front();
			qe.pop();
			for (auto i : temp->nexts) {
				if (!visit.count(i)) {
					qe.push(i);
					visit.insert(i);
				}
			}
			cout << temp->value << ' ';
		}
		cout << endl;
	}
	//������ȱ���
	void depth_first_search(Node* n) {
		if (this->nodes.empty()) return;
		stack<Node*> sk;
		unordered_set<Node*> visit;
		sk.push(n);
		visit.insert(n);
		cout << n->value << ' ';
		while (!sk.empty()) {
			Node* temp = sk.top();
			bool isFind = false;
			for (auto i : temp->nexts) {
				if (!visit.count(i)) {
					sk.push(i);
					cout << i->value << ' ';
					visit.insert(i);
					isFind = true;
					break;
				}
			}
			if (!isFind) sk.pop();
		}
		cout << endl;
	}
	//��������
	vector<Node*> topological_sort() {
		queue<Node*> qe;
		unordered_map<Node*, int> hash_map;
		for (auto i : this->nodes) {
			hash_map.insert(make_pair(i, i->in));
			if (!i->in) qe.push(i);
		}
		vector<Node*> result;
		while (!qe.empty()) {
			Node* temp = qe.front();
			qe.pop();
			result.push_back(temp);
			for (Node* i : temp->nexts) {
				--hash_map[i];
				if (!hash_map.at(i)) qe.push(i);
			}
		}
		return result;
	}
	//��С������ k  ����
	unordered_set<Edge*> minimum_spanning_tree_k() {
		UnionFindSets unionfind(this->nodes);
		priority_queue<Edge*, vector<Edge*>, ReCompare> priorityQueue;
		for (auto i : this->edges) priorityQueue.push(i);
		cout << priorityQueue.top()->weight << "TOP";
		unordered_set<Edge*> ret;
		while (!priorityQueue.empty()) {
			Edge* temp = priorityQueue.top();
			priorityQueue.pop();
			if (!unionfind.isSameSet(temp->from, temp->to)) {
				ret.insert(temp);
				unionfind.Union(temp->from, temp->to);
			}
		}
		return ret;
	}
	//��С������ p
	unordered_set<Edge*> minimum_spanning_tree_p() {
		priority_queue<Edge*, vector<Edge*>, Compare> priorityQueue;
		unordered_set<Node*> set;
		unordered_set<Edge*> ret;
		for (auto i : this->nodes) {
			if (!set.count(i)) {
				set.insert(i);
				for (auto j : i->edges) {
					priorityQueue.push(j);
				}
				while (!priorityQueue.empty()) {
					Edge* temp = priorityQueue.top();
					priorityQueue.pop();
					Node* toNode = temp->to;
					if (!set.count(toNode)) {
						set.insert(toNode);
						ret.insert(temp);
						for (auto j : toNode->edges) {
							priorityQueue.push(j);
						}
					}
				}
			}
		}
		return ret;
	}
	//���·��
	unordered_map<Node*, int> dijkstra(Node* head) {
		unordered_map<Node*, int> distanceMap;
		distanceMap.insert(make_pair(head, 0));
		unordered_set<Node*> selectedNodes;
		Node* minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
		while (minNode) {
			int distance = distanceMap.at(minNode);
			for (Edge* e : minNode->edges) {
				Node* toNode = e->to;
				if (!distanceMap.contains(toNode)) {
					distanceMap[toNode] = distance + e->weight;
				}
				distanceMap[e->to] = distanceMap.at(toNode) < distance + e->weight
					? distanceMap.at(toNode) : distance + e->weight;
			}
			selectedNodes.insert(minNode);
			minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
		}
		return distanceMap;
	}
	//��������
	~Graph() {
		for (auto i : this->nodes) delete i;
		for (auto i : this->edges) delete i;  //ɾ���ߵ�ʱ����ɾ�����ϵĵ�
	}
private:
	vector<Node*> nodes;
	vector<Edge*> edges;

	Node* getMinDistanceAndUnselectedNode(unordered_map<Node*, int>& m, unordered_set<Node*> s) {
		if (m.empty()) return nullptr;
		pair<Node*, int> min = *m.begin();
		for (auto i : m) {
			if (!s.count(i.first)) {
				min = i;
				break;
			}
		}
		for (auto i : m) {
			if (min.second > i.second) min = i;
		}
		return min.first;
	}
};
//��ӡ��
ostream& operator<<(ostream& out, const Edge& e) {
	out << e.from->getValue() << " ==> " << e.to->getValue() << " : " << e.weight << ' ';
	return out;
}
/*
int main()
{
	Graph g;
	Node* ns[5] = { new Node(1), new Node(2), new Node(3), new Node(4), new Node(5) };
	ns[0]->addNodesWithSameWeight(1, 3, ns[1], ns[2], ns[4]);
	ns[1]->addNodesWithSameWeight(2, 4, ns[0], ns[2], ns[3], ns[4]);
	ns[2]->addNodesWithSameWeight(3, 3, ns[0], ns[1], ns[3]);
	ns[3]->addNodesWithSameWeight(4, 3, ns[1], ns[2], ns[4]);
	ns[3]->addNodesWithSameWeight(5, 3, ns[0], ns[1], ns[3]);
	g.addNodes(ns, 5);

	auto min1 = g.minimum_spanning_tree_k();
	for (auto i : min1) cout << *i << endl;
	cout << "==========" << endl;
	auto min2 = g.minimum_spanning_tree_p();
	for (auto i : min2) cout << *i << endl;

	return 0;
}
*/