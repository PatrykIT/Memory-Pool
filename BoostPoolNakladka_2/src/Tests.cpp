#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"




void Perfomance_Test()
{

    for (int i = 0 ; i <10000 ; ++i)
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

void Test_0()
{
	int *x = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));

	if ( x == NULL)
		return;

	*x = 99;
	std::cout <<"*x: " << *x << std::endl;

	Memory_Pool::MyMemoryPool::my_delete(x);
	std::cout << " *x after delete: " << *x << std::endl;
}


void Test_1()
{
	int *i = new int (5);

	void *place_2 = (void*) i - 2; //place - 2;
	std::cout <<"my info: " << *(unsigned short int*)place_2 << std::endl;

	delete i;

	std::cout << "after free: *i " << *i << std::endl;
	std::cout <<"my info: " << *(unsigned short int*)place_2 << std::endl;
}



void Test_2()
{
    int *i = new int(10);
    double *d = new double(10.5);
    char *c = new char('C');

    std::cout << *i << " || " << *d << " || " << *c << std::endl;

    delete i; delete d; delete c;
}


