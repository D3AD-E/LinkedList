#pragma once
#include <stdexcept>
template<class Key, class Info>
class Sequence
{
private:
	struct Node
	{
		Key key;
		Info info;
		Node* next;
	};

public:
	class NodeWrapper
	{

	friend class Sequence;

	public:
		Key &key;
		Info &info;

		NodeWrapper& operator=(const NodeWrapper& Node)
		{
			this->key = Node.key;
			this->info = Node.info;
			return *this;
		}

		void swap(const NodeWrapper& Node)
		{
			Key ktemp = this->key;
			Info itemp = this->info;
			this->key = Node.key;
			this->info = Node.info;
			Node.key = ktemp;
			Node.info = itemp;
		}

	private:
		NodeWrapper(Node& Node) :
			key(Node.key),
			info(Node.info)
		{ }
	};

	class Iterator
	{
		friend class Sequence;
	public:


		Iterator& operator=(const Node* Node)
		{
			this->CurrentNode = Node;
			return *this;
		}

		Iterator& operator++()
		{
			if (CurrentNode)
				CurrentNode = CurrentNode->next;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator iterator = *this;
			++* this;
			return iterator;
		}

		Iterator operator+(int count)
		{
			Iterator temp(CurrentNode);
			for (; count > 0; count--)
				temp++;
			return temp;
		}

		bool operator!=(const Iterator& iterator)
		{
			return CurrentNode != iterator.CurrentNode;
		}

		bool operator==(const Iterator& iterator)
		{
			return CurrentNode == iterator.CurrentNode;
		}

		Key key()
		{
			return CurrentNode->key;
		}

		Info info()
		{
			return CurrentNode->info;
		}
	private:
		const Node* CurrentNode;

		Iterator(const Node* Node) noexcept :
			CurrentNode(Node) { }
	};

public:
	Sequence()
	{
		head = nullptr;
	}
	Sequence(const Sequence &other)
	{
		this->head = nullptr;
		if (other.empty())
		{
			return;
		}
		Node* thisnode = this->head;
		Node* othernode = other.head;
		while (othernode)
		{
			Node* node = new Node;
			node->key = othernode->key;
			node->info = othernode->info;
			node->next = nullptr;
			if (empty())
			{
				head = node;
				thisnode = head;
			}
			else
			{
				thisnode->next = node;
				thisnode = node;
			}
			othernode = othernode->next;
		}
	}
	Sequence(Iterator first, Iterator last)
	{
		if (!isIteratorBefore(first, last))
			throw std::logic_error("Wrong position of iterators");
		else
		{
			*this = copy(first, last);
		}
	}
	~Sequence()
	{
		clear();
	}

	Sequence& operator=(const Sequence& other)
	{
		if (this == &other)
			return *this;
		clear();
		Node* thisnode = head;

		Node* othernode = other.head;
		while (othernode)
		{
			Node* node = new Node;
			node->key = othernode->key;
			node->info = othernode->info;
			node->next = nullptr;
			if (empty())
			{
				head = node;
				thisnode = head;
			}
			else
			{
				thisnode->next = node;
				thisnode = node;
			}
			othernode = othernode->next;

		}
		return *this;
	}
	Sequence& operator+=(const Sequence& other)
	{
		if (this == &other || other.empty())
		{
			return *this;
		}
		else if (this->empty())
		{
			*this = other;
			return *this;
		}
		else
		{
			Node* thisnode = this->head;
			Node* othernode = other.head;

			while (thisnode->next)
			{
				thisnode = thisnode->next;
			}

			while (othernode)
			{
				Node* node = new Node;
				node->key = othernode->key;
				node->info = othernode->info;
				node->next = nullptr;

				thisnode->next = node;
				thisnode = node;

				othernode = othernode->next;
			}
			return *this;
		}
	}
	Sequence operator+(const Sequence& other)
	{
		if (other.empty())
		{
			Sequence result(*this);
			return result;
		}
		else if (this->empty())
		{
			Sequence result(other);
			return result;
		}
		else
		{
			Sequence ThisSequence(*this);
			Node* thisnode = ThisSequence.head;
			Node* othernode = other.head;
			while (thisnode->next)
			{
				thisnode = thisnode->next;
			}

			while (othernode)
			{
				Node* node = new Node;
				node->key = othernode->key;
				node->info = othernode->info;
				node->next = nullptr;

				thisnode->next = node;
				thisnode = node;

				othernode = othernode->next;
			}
			return ThisSequence;
		}
	}
	NodeWrapper operator[](const int index)
	{
		if (empty())
			throw std::logic_error("List is empty");
		if (!isInRange(index))
			throw std::out_of_range("Index out of range");
		else if (index == -1)
		{
			int _size = size();
			Node* temp = head;
			for (int i = _size-1; i > 0; i--)
			{
				temp = temp->next;
			}
			return NodeWrapper(*temp);
		}
		else
		{
			Node* temp = head;
			for (int i = index; i > 0; i--)
			{
				temp = temp->next;
			}
			return NodeWrapper(*temp);
		}
	};

