#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"
using namespace std;




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
    cout <<"End of perfomance test."<<endl;
}

void Test_0()
{
	int *x = (int*) My_memory_pool::my_new(sizeof(int));
	*x = 99;
	cout <<"*x: " << *x << endl;

	My_memory_pool::my_delete(x);
	cout << " *x after delete: " << *x << endl;
}


void Test_1()
{
	int *i = new int (5);

	void *place_2 = (void*) i - 2; //place - 2;
	cout <<"my info: " << *(unsigned short int*)place_2 << endl;

	delete i;

	cout << "after free: *i " << *i << endl;
	cout <<"my info: " << *(unsigned short int*)place_2 << endl;
}



void Test_2()
{
    int *i = new int(10);
    double *d = new double(10.5);
    char *c = new char('C');

    cout << *i << " || " << *d << " || " << *c << endl;

    delete i; delete d; delete c;
}


