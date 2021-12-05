#pragma once

#include "LinkedList.h"

template <class T>

class PriorityQueue : public LinkedList<T>
{

	LinkedList<T>* linkedList = nullptr;

public:

	/* destructor */

	~PriorityQueue()
	= default;

	/* constructor */

	PriorityQueue() { this->linkedList = new LinkedList<T>; }

	/* main functionality */

	/*void pop()
	{
		assert(this->linkedList->first != nullptr);

		Node<T>* node = this->linkedList->first->next;
		delete this->linkedList->first;

		this->linkedList->first = node;
		node = nullptr;

		--this->linkedList->size;
	}*/


void push(T data, int priority)
{
	Node<T>* node = new Node<T>(data, priority);

	if (this->size == 0)
	{
		this->first = this->last = node;
		++this->size;
	}
	else
	{
		this->iterator.setCurrent(this->first);
		this->iterator.setPosition(0);

		while(this->iterator.getCurrent() != nullptr)
		{
			if (this->iterator.getCurrent()->getPriority() >= priority)
			{
				if (this->iterator.getCurrent()->getPrevious() == nullptr)
				{
					this->first = node;
					node->setPrevious(nullptr);
				}
				else
					this->iterator.getCurrent()->getPrevious()->setNext(node);
					
				node->setPrevious(this->iterator.getCurrent()->getPrevious());
				node->setNext(this->iterator.getCurrent());
				this->iterator.getCurrent()->setPrevious(node);
				++this->size;
				return;
			}
			this->iterator.iterateFirst(this->iterator.getCurrent()->getNext());
		}

		this->last->setNext(node);
		node->setPrevious(this->last);
		node->setNext(nullptr);
		this->last = node;
		++this->size;
	}
}
};