#pragma once

#include <cassert>
#include "Array.h"

template<class T>
class UnorderedArray : public Array<T> {
public:

    // constructor
    UnorderedArray(int size) : Array<T>(size)
	{
    }

    // destructor
    ~UnorderedArray()
	{
    }

    void push(T num) {
        assert(this->m_pArr != nullptr);

        if (this->m_counter >= this->m_maxSize)
            this->expand();

        this->m_pArr[this->m_counter] = num;
        this->m_counter++;
    }
};