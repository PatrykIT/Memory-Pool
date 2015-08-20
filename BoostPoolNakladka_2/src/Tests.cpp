#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"




void Performance_Test_Default_New()
{

    for (int i = 0 ; i <50000 ; ++i)
    {
        char *a = new char;
        delete a;
    }

    for (int i = 0 ; i <500000 ; ++i)
    {
        int *a = new int;
        delete a;
    }


    for (int i = 0 ; i <500000 ; ++i)
    {
        double *a = new double;
        delete a;
    }

      struct large_structure {
        int a, b, c, d; double e, f; char g, h;
    };

    for(int i = 0 ; i < 500000; ++i)
    {
        large_structure *x = new large_structure;
        delete x;
    }
}
void Performance_Test_Pool()
{
	for (int i = 0 ; i <50000 ; ++i)
    {
        char *a = memory_pool::MyMemoryPool::my_new<char>();
        memory_pool::MyMemoryPool::my_delete(a);
    }

    for (int i = 0 ; i <500000 ; ++i)
    {
        int *a = memory_pool::MyMemoryPool::my_new<int>();
        memory_pool::MyMemoryPool::my_delete(a);
    }


    for (int i = 0 ; i <500000 ; ++i)
    {
        double *a = memory_pool::MyMemoryPool::my_new<double>();
        memory_pool::MyMemoryPool::my_delete(a);
    }

      /*struct large_structure {
        int a, b, c, d; double e, f; char g, h;
    };

    for(int i = 0 ; i < 500000; ++i)
    {
        large_structure *a = (large_structure*) memory_pool::MyMemoryPool::my_new(sizeof(large_structure));
        memory_pool::MyMemoryPool::my_delete(a);
    }*/
}

void Performance_Test_Default_New_2()
{
	int size = 12000, size_2 = size / 2;

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

void Performance_Test_Pool_2()
{
	/*int size = 12000, size_2 = size / 2;

	int *tab_of_ints[size]; char *tab_of_chars[size]; 	double *tab_of_doubles[size];
	for (int i = 0; i < size_2; ++i)
	{
		tab_of_ints[i] = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));
	}

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_chars[i] = (char*) memory_pool::MyMemoryPool::my_new(sizeof(char));
	}

	for (int i = 0; i < size_2; ++i)
	{
		tab_of_doubles[i] = (double*) memory_pool::MyMemoryPool::my_new(sizeof(double));
	}


	for (int i = size_2; i < size; ++i)
	{
		tab_of_ints[i] = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));
	}

	for (int i = size_2; i < size; ++i)
	{
		tab_of_doubles[i] = (double*) memory_pool::MyMemoryPool::my_new(sizeof(double));
	}

	for (int i = size_2; i < size; ++i)
	{
		tab_of_chars[i] = (char*) memory_pool::MyMemoryPool::my_new(sizeof(char));
	}

	memory_pool::MyMemoryPool::my_pools_vector[0]->purge_memory();
	memory_pool::MyMemoryPool::my_pools_vector[1]->purge_memory();
	memory_pool::MyMemoryPool::my_pools_vector[2]->purge_memory();*/


	/*for (int i = 0; i < size; ++i)
	{
		Memory_Pool::MyMemoryPool::my_delete(tab_of_ints[i]);
		Memory_Pool::MyMemoryPool::my_delete(tab_of_chars[i]);
		Memory_Pool::MyMemoryPool::my_delete(tab_of_doubles[i]);
	}*/
}

void Test_0()
{
	//int *x = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));
	int *x = memory_pool::MyMemoryPool::my_new<int>();

	if ( x == NULL)
		return;

	*x = 99;
	std::cout <<"*x: " << *x << std::endl;
	memory_pool::MyMemoryPool::my_delete(x);
	std::cout << "*x after delete: " << *x << std::endl;
}

void Test_1()
{
	/*int *x, *y, *a, *b;

	x = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));
	y = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));
	a = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));
	b = (int*) memory_pool::MyMemoryPool::my_new(sizeof(int));

	*y = 10; *b = 6; *a = 2; *x = -15;

	double *q, *w, *e;
	q = (double*) memory_pool::MyMemoryPool::my_new(sizeof(double));
	w = (double*) memory_pool::MyMemoryPool::my_new(sizeof(double));
	e = (double*) memory_pool::MyMemoryPool::my_new(sizeof(double));

	*q = 7.5; *w = 8.9; *e = -23.4;

	printf("My vars: %d | %d | %d | %d\n%f | %f | %f\n", *y, *b, *a, *x, *q, *w, *e);

	memory_pool::MyMemoryPool::my_delete(x);
	memory_pool::MyMemoryPool::my_delete(y);
	memory_pool::MyMemoryPool::my_delete(a);
	memory_pool::MyMemoryPool::my_delete(b);

	memory_pool::MyMemoryPool::my_delete(q);
	memory_pool::MyMemoryPool::my_delete(e);
	memory_pool::MyMemoryPool::my_delete(w);

	printf("My vars after delete: %d | %d | %d | %d\n%f | %f | %f\n", *y, *b, *a, *x, *q, *w, *e);*/


}
