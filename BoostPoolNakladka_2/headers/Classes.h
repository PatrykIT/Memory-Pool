#ifndef CLASSES_H_
#define CLASSES_H_

#include <boost/pool/pool.hpp>
#include <initializer_list>
#include <vector>
#include <thread>
#include <mutex>
namespace memory_pool
{
	class MyMemoryPool : public boost::pool<>
	{
	private:
		unsigned int chuncks_allocated = 0;
		unsigned int bytes_allocated = 0;

	private:
		void* allocate(uintptr_t* place);
		void deallocate(void *my_info ,void *to_erase);

		struct Compare
		{
		  bool operator() (const boost::shared_ptr<memory_pool::MyMemoryPool> left, unsigned int value);
		};

		static std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>>::iterator Pick_Pool(const size_t n_bytes);

	public:
		MyMemoryPool(size_t, size_t blocks_number = 512);
		//MyMemoryPool(std::initializer_list<int> list);

		template<class T>
		static T* my_new() //We could use 'export' keyword and put declaration in the .cpp file, but it is a bad practice. See here: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1426.pdf
		{
			//std::lock_guard<boost::mutex> lock_vector (my_pools_vector_mutex); //lock the mutex at first, if already locked, this will suspend execution until it's available.

			pool_choice = Pick_Pool(sizeof(T) + sizeof(uintptr_t)); //Place for my own information, which is index of a vector that points to the memory pool object was allocated from.

			(*pool_choice)->bytes_allocated = (*pool_choice)->bytes_allocated + sizeof(T) + sizeof(uintptr_t); //We would have to place this information before object - would it profitable?
			(*pool_choice)->chuncks_allocated = (*pool_choice)->chuncks_allocated  + (*pool_choice)->get_requested_size();

			uintptr_t* place = reinterpret_cast<uintptr_t*> (&(*pool_choice));

			if(pool_choice == my_pools_vector.end())
			{
				perror("Cannot allocate - there is no suitable memory pool for that amount of memory.");
				return NULL;
			}

			return reinterpret_cast<T*> ((*pool_choice)->allocate(place));
		}//To further read: http://stackoverflow.com/questions/2185954/implications-of-template-declaration-definition


		static void my_delete(void *to_erase);
		unsigned int get_allocated_chuncks();
		unsigned int get_allocated_bytes();

	public:
		static std::vector <boost::shared_ptr<MyMemoryPool>> my_pools_vector;
		static std::vector <boost::shared_ptr<MyMemoryPool>>::iterator pool_choice;
		static boost::mutex my_pools_vector_mutex;
	};
	void Enter_Pools();
}


#endif /* CLASSES_H_ */
