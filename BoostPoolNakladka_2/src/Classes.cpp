#include "../headers/Classes.h"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <vector>


extern std::vector <boost::shared_ptr<Memory_Pool::MyMemoryPool>> my_pools_vector;
extern std::vector <boost::shared_ptr<Memory_Pool::MyMemoryPool>>::iterator pool_choice;

  void* Memory_Pool::MyMemoryPool::allocate(size_t n_bytes)
  {
    std::cout << "Putting object into pool size: " << get_requested_size() << std::endl;
    void *storage;
    storage = this->malloc();

    if( storage == NULL )
    {
      perror("Memory allocation failed!");
      return NULL;
    }

    *(unsigned short int*) storage = (unsigned short int) n_bytes; //Place information about allocated bytes in the beginning of 'storage'.

    return storage + sizeof(unsigned short int); //Compiler will construct an object one place after our information.
  }

	void Memory_Pool::MyMemoryPool::deallocate(void *my_info ,void *to_erase)
	{
		this->free(my_info);
		//this->free(to_erase); //to consult with Tomek. I have a question.
	}


	Memory_Pool::MyMemoryPool::MyMemoryPool(size_t max_size) : boost::pool<>(max_size)
	{

	};


	void* Memory_Pool::MyMemoryPool::my_new(size_t n_bytes)
	{
		n_bytes = n_bytes + sizeof(unsigned short int); //Place for my own information, which tells how many bytes were allocated by user.
		pool_choice = Pick_Pool(n_bytes);

		if(pool_choice == my_pools_vector.end())
		{
			perror("Cannot allocate - there is no suitable memory pool for that amount of memory.");
			return NULL;
	  }

		return (*pool_choice)->allocate(n_bytes);
	}


	void Memory_Pool::MyMemoryPool::my_delete(void *to_erase)
	{
	    void *my_info = (unsigned short int*) to_erase - 1; //Go one place back, in order to retrieve information about how many bytes were allocated by user.
	    pool_choice = Pick_Pool(*(unsigned short int*) my_info);
	    printf("Freeing from the pool size: %zu\n", (*pool_choice)->get_requested_size());
	    (*pool_choice)->deallocate(my_info, to_erase);

	}


	bool Memory_Pool::MyMemoryPool::compare::operator() (const boost::shared_ptr<Memory_Pool::MyMemoryPool> left, unsigned int value)
	{
			return left->get_requested_size() < value;
	}


	std::vector <boost::shared_ptr<Memory_Pool::MyMemoryPool>>::iterator Memory_Pool::MyMemoryPool::Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
	{
		pool_choice = lower_bound(my_pools_vector.begin(), my_pools_vector.end(), n_bytes, MyMemoryPool::compare() );
		if(pool_choice == my_pools_vector.end())
		    	return my_pools_vector.end();
		else
		   	return pool_choice;
	}


	void Enter_Pools()
	{
		int sizes = 1; /* '=1' so valgrind doesn't say jump depends on unitialized values. Must be an int, because unsigned int && size_t convert negative number to random positive - there is no way
						to check wether user didn't input negative number.*/
		std::cout << "Please enter the sizes of pool you want. 0 for end." << std::endl;

		while (sizes > 0)
		{
			std::cin >> sizes;
			if(sizes > 0)
				my_pools_vector.push_back(boost::shared_ptr<Memory_Pool::MyMemoryPool>(new Memory_Pool::MyMemoryPool(sizes)));
		}

		sort(my_pools_vector.begin(), my_pools_vector.end(), [ ] (const boost::shared_ptr<Memory_Pool::MyMemoryPool> pool0, const boost::shared_ptr<Memory_Pool::MyMemoryPool> pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } ); // Sort pools by size.
	}




