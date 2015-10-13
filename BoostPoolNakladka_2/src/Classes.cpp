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
	{};

	void* MyMemoryPool::allocate(uintptr_t* place)
	{
		std::lock_guard<boost::mutex> lock_vector (my_pools_vector_mutex);
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
		this->free(to_erase);
	}


	void MyMemoryPool::my_delete(void *to_erase)
	{
		std::lock_guard<boost::mutex> lock_deleting (my_pools_vector_mutex);
		void *my_info = static_cast<uintptr_t*>(to_erase) - 1; //Go one place back, to get iterator that allocated this object.

	    std::vector <boost::shared_ptr<MyMemoryPool>>::iterator *delete_choice = (std::vector <boost::shared_ptr<MyMemoryPool>>::iterator *) my_info;
	    (**delete_choice)->chuncks_allocated_ = (**delete_choice)->chuncks_allocated_ - (**delete_choice)->get_requested_size();

	    (**delete_choice)->deallocate(my_info, to_erase);
	}

	void MyMemoryPool::sort_pools()
	{
		sort(MyMemoryPool::my_pools_vector.begin(), MyMemoryPool::my_pools_vector.end(), [ ] (const boost::shared_ptr<MyMemoryPool> pool0, const boost::shared_ptr<MyMemoryPool> pool2) { return pool0->get_requested_size() < pool2->get_requested_size(); } ); // Sort pools by size.
	}

	unsigned int MyMemoryPool::get_allocated_chuncks()
	{
		return chuncks_allocated_;
	}

	bool MyMemoryPool::Compare::operator() (const boost::shared_ptr<MyMemoryPool> left, unsigned int value)
	{
			return left->get_requested_size() < value;
	}

	std::vector <boost::shared_ptr<MyMemoryPool>>::iterator MyMemoryPool::Pick_Pool(const size_t n_bytes) //This functions returns the most optimal pool to allocate\deallocate memory from.
	{
		std::lock_guard<boost::mutex> lock_vector (my_pools_vector_mutex);
		pool_choice = lower_bound(my_pools_vector.begin(), my_pools_vector.end(), n_bytes, MyMemoryPool::Compare() );
		return  (std::move (pool_choice));
	}

	std::once_flag run_once_flag;
	void MyMemoryPool::Register_Pools(std::initializer_list<size_t> sizes)
	{
		std::call_once(run_once_flag, [&sizes] ()
		{
			for (auto &sizes_iterator : sizes)
				MyMemoryPool::my_pools_vector.push_back(std::move (boost::shared_ptr<memory_pool::MyMemoryPool>(new MyMemoryPool(sizes_iterator)))); //Is this right usage of a move?
			sort_pools();
		});
	}

	void memory_pool::MyMemoryPool::Register_Pools(std::initializer_list<std::pair<size_t, size_t>> sizes)
	{
		std::call_once(run_once_flag, [&sizes] ()
		{
			for (auto &sizes_iterator : sizes)
				MyMemoryPool::my_pools_vector.push_back(std::move (boost::shared_ptr<memory_pool::MyMemoryPool>(new MyMemoryPool(sizes_iterator.first, sizes_iterator.second))));
			sort_pools();
		});
	}
}





