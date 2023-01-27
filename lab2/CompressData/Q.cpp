class Priority_Queue
{
	vector<Node*> vdata;
public:
	Priority_Queue() { }
	Node* top()
	{
		if (vdata.size() > 0)
			return vdata[0];
		return *vdata.end();
	}
	int size() { return vdata.size(); }
	bool empty() { return vdata.size() < 1; }
	void push(Node* n)
	{
		vdata.push_back(n);
		sort(vdata.begin(), vdata.end(),
			[](Node* a, Node* b)
			{
				return a->key() < b->key();
			});
	}
	void pop()
	{
		vdata.erase(vdata.begin());
	}
	void print()
	{
		for_each(vdata.begin(), vdata.end(),
			[](Node* n)
			{
				cout << n->key() << '\t' << n->value() << endl;
			});
	}
};