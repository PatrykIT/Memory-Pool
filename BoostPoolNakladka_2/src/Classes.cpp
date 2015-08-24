#include "../headers/Classes.h"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <initializer_list>
#include <utility>

namespace memory_pool
{
	// Initation of static members.
	std::vector <boost::shared_ptr<MyMemoryPool>> MyMemoryPool::my_pools_vector;
	std::vector <boost::shared_ptr<MyMemoryPool>>::iterator MyMemoryPool::pool_choice;
	boost::mutex MyMemoryPool::my_pools_vector_mutex;



	MyMemoryPool::MyMemoryPool(size_t max_size, size_t blocks_number) : boost::pool<>(max_size, blocks_number, 0)
	{
		//boost::pool<>(max_size, blocks_number, 0);
	};

	/*MyMemoryPool::MyMemoryPool(std::initializer_list<int> list) : boost::pool<>(32)
	{
		for(auto element : list)
			std::cout << "liczba: " << element << "\n";
	};*/

	/*template<typename T>
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

	void* MyMemoryPool::allocate(uintptr_t* place)
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

	void MyMemoryPool::deallocate(void *my_info ,void *to_erase)
	{
		this->free(my_info);
		this->free(to_erase); //to consult with Tomek. I have a question.
	}


	void MyMemoryPool::my_delete(void *to_erase)
	{
		std::lock_guard<boost::mutex> lock_deleting (my_pools_vector_mutex);
		void *my_info = static_cast<uintptr_t*>(to_erase) - 1; //Go one place back, to get iterator that allocated this object.

	    std::vector <boost::shared_ptr<MyMemoryPool>>::iterator *delete_choice = (std::vector <boost::shared_ptr<MyMemoryPool>>::iterator *) my_info;
	    //std::cout  << "Deleting from pool: " << (**delete_choice)->get_requested_size() << "\n";
	    (**delete_choice)->chuncks_allocated = (**delete_choice)->chuncks_allocated - (**delete_choice)->get_requested_size();

	    (**delete_choice)->deallocate(my_info, to_erase);
	}

	unsigned int MyMemoryPool::get_allocated_chuncks()
	{
		return chuncks_allocated;
	}
	unsigned int MyMemoryPool::get_allocated_bytes()
	{
		return bytes_allocated;
	}


	bool MyMemoryPool::Compare::operator() (const boost::shared_ptr<MyMemoryPool> left, unsigned int value)
	{
			return left->get_requested_size() < value;
	}


	std::vector <boost::shared_ptr<MyMemoryPool>>::iterator MyMemoryPool::Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
	{
		pool_choice = lower_bound(my_pools_vector.begin(), my_pools_vector.end(), n_bytes, MyMemoryPool::Compare() );
		return  (std::move (pool_choice));
	}

	std::once_flag run_once_flag; //Function will be run only once in the lifetime of a application.
	void Enter_Pools()
	{
		std::lock_guard<boost::mutex> lock_entering (MyMemoryPool::my_pools_vector_mutex);

		std::call_once(run_once_flag, [] ()
		{
			int sizes = 1; /* '=1' so valgrind doesn't say jump depends on unitialized values. Must be an int, because unsigned int && size_t convert negative number to random positive - there is no way
							to check wether user didn't input negative number.*/
			std::cout << "Please enter the sizes of pool you want. 0 for end." << std::endl;

			while (sizes > 0)
			{
				std::cin >> sizes;
				if(sizes > 0)
					MyMemoryPool::my_pools_vector.push_back(std::move (boost::shared_ptr<memory_pool::MyMemoryPool>(new MyMemoryPool(sizes)))); //Is this right usage of a move?
			}

			sort(MyMemoryPool::my_pools_vector.begin(), MyMemoryPool::my_pools_vector.end(), [ ] (const boost::shared_ptr<MyMemoryPool> pool0, const boost::shared_ptr<MyMemoryPool> pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } ); // Sort pools by size.
		});
	}
}





