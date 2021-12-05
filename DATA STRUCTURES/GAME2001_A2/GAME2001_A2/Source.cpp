#include <cstdlib>
#include <ctime>
#include <iostream>

#include "PriorityQueue.h"
#include "LinkedList.h"

int main(int argc, char* argv[])
{
	srand((unsigned) time(0));
	PriorityQueue<int> priorityQueue;

	priorityQueue.push((rand() % 100) + 1, 1);
	std::cout << priorityQueue.front() << std::endl;

	priorityQueue.push((rand() % 100) + 1, 2);
	std::cout << priorityQueue.front() << std::endl;
	
	priorityQueue.push((rand() % 100) + 1, 4);
	std::cout << priorityQueue.front() << std::endl;

	priorityQueue.push((rand() % 100) + 1, 1);
	std::cout << priorityQueue.front() << std::endl;
	

	return 0;
}
