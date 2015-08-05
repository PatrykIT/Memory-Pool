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
#include <boost/pool/singleton_pool.hpp>
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


vector <unsigned short int> bytes_for_singletons {4, 8, 16, 256};
vector<unsigned short int>::iterator singleton_choice;

struct four_bytes {};
struct eight_bytes {};
struct sixteen_bytes {};
struct STL_containers {};

typedef boost::singleton_pool <four_bytes, 4, default_user_allocator_new_delete, boost::details::pool::default_mutex, 4, 4>          singleton_fours;
typedef boost::singleton_pool <eight_bytes, 8, default_user_allocator_new_delete, boost::details::pool::default_mutex, 8, 8>         singleton_eights;
typedef boost::singleton_pool <sixteen_bytes, 16, default_user_allocator_new_delete, boost::details::pool::default_mutex, 16, 16>    singleton_sixteens;
typedef boost::singleton_pool <STL_containers, 256, default_user_allocator_new_delete, boost::details::pool::default_mutex, 256, 256>    singleton_STL;

                            //<Tag,  RequestedSize,  UserAllocator,  Mutex,  NextSize,  MaxSize>

/* Tag - tag has no purpose other than creating separate instances o singleton_pool. Thanks to tags, multiple singletons can manage different memory pools, even if they have the same size.
 * Requested size - number of chunks to request from the system the first time that object needs to allocate system memory. This is passed as a constructor parameter to the underlying pool.
 * Next size      - number of chunks to request from the system the next time that object needs to allocate system memory. */









/*void * operator new(size_t n_bytes) throw (bad_alloc)
{
	void *storage;
    n_bytes = n_bytes + sizeof(unsigned short int); //Place for my own information, which tells how many bytes were allocated by user.

    singleton_choice = lower_bound(bytes_for_singletons.begin(), bytes_for_singletons.end(), n_bytes); //crashes on this line.
    if(singleton_choice == bytes_for_singletons.end())
		return NULL;


    switch( *singleton_choice ) //Switch range. It is extension to C++. Switch might be implemented using a lookup table or a hash list. This means that all items get the same access time.
    {
        case 4:
            storage = singleton_fours::malloc();
            break;
        case 8:
            storage = singleton_eights::malloc(); cout << "sing eights" << endl;
            break;
        case 16:
            storage = singleton_sixteens::malloc();
            break;
        default:
            storage = singleton_STL::malloc();
            break;
    }
    if( storage == NULL )
        throw "memory allocation failed!";

    *(unsigned short int*) storage = (unsigned short int) n_bytes; //Place information about allocated bytes in the beginning of 'storage'.

    void *storage2;
    storage2 = (unsigned short int*) storage + 1; //Compiler will construct an object one place after our information.

    return storage2;
}*/

void * operator new(size_t n_bytes) throw (bad_alloc)
{
    void *storage;
    n_bytes = n_bytes + sizeof(unsigned short int); //Place for my own information, which tells how many bytes were allocated by user.

    switch(n_bytes) //Switch range. It is extension to C++. Switch might be implemented using a lookup table or a hash list. This means that all items get the same access time.
    {
        case 0 ... 4:
            storage = singleton_fours::malloc();
            break;
        case 5 ... 8:
            storage = singleton_eights::malloc();
            break;
        case 9 ... 16:
            storage = singleton_sixteens::malloc();
            break;
        default:
            storage = singleton_STL::malloc();
            break;
    }
    if( storage == NULL )
        throw "memory allocation failed!";

    *(unsigned short int*) storage = (unsigned short int) n_bytes; //Place information about allocated bytes in the beginning of 'storage'.

    void *storage2;
    storage2 = (unsigned short int*) storage + 1; //Compiler will construct an object one place after our information.

    return storage2;
}

void operator delete(void *to_erase) throw()
{
    void *my_info = (unsigned short int*) to_erase - 1; //Go one place back, in order to retrieve information about how many bytes were allocated by user.

    if (*(unsigned short int*) my_info > 0)             //If there is something to free.
        switch ( *(unsigned short int*) my_info )
        {
            case 0 ... 4:
                singleton_fours::free( my_info );
                singleton_fours::free( to_erase );
                break;
            case 5 ... 8:
                singleton_eights::free( my_info);
                singleton_fours::free( to_erase ); //Tu jest ŹLE!!!!
                break;
            case 9 ... 16:
                singleton_sixteens::free( my_info );
                singleton_fours::free( to_erase );
                break;
            default:
                singleton_STL::free( my_info );
                singleton_fours::free( to_erase );
                break;
        }
}



void Test_4()
{
    int *i = new int(10);
    double *d = new double(10.5);
    char *c = new char('C');

    cout << *i << " || " << *d << " || " << *c << endl;

    delete i; delete d; delete c;
}

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





void Free_All();


int main(int argc, char** argv)
{
	clock_t start = clock();

    //Test_4();
    //Perfomance_Test();


    Free_All();
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC; cout<< endl << endl << "Time: " << duration << endl;
    return 0;
}

once_flag call_once_flag;
void Free_All()
{
    call_once(call_once_flag, []  //Executes the Callable object  exactly once, even if called from several threads.
    {
        singleton_fours::purge_memory();
        singleton_eights::purge_memory();
        singleton_sixteens::purge_memory();
        singleton_STL::purge_memory();
    }        );
}



/*	//clock_t start; double duration; start = std::clock();
    //duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC; cout<< endl << endl << "Time: " << duration << endl;*/
