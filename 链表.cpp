#include<iostream>
using namespace std;
#include<unordered_set>

struct Node {
	Node() = default;
	Node(const int v) : val(v) { }
	Node(const Node& other) : Node(other.val) { }
	inline const Node& operator=(const Node& n) {
		this->val = n.val;
	}

	Node* next = nullptr;
	int val = 0;
};

class List {
public:
	explicit List() = default;
	explicit List(const int val) : head(new Node(val)), m_Size() { }
	explicit List(Node* np, const size_t s) : head(np), m_Size(s) { }
	List(initializer_list<int>&& initlist) : List(nullptr, initlist.size()) {
		if (!this->m_Size) return;
		this->head = new Node(*initlist.begin());
		Node* p = this->head;
		for (auto it = initlist.begin() + 1; it != initlist.end(); ++it) {
			p->next = new Node(*it);
			p = p->next;
		}
	}
	//拷贝构造
	List(const List& other) : List() {
		if (!other.m_Size) return;
		unordered_set<Node*> hash_set;
		bool flag = false;
		Node* p = new Node(other.head->val), *h = other.head;
		this->head = p;
		hash_set.insert(other.head);
		while (h->next) {
			h = h->next;
			if (hash_set.count(h)) {
				flag = true;
				break;
			}
			else {
				p->next = new Node(h->val);
				hash_set.insert(h);
			}
			p = p->next;
		}
		if (flag) p->next = this->head;
		this->m_Size = other.m_Size;
	}
	//赋值
	const List& operator=(const List& other) {
		this->del();
		if (!other.m_Size) return *this;
		unordered_set<Node*> hash_set;
		bool flag = false;
		Node* h = other.head;
		this->head = new Node(h->val);
		if (!h->next) return *this;
		Node* p = this->head;
		h = h->next;
		hash_set.insert(other.head);
		while (h) {
			if (hash_set.count(h)) {
				flag = true;
				break;
			}
			else {
				p->next = new Node(h->val);
				hash_set.insert(h);
			}
			p = p->next;
			h = h->next;
		}
		if (flag) p->next = this->head;
		this->m_Size = other.m_Size;
		return *this;
	}
	//头部添加节点(无环)
	Node* push_front(const int val) {
		return this->add(0, val, true);
	}
	//尾部添加节点(无环)
	Node* push_back(const int val) {
		if (!this->m_Size) return this->add(0, val, true);
		return this->add(this->m_Size - 1, val, false);
	}
	//添加节点(无环)
	Node* add(const int index, const int val, const bool front = true) {
		if (index < 0 || index > m_Size) return nullptr;  //越界
		Node* temp = new Node(val);
		if (front && !index) {  //如果要在链表前面加一个节点
			temp->next = this->head;
			this->head = temp;
			++this->m_Size;
			return temp;
		}
		Node* p = this->head;
		for (int i = 0; i < index - front; ++i) p = p->next;
		if (!p) return nullptr;
		if (!front) {  //如果要在接节点后面加一个节点
			temp->next = p->next;
			p->next = temp;
			++this->m_Size;
			return temp;
		}
		temp->next = p->next;
		p->next = temp;
		++this->m_Size;
		return temp;
	}
	//打印链表
	void print() const {
		if (!this->head) return;
		Node* p = this->head;
		while (p->next) {
			cout << p->val << ' ';
			p = p->next;
		}
		cout << p->val << endl;
	}
	//返回大小
	inline size_t size() const { return this->m_Size; }
	//反转链表(无环)
	void reverse() {
		if (this->m_Size <= 1) return;
		Node* old = nullptr, *next = this->head->next;
		while (next) {
			this->head->next = old;
			old = this->head;
			this->head = next;
			next = next->next;
		}
		this->head->next = old;
	}
	//判断是否为回文链表(无环)
	bool palindrome() const {
		Node* q = this->head, *s = this->head;  //快慢指针
		size_t t = 0;
		while (q->next) {
			q = q->next;
			if (q->next) q = q->next;
			else {
				if (s->val != s->next->val) return false;
				s = s->next->next;
				break;
			}
			s = s->next;
			++t;
		}
		Node* old = s, *next = s->next;
		if (!next) return true;
		s = s->next;
		while (next) {
			next = next->next;
			s->next = old;
			old = s;
			s = next;
		}  // s = next = nullptr , old = end
		next = old->next;
		Node* front = this->head;
		bool f = true;
		for (int i = 0; i < t; ++i) {
			if (front->val != q->val) {
				f = false;
				break;
			}
			front = front->next;
			q = q->next;
		}
		for (int i = 0; i < t; ++i) {
			old->next = s;
			s = old;
			old = next;
			next = next->next;
		}
		return (true && f);
	}
	//删除节点(无环)
	bool erase(const int index) {
		if (!this->m_Size) return false;
		if (index < 0 || index >= this->m_Size) return false;
		Node* p = this->head;
		if (!index) {
			Node* temp = this->head;
			this->head = this->head->next;
			delete temp;
			--this->m_Size;
			return true;
		}
		for (int i = 0; i < index - 1; ++i) p = p->next;
		Node* temp = p->next;
		p->next = temp->next;
		delete temp;
		--this->m_Size;
		return true;
	}
	//删除头部节点(无环)
	inline bool pop_front() {
		return this->erase(0);
	}
	//删除尾部节点(无环)
	inline bool pop_back() {
		return this->erase(this->m_Size - 1);
	}
	//特殊拷贝(无环)
	List copy() const {
		if (this->m_Size <= 1) return *this;
		Node* p = this->head, *old = nullptr;
		while (p) {
			old = p->next;
			p->next = new Node(p->val);
			p->next->next = old;
			p = old;
		}
		p = this->head;
		if (p) old = p->next;  //p必定不为nullptr
		Node* h = old, *temp = nullptr;
		while (old) {
			temp = (old->next == nullptr ? nullptr : p->next->next);
			old->next = temp == nullptr ? nullptr : temp->next;
			//特殊成员拷贝操作...
			old = old->next;
			p->next = temp;
			p = temp;
		}
		return List(h, this->m_Size);
	}
	//两个链表第一次相交的节点
	const Node* first_intersect_node(const List& other) const {
		if (!this->m_Size || !other.m_Size) return nullptr;
		const Node* s1 = this->head, *f1 = this->head;
		const Node* s2 = other.head, *f2 = other.head;
		bool rings[2] = { false, false };  //两个链表是否有环
		size_t ts[2] = { 0, 0 };  //记录距离第一个入环点的长度
		bool intersect = false;  //是否相交
		const Node* ns[4] = { s1, f1, s2, f2 };  //两个链表用到的快慢指针
		//判断两个链表有无环
		for (int i = 0; i < 2; ++i) {
			while (true) {
				ns[i * 2 + 1] = ns[i * 2 + 1]->next;
				if (!ns[i * 2 + 1]) break;
				ns[i * 2 + 1] = ns[i * 2 + 1]->next;
				if (!ns[i * 2 + 1]) break;
				ns[i * 2] = ns[i * 2]->next;
				if (ns[i * 2 + 1] == ns[i * 2]) {
					rings[i] = true;
					break;
				}
			}
		}
		//如果只有一个链表有环  肯定不相交
		if (rings[0] && !rings[1] || !rings[0] && rings[1]) return nullptr;
		//如果两个列表都有环  判断各自入环点和是否相交
		if (rings[0] && rings[1]) {
			f1 = this->head;
			f2 = other.head;
			for (int i = 0; i < 2; ++i) {
				while (ns[i * 2 + 1] != ns[i * 2]) {
					ns[i * 2 + 1] = ns[i * 2 + 1]->next;
					ns[i * 2] = ns[i * 2]->next;
					++ts[i];
				}
			}  //此时 ns[0] ns[1] 为this链表的入环点  ns[2] ns[3] 为other链表的入环点
			if (ns[0] == ns[2]) intersect = true;  //判断是否相交
			else return nullptr;  //没有相交，返回空指针
		}
		//两个链表都没有环
		if (!rings[0] && !rings[1]) {
			//判断是否相交
			f1 = this->head;
			f2 = other.head;
			for (int i = 0; i < 2; ++i) {
				if (!ns[i * 2 + 1]) return nullptr;  //ns[i*2+1]绝对不为nullptr，防止警告
				while (ns[i * 2 + 1]->next) {
					ns[i * 2 + 1] = ns[i * 2 + 1]->next;
				}
				if (f1 != f2) return nullptr;  //不相交
				else intersect = true;
			}
		}
		//如果两个链表相交
		if (intersect) {
			if (ts[0] == ts[1]) return this->head;  //返回other.head也行
			bool f = ts[0] > ts[1];
			f1 = this->head;
			f2 = other.head;
			size_t dir = f ? ts[0] - ts[1] : ts[1] - ts[0];
			for (int i = 0; i < dir; ++i) {
				if (f) f1 = f1->next;
				else f2 = f2->next;
			}  //现在 f1和f2距离入环点的距离相同
			while (f1 != f2) {
				if (f1 == s1 || f2 == s2) {  //其实两个条件总是同时满足
					return s1;  //返回s2也一样
				}
				f1 = f1->next;
				f2 = f2->next;
			}
			return f1;  //返回f2也一样
		}
	}
	//析构函数
	~List() {
		this->del();
	}
private:
	Node* head = nullptr;
	size_t m_Size = 0;

	void del() {
		unordered_set<Node*> hash_set;
		bool b = true;
		if (!this->head) return;
		Node* old = this->head;
		while (this->head->next) {
			this->head = this->head->next;
			hash_set.insert(old);
			if (hash_set.count(old)) {
				b = false;
				break;
			}
			else delete old;
			old = this->head;
		}
		if (b) delete this->head;  //析构最后一个
	}
};