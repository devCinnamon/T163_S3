#pragma once

#include <cassert>
#include "Array.h"

template <class T>
class OrderedArray : public Array<T>
{
public:

	// constructor(s)
	explicit OrderedArray(int size, int grow = 1) : Array<T>(size)
	{
		m_isDublicated = true;
	}

	OrderedArray(int size, const bool isDublicated, int grow = 1) : Array<T>(size, grow), m_isDublicated(false)
	{
		m_isDublicated = isDublicated;
	}

	// general funcs
	int push(T num)
	{
		assert(Array<T>::m_pArr != nullptr);
		
		int i = 0;

		if (Array<T>::m_counter >= Array<T>::m_maxSize)
		{
			Array<T>::expand();
		}
		
		for (int i = 0; i < Array<T>::m_counter; i++)
		{
			if (Array<T>::m_pArr[i] == num && !m_isDublicated)
				return -1;
			
			if (Array<T>::m_pArr[i] > num)
				break;
			
		}

		for (int k = Array<T>::m_counter; k > i; k--)
		{
			Array<T>::m_pArr[k] = Array<T>::m_pArr[k - 1];
		}
		
		Array<T>::m_pArr[i] = num;

		++Array<T>::m_counter;

		return i;
	}

	int search(T searchKey) { return binarySearch(searchKey, 0, Array<T>::m_counter - 1); }

	// getter/setter
	bool get_isDublicated() const { return m_isDublicated; }
	void set_isDublicated(bool duplicateFlag) { m_isDublicated = duplicateFlag; }
	
	

private:
	bool m_isDublicated;
};