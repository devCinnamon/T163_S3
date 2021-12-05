#pragma once

#include <cassert>
#include "Node.h"

template <class T>
class PriorityQueue;

template <class T>

class LinkedList
{
	// TODO: figure out how to do not use size.
	int size = 0;

	Node<T> * first = nullptr;
	Node<T> * last = nullptr;

	Iterator<Node<T>> iterator;

public:

	friend class PriorityQueue<T>;

	/* destructor */

	~LinkedList() { delete this->first; }

	/* constructor */

	LinkedList() : first(nullptr), last(nullptr) { }
	
	/* getter */

	/* main functionality */

	Node<T>* begin()
	{
		assert(first != nullptr);
		return first;
	}

	Node<T>* end()
	{
		return nullptr;
	}

	Node<T>* f_last()
	{
		assert(last != nullptr);
		return last;
	}

	void push(T data)
	{
		Node<T>* node;

		if (this->size == 0)
		{
			node = new Node<T>(data);
			this->first = node;
			this->last = node;
			++this->size;
		}
		else
		{
			node = new Node<T>(data, this->first);
			this->first->setPrevious(node);
			this->first = node;
			++this->size;
		}
	}
	
	T pop()
	{
		if (this->size > 0)
		{
			T tempData = this->first->getData();

			if (this->first->getNext() != nullptr)
			{
				Node<T>* tempFirst = this->first->getNext();

				delete this->first;
				this->first = tempFirst;
				tempFirst = nullptr;
			}
			else
			{
				delete this->first;
				this->first = nullptr;
				this->last = nullptr;
			}

			--this->size;
			return tempData;
		}

		return {};
	}
	
	T front()
	{
		return this->first->getData();
	}
	int getSize()
	{ return this->size; }

};

template <class T>

class Iterator
{

	T* node = nullptr;
	int position = 0;

public:

	friend class PriorityQueue<T>;
	
	~Iterator()
	= default;

	Iterator()  { this->node = nullptr; }


	T* getCurrent()
	{ return this->node; }


	int getPosition() const
	{ return this->position; }


	void setCurrent(T* node)
	{ this->node = node; }


	void setPosition(int position)
	{ this->position = position; }

	
	
	void iterateFirst(T* node)
	{
		this->node = node;
		++this->position;
	}
	
	void iterateLast(T* node)
	{
		this->node = node;
		--this->position;
	}
};

