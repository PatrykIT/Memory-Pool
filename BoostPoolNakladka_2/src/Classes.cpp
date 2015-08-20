#include "../headers/Classes.h"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <initializer_list>

namespace memory_pool
{
	std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>> memory_pool::MyMemoryPool::my_pools_vector;
	std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>>::iterator memory_pool::MyMemoryPool::pool_choice;

	MyMemoryPool::MyMemoryPool(size_t max_size, size_t blocks_number) : boost::pool<>(max_size, blocks_number, 0)
	{

	};

	/*template <class T>
	Memory_Pool::MyMemoryPool::MyMemoryPool(std::initializer_list<T> list)
	{
		for(auto element : list)
			std::cout << "liczba: " << element << "\n";
	};

	template<typename T>
	T add_pool(T v)
	{
		std::cout << "v: " << v << "\n";
		return v;
	}

	template<typename T, typename... Args>
	T add_pool(T first, Args... args)
	{
		return first + add_pool(args...);
	}*/


	void* MyMemoryPool::allocate(size_t n_bytes, uintptr_t* place)
	{
	void *storage;
	storage = this->malloc();

	if( storage == NULL )
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	*reinterpret_cast<uintptr_t*> (storage) = reinterpret_cast<uintptr_t>(place); //Place address about allocated bytes in the beginning of 'storage'.

	return (static_cast<char*>(storage) + sizeof(uintptr_t)); //Compiler will construct an object one place after our information.
	}


	void* MyMemoryPool::my_new(size_t n_bytes)
	{
		n_bytes = n_bytes + sizeof(uintptr_t); //Place for my own information, which is index of a vector that points to the memory pool object was allocated from.
		pool_choice = Pick_Pool(n_bytes);

		uintptr_t* place = reinterpret_cast<uintptr_t*> (&(*pool_choice));

		if(pool_choice == my_pools_vector.end())
		{
			perror("Cannot allocate - there is no suitable memory pool for that amount of memory.");
			return NULL;
		}

		return (*pool_choice)->allocate(n_bytes, place);
	}


	void MyMemoryPool::deallocate(void *my_info ,void *to_erase)
	{
		this->free(my_info);
		this->free(to_erase); //to consult with Tomek. I have a question.
	}


	void MyMemoryPool::my_delete(void *to_erase)
	{
	    void *my_info = static_cast<uintptr_t*>(to_erase) - 1; //Go one place back, to get iterator that allocated this object.

	    std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>>::iterator *delete_choice = (std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>>::iterator *) my_info;
	    std::cout  << "Deleting from pool: " << (**delete_choice)->get_requested_size() << "\n";
	    (**delete_choice)->deallocate(my_info, to_erase);
	}


	bool MyMemoryPool::Compare::operator() (const boost::shared_ptr<memory_pool::MyMemoryPool> left, unsigned int value)
	{
			return left->get_requested_size() < value;
	}


	std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>>::iterator memory_pool::MyMemoryPool::Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
	{
		pool_choice = lower_bound(my_pools_vector.begin(), my_pools_vector.end(), n_bytes, MyMemoryPool::Compare() );
		return pool_choice;
	}

	std::once_flag run_once_flag; //Function will be run only once in the lifetime of a application.
	void Enter_Pools()
	{
		std::call_once(run_once_flag, [] ()
		{
			int sizes = 1; /* '=1' so valgrind doesn't say jump depends on unitialized values. Must be an int, because unsigned int && size_t convert negative number to random positive - there is no way
							to check wether user didn't input negative number.*/
			std::cout << "Please enter the sizes of pool you want. 0 for end." << std::endl;

			while (sizes > 0)
			{
				std::cin >> sizes;
				if(sizes > 0)
					MyMemoryPool::my_pools_vector.push_back(boost::shared_ptr<memory_pool::MyMemoryPool>(new memory_pool::MyMemoryPool(sizes)));
			}

			sort(MyMemoryPool::my_pools_vector.begin(), MyMemoryPool::my_pools_vector.end(), [ ] (const boost::shared_ptr<memory_pool::MyMemoryPool> pool0, const boost::shared_ptr<memory_pool::MyMemoryPool> pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } ); // Sort pools by size.
		});
	}
}