	Iterator begin() const
	{
		return Iterator(head);
	}
	Iterator end() const 
	{
		return nullptr;
	}
	Iterator find(const Key& key)
	{
		Iterator it(head);
		if (!contains(key))
			return end();
		while (it.key() != key)
		{
			it++;
		}
		return it;
	}
	Iterator erase(Iterator pos)
	{
		if (pos == end())
		{
			return end();
		}
		else if (pos == begin())
		{
			Node* temp = head->next;
			delete head;
			head = temp;
			return Iterator(temp);
		}
		else
		{
			Node* prev = nullptr;
			Node* curr = head;
			Iterator it(head);
			for (; it!=pos; it++)
			{
				prev = curr;
				curr = curr->next;
			}
			prev->next = curr->next;
			delete curr;
			return Iterator(prev);
		}
	}
	Iterator erase(Iterator first, Iterator last)
	{
		if (!isIteratorBefore(first, last))
			throw std::logic_error("Wrong position of iterators");
		if (first == begin() && last == end())
		{
			clear();
			return end();
		}
		while (first!= last)
		{
			Iterator(temp) = first;
			first++;
			erase(temp);
		}
		last++;
		Iterator(temp) = first;
		erase(temp);
		return last;
	}
	Iterator insert(Iterator pos, const Key& key, const Info& info)
	{
		if (pos == begin())
		{
			insertAtHead(key, info);
			return begin();
		}
		else if (pos == end())
		{
			push_back(key, info);
			Iterator it = begin();
			int i = size() - 1;
			for (; i > 0; i--)
			{
				it++;
			}
			return it;
		}
		else
		{
			Node* temp = head;
			Iterator it(temp);
			while (it + 1 != pos)
			{
				temp = temp->next;
				it++;
			}
			Node* nnext = new Node;
			nnext->key = key;
			nnext->info = info;
			nnext->next = temp->next;
			temp->next = nnext;
			Iterator nit(nnext);
			return nit;
		}
	}
	Iterator insert(Iterator pos, const NodeWrapper& nw)
	{
		return insert(pos, nw.key, nw.info);
	}
	Iterator insert(Iterator pos, size_t count, const Key& key, const Info& info)
	{
		if (pos == begin())
		{
			for(; count > 0; count--)
				insertAtHead(key, info);
			return begin();
		}
		else if (pos == end())
		{
			Iterator it = begin();
			int i = size() - 1;
			for (; i > 0; i--)
			{
				it++;
			}
			for (; count > 0; count--)
				push_back(key, info);
			it++;
			return it;
		}
		else
		{
			Node* temp = head;
			Iterator it(temp);
			while (it + 1 != pos)
			{
				temp = temp->next;
				it++;
			}
			for (; count > 0; count--)
			{
				Node* nnext = new Node;
				nnext->key = key;
				nnext->info = info;
				nnext->next = temp->next;
				temp->next = nnext;
			}
			Iterator nit(temp->next);
			return nit;
		}
	}
	Iterator insert(Iterator pos, size_t count, const NodeWrapper& nw)
	{
		return insert(pos,count, nw.key, nw.info);
	}

	Sequence copy(Iterator position) const
	{
		Sequence Seq;
		if (position == end())
		{
			return Seq;
		}
		Seq.push_back(position);
		return Seq;
	}
	Sequence copy(Iterator first, Iterator last) 
	{
		if (!isIteratorBefore(first, last))
			throw std::logic_error("Wrong position of iterators");
		if (first == begin() && last == end())
		{
			Sequence Seq(*this);
			return Seq;
		}
		Sequence Seq;
		for (; first != last; first++)
		{
			Seq.push_back(first);
		}
		if(last != end())
			Seq.push_back(first++);
		return Seq;
	}

