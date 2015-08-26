#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"

void Performance_Test_Default_New()
{
	int size = 240000, size_2 = size / 2;

	int *tab_of_ints[size]; char *tab_of_chars[size]; 	double *tab_of_doubles[size];

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_ints[i] = new int;
	}

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_chars[i] = new char;
	}

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_doubles[i] = new double;
	}



	for(int i = size_2; i < size; ++i)
	{
		tab_of_ints[i] = new int;
	}

	for(int i = size_2; i < size; ++i)
	{
		tab_of_doubles[i] = new double;
	}

	for(int i = size_2; i < size; ++i)
	{
		tab_of_chars[i] = new char;
	}



	for (int i = 0; i < size; ++i)
	{
		delete (tab_of_ints[i]);
		delete (tab_of_chars[i]);
		delete (tab_of_doubles[i]);
	}
}

void Performance_Test_Pool()
{
	int size = 240000, size_2 = size / 2;

	int *tab_of_ints[size]; char *tab_of_chars[size]; 	double *tab_of_doubles[size];
	for (int i = 0; i < size_2; ++i)
	{
		tab_of_ints[i] = memory_pool::MyMemoryPool::my_new<int>();
	}

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_chars[i] = memory_pool::MyMemoryPool::my_new<char>();
	}
	 std::cout << "Allocated chuncks by vec[0]: " <<memory_pool::MyMemoryPool::my_pools_vector[0]->get_allocated_chuncks() << "\n";

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_doubles[i] = memory_pool::MyMemoryPool::my_new<double>();
	}



	for (int i = size_2; i < size; ++i)
	{
		tab_of_ints[i] = memory_pool::MyMemoryPool::my_new<int>();
	}

	for (int i = size_2; i < size; ++i)
	{
		tab_of_doubles[i] = memory_pool::MyMemoryPool::my_new<double>();
	}

	for (int i = size_2; i < size; ++i)
	{
		tab_of_chars[i] = memory_pool::MyMemoryPool::my_new<char>();
	}

	for (int i = 0; i < size; ++i)
	{
		memory_pool::MyMemoryPool::my_delete(tab_of_ints[i]);
		memory_pool::MyMemoryPool::my_delete(tab_of_chars[i]);
		memory_pool::MyMemoryPool::my_delete(tab_of_doubles[i]);
	}
}

void Test_0()
{
	int *x = memory_pool::MyMemoryPool::my_new<int>();
	if ( x == NULL)
		return;

	*x = 99;
	std::cout <<"*x: " << *x << std::endl;
	memory_pool::MyMemoryPool::my_delete(x);
	std::cout << "*x after delete: " << *x << std::endl;
}

