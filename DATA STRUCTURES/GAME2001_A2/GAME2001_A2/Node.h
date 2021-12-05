#pragma once

template <class T>
class LinkedList;

template <class T>
class PriorityQueue;

template <class T>
class Iterator;

template <class T>

class Node
{
	
	Node* next;
	Node* previous;
	
	int priority;
	T data;

public:

	/* --- friend classes --- */

	friend class LinkedList<T>;
	friend class PriorityQueue<T>;
	friend class Iterator<T>;

	/* --- destructor --- */

	~Node()
	{
		this->next = nullptr;
		this->previous = nullptr;
	}

	/* --- constructors --- */

	Node(T data, int priority, Node* next)
	{
		this->previous = nullptr;
		this->next = next;

		this->data = data;

		this->priority = priority;
	}

	Node(T data, int priority, Node* next, Node* inputPrev)
	{
		this->previous = next;
		this->next = nullptr;

		this->data = data;

		this->priority = priority;
	}

	Node (T data, int priority)
	{
		this->previous = nullptr;
		this->next = nullptr;

		this->data = data;

		this->priority = priority;
	}

	Node(T data, Node* next)
	{
		this->previous = nullptr;
		this->next = next;

		this->data = data;

		this->priority = 0;
	}

	Node(T data, Node* next, Node* previous)
	{
		this->previous = previous;
		this->next = next;

		this->data = data;
		
		this->priority = 0;
	}

	explicit Node(T data)
	{
		this->previous = nullptr;
		this->next = nullptr;

		this->data = data;
		
		this->priority = 0;
	}

	/* --- getters --- */
		
	Node* getNext() { return this->next; }
	Node* getPrevious() { return this->previous; }
	T getData() { return this->data; }
	int getPriority() const { return this->priority; }

	/* --- setters --- */

	void setNext(Node* next) { this->next = next; }
	void setPrevious(Node* previous) { this->previous = previous; }
	void setData(T data) { this->data = data; }
	void setPriority(int priority) { this->priority = priority; }

};