	NodeWrapper at(const int index)
	{
		int _size = size();
		if (empty())
			throw std::logic_error("List is empty");
		if(!isInRange(index))
			throw std::out_of_range("Index out of range");
		else if (index == -1)
		{
			Node* temp = head;
			for (int i = _size - 1; i > 0; i--)
			{
				temp = temp->next;
			}
			return NodeWrapper(*temp);
		}
		else
		{
			Node* temp = head;
			for (int i = index; i > 0; i--)
			{
				temp = temp->next;
			}
			return NodeWrapper(*temp);
		}
	}
	NodeWrapper at(Iterator pos)
	{
		if (pos == end())
		{
			throw std::logic_error("End of the list");
		}
		Node* temp = head;
		Iterator it(temp);
		while (it + 1 != pos)
		{
			temp = temp->next;
			it++;
		}
		return NodeWrapper(*temp);
	}
	NodeWrapper front()
	{
		if (empty())
			throw std::logic_error("List is empty");
		else
			return NodeWrapper(*head);
	}
	NodeWrapper back()
	{
		if (empty())
			throw std::logic_error("List is empty");
		else
		{
			Node* temp = head;
			int i = size() - 1;
			for (; i > 0; i--)
			{
				temp = temp->next;
			}
			return NodeWrapper(*temp);
		}
	}

	bool empty() const
	{
		if (begin() == end())
			return true;
		else
			return false;
	}
	bool contains(const Key& key) 
	{
		Iterator it(head);
		while (it!=end())
		{
			if (it.key() == key)
				return true;
			it++;
		}
		return false;
	}
	bool erase_key(const Key& key)
	{
		if (empty() || !contains(key))
		{
			return false;
		}
		else if (head->key == key)
		{
			Node* temp = head->next;
			delete head;
			head = temp;
		}
		else
		{
			Node* prev = nullptr;
			Node* curr = head;

			while (curr->key!=key)
			{
				prev = curr;
				curr = curr->next;
			}
			prev->next = curr->next;
			delete curr;
		}
		return 1;
	}

	void concatenate(const Sequence& other)
	{
		*this += other;
	}
	void push_back(const Key& key, const Info& info)
	{
		Node* temp = new Node;
		temp->info = info;
		temp->key = key;
		temp->next = nullptr;

		if (!head)
		{
			head = temp;
		}
		else
		{
			Node* last = head;
			while (last->next)
				last = last->next;
			last->next = temp;
		}
		return ;
	}
	void push_back(const NodeWrapper& node)
	{
		push_back(node.key, node.info);
		return;
	}
	void pop_back()
	{
		if (empty())
			throw std::logic_error("List is empty");
		else
		{
			if (size() == 1)
			{
				clear();
				return;
			}
			Node* prev = nullptr;
			Node* curr = head;
			int i = size()-1;
			for (; i>0; i--)
			{
				prev = curr;
				curr = curr->next;
			}
			prev->next = curr->next;
			delete curr;
		}
	}
	void swap(Sequence& other)
	{
		Node* tempHead = this->head;
		this->head = other.head;
		other.head = tempHead;
	}
	void swap(NodeWrapper& thisNode, NodeWrapper& otherNode)
	{
		thisNode.swap(otherNode);
	}
	void clear()
	{
		Node* temp = head;
		while (head != nullptr)
		{
			temp = head->next;
			delete head;
			head = temp;
		}
		head = nullptr;
	}
	void reverse()
	{
		Node* current = head;
		Node* prev = nullptr;
		Node* next = nullptr;

		while (current != nullptr) 
		{
			next = current->next;
			current->next = prev;
			prev = current;
			current = next;
		}
		head = prev;
	}

	int size()
	{
		Node* temp = head;
		int size = 0;
		while (temp)
		{
			size++;
			temp = temp->next;
		}
		return size;
	}


private:
	Node* head;

	bool isInRange(int index)
	{
		if (index > (size() - 1) || index < -1)
		{
			return false;
		}
		return true;
	}
	bool isIteratorBefore(Iterator first, Iterator last)
	{
		if (last == end())
			return true;
		for (; first != end(); first++)
			if (first == last)
				return true;
		return false;
	}

	void push_back(Iterator it)
	{
		push_back(it.key(), it.info());
	}
	void insertAtHead(const Key& key, const Info& info)
	{
		Node* nhead = new Node;
		nhead->key = key;
		nhead->info = info;
		nhead->next = head;
		head = nhead;
	}
};
