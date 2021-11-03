#include <iostream>

#include "Array.h"
#include "UnorderedArray.h"
#include "OrderedArray.h"

using namespace std;

int main(int argc, char* argv[])
{
	UnorderedArray<int> unorderedArray(3);
	
	srand (time(NULL));
	constexpr int arrSize = 10;

	for (int i = 0; i < arrSize; i++)
		unorderedArray.push(rand() % 100 + 1);
	unorderedArray.push(19);
	
	cout << "Poping into array...\n";
	unorderedArray.pop();
	cout << "Unordered array : ";
	for (int i = 0; i < unorderedArray.getSize(); i++)
		cout << unorderedArray[i] << ", ";
	cout << "\n\n";
	
	cout << "Removing 3rd element out of array...\n";
	unorderedArray.remove(3);
	cout << "Unordered array : ";
	for (int i = 0; i < unorderedArray.getSize(); i++)
		cout << unorderedArray[i] << ", ";
	cout << "\n\n";


	unorderedArray.search(19);
	cout << "Element 19 is... : ";
	cout << unorderedArray.search(52) << "\n\n";


	OrderedArray<int> orderedArray(3);
	for (int i = 0; i < arrSize; i++)
		orderedArray.push(rand() % 100 + 1);
	orderedArray.push(19);

	cout << "Ordered array elemeents : ";
	for (int i = 0; i < orderedArray.getSize(); i++)
		cout << orderedArray[i] << ", ";
	
}
