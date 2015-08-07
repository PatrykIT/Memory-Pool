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
#include <memory>

#include <boost/pool/pool.hpp>
#include <boost/pool/detail/mutex.hpp>
#include <boost/pool/detail/guard.hpp>
#include <boost/pool/pool_alloc.hpp>

using namespace std;

void Test_1();
void Test_4();
void Perfomance_Test();
class My_memory_pool;
vector <My_memory_pool*>::iterator Pick_Pool(const size_t n_bytes);




vector <My_memory_pool*> my_pools_vector;
vector <My_memory_pool*>::iterator pool_choice;

class My_memory_pool : public boost::pool<>
{
private:
	size_t min_size_ = 1;
public:

	My_memory_pool(size_t max_size) : boost::pool<>(max_size)
	{

	};

	size_t Get_Min_Size() { return min_size_;}

	void Set_Min_Size(int distance)
	{
		this->min_size_ = distance;
	}

	void* allocate(size_t n_bytes) throw (bad_alloc)
	{
		cout << "Putting object into pool size: " << get_requested_size() << endl;
		void *storage;
	    storage = this->malloc();

	    if( storage == NULL )
	        throw "Memory allocation failed!";

	    *(unsigned short int*) storage = (unsigned short int) n_bytes; //Place information about allocated bytes in the beginning of 'storage'.

	    void *storage2;
	    storage2 = (unsigned short int*) storage + 1; //Compiler will construct an object one place after our information.

	    return storage2;
	}

	void deallocate(void *my_info ,void *to_erase) throw()
	{
		this->free(my_info);
		this->free(to_erase);
	}

	static void* my_new(size_t n_bytes) throw (bad_alloc)
	{
		n_bytes = n_bytes + sizeof(unsigned short int); //Place for my own information, which tells how many bytes were allocated by user.
		pool_choice = Pick_Pool(n_bytes);

		if(pool_choice == my_pools_vector.end())
		{
			cout << "Cannot allocate - there is no suitable memory pool for that amount of memory." << endl;
			return NULL;
	  	}

		void* storage = (*pool_choice)->allocate(n_bytes);
		return storage;
	}

	static void my_delete(void *to_erase) throw()
	{
	    void *my_info = (unsigned short int*) to_erase - 1; //Go one place back, in order to retrieve information about how many bytes were allocated by user.
	    pool_choice = Pick_Pool(*(unsigned short int*) my_info);
	    cout << "Freeing from the pool size: : " << (*pool_choice)->get_requested_size() << endl;
	    (*pool_choice)->deallocate(my_info, to_erase);

	}
};



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

	sort(my_pools_vector.begin(), my_pools_vector.end(), [ ] (const My_memory_pool *pool0, const My_memory_pool *pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } ); // Sort pools by size.

//-------------------------------------------------------Set minimum size--------------------------------------------------------

	for(vector <My_memory_pool*>::iterator pools_iterator = my_pools_vector.begin(); pools_iterator != my_pools_vector.end() - 1; pools_iterator++ ) // end()-1 so we do not jump out of bound.
		(*(pools_iterator + 1))->Set_Min_Size((*pools_iterator)->get_requested_size() + 1); //We are setting minimum size that pool can take. It is calculated by the distance to a smaller pool.
}




struct compare
{
	bool operator() (const My_memory_pool *left, unsigned int value)
	{
		return left->get_requested_size() < value;
	}
};
vector <My_memory_pool*>::iterator Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
{
	pool_choice = lower_bound(my_pools_vector.begin(), my_pools_vector.end(), n_bytes, compare() );
	if(pool_choice == my_pools_vector.end())
	    	return my_pools_vector.end();
	else
	   	return pool_choice;
}




void Test_0()
{
	int *x = (int*) My_memory_pool::my_new(sizeof(int));
	*x = 99;
	cout <<"*x: " << *x << endl;

	My_memory_pool::my_delete(x);
	cout << " *x after delete: " << *x << endl;
}

once_flag call_once_flag;
void Free_All()
{
    call_once(call_once_flag, []  //Executes the Callable object  exactly once, even if called from several threads.
    {
    	for(vector <My_memory_pool*>::iterator pools_iterator = my_pools_vector.begin(); pools_iterator != my_pools_vector.end() ; pools_iterator++ ) // end()-1 so we do not jump out of bound.
    	{
    		(*pools_iterator)->purge_memory();
    	}
    	my_pools_vector.clear();
    });
}

int main(int argc, char** argv)
{
	clock_t start = clock();
	Enter_Pools();
	Test_0();




	Free_All();
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
