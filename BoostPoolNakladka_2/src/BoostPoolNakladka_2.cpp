#include <cstdlib>
#include <iostream>
#include <string>
#include <new>
#include <utility>
#include <mutex> //call_once
#include <stddef.h>
#include <cstdio>
#include <ctime>
#include <vector>

#include <boost/pool/pool.hpp>
#include <boost/pool/detail/mutex.hpp>
#include <boost/pool/detail/guard.hpp>
#include <boost/pool/pool_alloc.hpp>

using namespace std;


struct default_user_allocator_new_delete
{
  typedef std::size_t size_type; // An unsigned integral type that can represent the size of the largest object to be allocated.
  typedef std::ptrdiff_t difference_type; // A signed integral type that can represent the difference of any two pointers.

  static char * malloc(const size_type bytes)
  {
      return new (std::nothrow) char[bytes];
  }


  static void free(char * const block)
  {
      delete [] block;
  }
};




boost::pool<> pool_fours (4);
boost::pool<> pool_eights (8);
boost::pool<> pool_sixteens (16);
boost::pool<> pool_STL (256);



vector <unsigned short int> bytes_for_pool {4, 8, 16, 256};
vector<unsigned short int>::iterator pool_choice;

int Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
{
	pool_choice = lower_bound(bytes_for_pool.begin(), bytes_for_pool.end(), n_bytes);
	if(pool_choice == bytes_for_pool.end())
	    	return 256; /* it should return -1, and it should mean that there was not large enough pool to allocate memory from. Unfortunately, creating vector seems to always fail this test, and go to this line.
	    				So this is just a workaround, untill a fix is found.*/
	else
	    	return *pool_choice;
}

void * operator new(size_t n_bytes) throw (bad_alloc)
{
	void *storage;
	int pool_size;
	n_bytes = n_bytes + sizeof(unsigned short int); //Place for my own information, which tells how many bytes were allocated by user.

    pool_size = Pick_Pool(n_bytes);
    if(pool_size < 0)
    	throw "Cannot allocate - there is no suitable memory pool for that amount of memory.\n";

    switch(pool_size)
    {
        case 4:
            storage = pool_fours.malloc();
            break;
        case 8:
            storage = pool_eights.malloc();
            break;
        case 16:
            storage = pool_sixteens.malloc();
            break;
        default:
            storage = pool_STL.malloc();
            break;
    }

    if( storage == NULL )
        throw "Memory allocation failed!";

    *(unsigned short int*) storage = (unsigned short int) n_bytes; //Place information about allocated bytes in the beginning of 'storage'.

    void *storage2;
    storage2 = (unsigned short int*) storage + 1; //Compiler will construct an object one place after our information.

    return storage2;
}

void operator delete(void *to_erase) throw()
{
    void *my_info = (unsigned short int*) to_erase - 1; //Go one place back, in order to retrieve information about how many bytes were allocated by user.
    *(unsigned short int*) my_info = Pick_Pool(*(unsigned short int*) my_info);


    if (*(unsigned short int*) my_info < 0)             //If there is nothing to free.
    	return;

    switch ( *(unsigned short int*) my_info )
    {
    	case 4:
			pool_fours.free( my_info );
           	pool_fours.free( to_erase );
            break;
        case 8:
			pool_eights.free( my_info);
            pool_eights.free( to_erase );
            break;
        case 16:
			pool_sixteens.free( my_info );
           	pool_sixteens.free( to_erase );
           	break;
        default:
           	pool_STL.free( my_info );
           	pool_STL.free( to_erase );
            break;
    }
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



void Test_4()
{
    int *i = new int(10);
    double *d = new double(10.5);
    char *c = new char('C');

    cout << *i << " || " << *d << " || " << *c << endl;

    delete i; delete d; delete c;
}

void Perfomance_Test();


int main(int argc, char** argv)
{
	clock_t start = clock();
	//Test_1();
    Test_4();
    //Perfomance_Test();


    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC; cout<< endl << endl << "Time: " << duration << endl;
    return 0;
}





void Perfomance_Test()
{

    for (int i = 0 ; i <10000 ; ++i)
    {
        char *a = new char;
        //delete a;
    }

    for (int i = 0 ; i <500000 ; ++i)
    {
        int *a = new int;
        //delete a;
    }


    for (int i = 0 ; i <500000 ; ++i)
    {
        double *a = new double;
        //delete a;
    }

      struct large_structure {
        int a, b, c, d; double e, f; char g, h;
    };

    for(int i = 0 ; i < 500000; ++i)
    {
        large_structure *x = new large_structure;
        //delete x;
    }

}




/*	//clock_t start; double duration; start = std::clock();
    //duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC; cout<< endl << endl << "Time: " << duration << endl;*/
