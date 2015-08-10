#include "../headers/Classes.h"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <vector>

using namespace std;


extern vector <boost::shared_ptr<My_memory_pool>>::iterator Pick_Pool(const size_t n_bytes);
extern vector <boost::shared_ptr<My_memory_pool>> my_pools_vector;
extern vector <boost::shared_ptr<My_memory_pool>>::iterator pool_choice;


	void* My_memory_pool::allocate(size_t n_bytes)
	{
		cout << "Putting object into pool size: " << get_requested_size() << endl;
		void *storage;
	    storage = this->malloc();

	    if( storage == NULL )
	    {
	    	printf("Memory allocation failed!");
	    	return NULL;
	    }

	    *(unsigned short int*) storage = (unsigned short int) n_bytes; //Place information about allocated bytes in the beginning of 'storage'.

	    return storage + sizeof(unsigned short int); //Compiler will construct an object one place after our information.
	}

	void My_memory_pool::deallocate(void *my_info ,void *to_erase)
	{
		this->free(my_info);
		//this->free(to_erase); //to consult with Tomek. I have a question.
	}




	My_memory_pool::My_memory_pool(size_t max_size) : boost::pool<>(max_size)
	{

	};

	void* My_memory_pool::my_new(size_t n_bytes)
	{
		n_bytes = n_bytes + sizeof(unsigned short int); //Place for my own information, which tells how many bytes were allocated by user.
		pool_choice = Pick_Pool(n_bytes);

		if(pool_choice == my_pools_vector.end())
		{
			cout << "Cannot allocate - there is no suitable memory pool for that amount of memory." << endl;
			return NULL;
	  	}
		return (*pool_choice)->allocate(n_bytes);
	}

	void My_memory_pool::my_delete(void *to_erase)
	{
	    void *my_info = (unsigned short int*) to_erase - 1; //Go one place back, in order to retrieve information about how many bytes were allocated by user.
	    pool_choice = Pick_Pool(*(unsigned short int*) my_info);
	    cout << "Freeing from the pool size: " << (*pool_choice)->get_requested_size(); cout << endl;
	    (*pool_choice)->deallocate(my_info, to_erase);

	}




	void Enter_Pools()
	{
		int sizes = 1; /* '=1' so valgrind doesn't say jump depends on unitialized values. Must be an int, because unsigned int && size_t convert negative number to random positive - there is no way
						to check wether user didn't input negative number.*/
		cout << "Please enter the sizes of pool you want. 0 for end." << endl;

		while (sizes > 0)
		{
			cin >> sizes;
			if(sizes > 0)
				my_pools_vector.push_back(boost::shared_ptr<My_memory_pool>(new My_memory_pool(sizes)));
		}

		sort(my_pools_vector.begin(), my_pools_vector.end(), [ ] (const boost::shared_ptr<My_memory_pool> pool0, const boost::shared_ptr<My_memory_pool> pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } ); // Sort pools by size.
	}



		bool compare::operator() (const boost::shared_ptr<My_memory_pool> left, unsigned int value)
		{
			return left->get_requested_size() < value;
		}

	vector <boost::shared_ptr<My_memory_pool>>::iterator Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
	{
		pool_choice = lower_bound(my_pools_vector.begin(), my_pools_vector.end(), n_bytes, compare() );
		if(pool_choice == my_pools_vector.end())
		    	return my_pools_vector.end();
		else
		   	return pool_choice;
	}

