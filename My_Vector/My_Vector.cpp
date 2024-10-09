#include <iostream>
#include "Vector.hpp"	
#include <vector>



template<typename T>
void print(Vector<T>& vc)
{
	for (size_t i = 0; i < vc.size(); ++i)
	{
		std::cout << vc[i] << std::endl;
	}
}

int main()
{

	Vector<int> test;
	std::cout << test.capacity() << "\tsize:" << test.size() << std::endl;

	test.push_back(1);
	test.push_back(2);
	test.push_back(3);
	test.push_back(4);


	std::cout << test.capacity() << "\tsize:" << test.size() << std::endl;

	//print<int>(test);

	Vector<int>b(3, 5);
	//print(b);
	auto cop = test+b;
	print(cop);
	

    return 0;
}




