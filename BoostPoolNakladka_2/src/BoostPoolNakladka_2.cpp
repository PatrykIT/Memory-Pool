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
#include <algorithm>
#include <math.h>

#include <boost/pool/pool.hpp>
#include <boost/pool/detail/mutex.hpp>
#include <boost/pool/detail/guard.hpp>
#include <boost/pool/pool_alloc.hpp>

using namespace std;

void Test_1();
void Test_4();
void Perfomance_Test();



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

/*vector <unsigned short int> bytes_for_pool {4, 8, 16, 256};
vector<unsigned short int>::iterator pool_choice;

int Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
{
	pool_choice = lower_bound(bytes_for_pool.begin(), bytes_for_pool.end(), n_bytes);
	if(pool_choice == bytes_for_pool.end())
	    	return 256; // it should return -1, and it should mean that there was not large enough pool to allocate memory from. Unfortunately, creating vector seems to always fail this test, and go to this line.
	    				//So this is just a workaround, untill a fix is found.
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
            //storage = pool_fours.malloc();
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
}*/



class My_memory_pool : public boost::pool<>
{
private:
	size_t min_size_;
public:

	My_memory_pool(size_t max_size) : boost::pool<>(max_size)
	{

	};

	size_t Get_Min_Size() { return min_size_;}
	size_t Set_Min_Size(int distance)
	{
		this->min_size_ = distance;
	}

};

vector <My_memory_pool*> my_pools_vector;

void Enter_Pools()
{
	size_t sizes;
	cout << "Please enter the sizes of pool you want. 0 for end." << endl;

	while (sizes != 0)
	{
		cin >> sizes;
		if(sizes != 0)
			my_pools_vector.push_back(new My_memory_pool(sizes));
	}

	sort(my_pools_vector.begin(), my_pools_vector.end(), [ ] (const My_memory_pool *pool0, const My_memory_pool *pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } );


	int distance;
	for(vector <My_memory_pool*>::iterator pools_iterator = my_pools_vector.begin(); pools_iterator != my_pools_vector.end(); pools_iterator++ )
	{
			if( (pools_iterator + 1) == my_pools_vector.end())
				break;

			(*(pools_iterator + 1))->Set_Min_Size((*pools_iterator)->get_requested_size() + 1); //We are setting minimum size that pool can take. It is calculated by the distance to a smaller pool.
	}

	for(vector <My_memory_pool*>::iterator pools_iterator = my_pools_vector.begin(); pools_iterator != my_pools_vector.end(); pools_iterator++ )
		cout <<"Minimum size: " <<  (*pools_iterator)->Get_Min_Size() << endl;
}


int main(int argc, char** argv)
{
	clock_t start = clock();
	Enter_Pools();


    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC; cout<< endl << endl << "Time: " << duration << endl;
    return 0;
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
