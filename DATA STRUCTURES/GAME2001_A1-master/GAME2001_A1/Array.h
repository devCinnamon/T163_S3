#pragma once

#include <cassert>

template<class T>

class Array
{
public:
    // constructor
    explicit Array(const int size) : m_pArr(nullptr), m_maxSize(0), m_growSize(1), m_counter(0)
	{
        if (size == true)
        {
            this->m_maxSize = size;
            this->m_pArr = new T[m_maxSize];
            memset(this->m_pArr, 0, sizeof(T) * this->m_maxSize);
        }
    }

    // destructor
    ~Array()
	{
        // clearing memory to be not Google Chrome
        if (this->m_pArr != nullptr)
            delete[] this->m_pArr; this->m_pArr = nullptr;
    }

    // general funcs
    void pop()
	{
        if (this->m_counter > 0)
            --this->m_counter;
    }


    void remove(const int index)
	{
        assert(this->m_pArr != nullptr);
        
        for (int i = index; i < this->m_counter; i++)
        {
            if (i + 1 < this->m_counter)
                this->m_pArr[i] = this->m_pArr[i + 1];
            
        }
        --this->m_counter;
    }

    int search(T val)
	{
        assert(this->m_pArr != nullptr);
        
        for (int i = 0; i < this->m_counter; i++)
        {
            if (this->m_pArr[i] == val)
                return i;
        }
        return -1;
    }

	bool expand()
	{
        if (this->m_growSize <= 0)
            return false;
        
        T * temp = new T[this->m_maxSize + this->m_growSize];
        assert(temp != nullptr);
        
        memcpy(temp, this->m_pArr, sizeof(T) * this->m_maxSize);
        delete[] this->m_pArr;
        
        this->m_pArr = temp;
        temp = NULL;

        this->m_maxSize += this->m_growSize;

        return true;
    }
    
    void clear() { this->m_counter = 0; }

    // getters / setters
    int getSize() const { return this->m_counter; }
    int getMaxSize() const { return this->m_maxSize; }
    int getGrowSize() const { return this->m_growSize; }
    void setGrowSize(const int val) { assert(val >= 0); this->m_growSize = val; }

	// operator
	T& operator[](int index)
	{
		assert(this->m_pArr != nullptr && index < this->m_counter);
		return this->m_pArr[index];
	}

protected:

    T *m_pArr;

    int m_maxSize;
    int m_growSize;
    int m_counter;
};