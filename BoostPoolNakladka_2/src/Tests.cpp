#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"




void Performance_Test_Default_New()
{

    for (int i = 0 ; i <500000 ; ++i)
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
    /*for (int i = 0 ; i <500000 ; ++i)
    {
        char *a = (char*) Memory_Pool::MyMemoryPool::my_new(sizeof(char));
        Memory_Pool::MyMemoryPool::my_delete(a);
    }*/

    for (int i = 0 ; i <500000 ; ++i)
    {
        int *a = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));
        Memory_Pool::MyMemoryPool::my_delete(a);
    }


    for (int i = 0 ; i <500000 ; ++i)
    {
        double *a = (double*) Memory_Pool::MyMemoryPool::my_new(sizeof(double));
        Memory_Pool::MyMemoryPool::my_delete(a);
    }

      struct large_structure {
        int a, b, c, d; double e, f; char g, h;
    };

    for(int i = 0 ; i < 500000; ++i)
    {
        large_structure *a = (large_structure*) Memory_Pool::MyMemoryPool::my_new(sizeof(large_structure));
        Memory_Pool::MyMemoryPool::my_delete(a);
    }
}

void Test_0()
{
	int *x = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));

	if ( x == NULL)
		return;

	*x = 99;
	//std::cout <<"*x: " << *x << std::endl;
	Memory_Pool::MyMemoryPool::my_delete(x);
	std::cout << "*x after delete: " << *x << std::endl;
}

void Test_1()
{
	int *x, *y, *a, *b;

	x = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));
	y = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));
	a = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));
	b = (int*) Memory_Pool::MyMemoryPool::my_new(sizeof(int));

	*y = 10; *b = 6; *a = 2; *x = -15;

	double *q, *w, *e;
	q = (double*) Memory_Pool::MyMemoryPool::my_new(sizeof(double));
	w = (double*) Memory_Pool::MyMemoryPool::my_new(sizeof(double));
	e = (double*) Memory_Pool::MyMemoryPool::my_new(sizeof(double));

	*q = 7.5; *w = 8.9; *e = -23.4;

	printf("My vars: %d | %d | %d | %d\n%f | %f | %f\n", *y, *b, *a, *x, *q, *w, *e);

	Memory_Pool::MyMemoryPool::my_delete(x);
	Memory_Pool::MyMemoryPool::my_delete(y);
	Memory_Pool::MyMemoryPool::my_delete(a);
	Memory_Pool::MyMemoryPool::my_delete(b);

	Memory_Pool::MyMemoryPool::my_delete(q);
	Memory_Pool::MyMemoryPool::my_delete(e);
	Memory_Pool::MyMemoryPool::my_delete(w);

	printf("My vars after delete: %d | %d | %d | %d\n%f | %f | %f\n", *y, *b, *a, *x, *q, *w, *e);


}

void Test_2()
{
    int *i = new int(10);
    double *d = new double(10.5);
    char *c = new char('C');

    std::cout << *i << " || " << *d << " || " << *c << std::endl;

    delete i; delete d; delete c;
}


