#ifndef CLASSES_H_
#define CLASSES_H_

#include <boost/pool/pool.hpp>
#include <initializer_list>
#include <vector>
namespace memory_pool
{
	class MyMemoryPool : public boost::pool<>
	{
	private:
		void* allocate(size_t n_bytes, uintptr_t* place);
		void deallocate(void *my_info ,void *to_erase);
		struct Compare
		{
		  bool operator() (const boost::shared_ptr<memory_pool::MyMemoryPool> left, unsigned int value);
		};
		static std::vector <boost::shared_ptr<memory_pool::MyMemoryPool>>::iterator Pick_Pool(const size_t n_bytes);

	public:
		MyMemoryPool(size_t, size_t blocks_number = 512);


		/*template <class T>
		MyMemoryPool(std::initializer_list<T> list);*/

		static void* my_new(size_t n_bytes);
		static void my_delete(void *to_erase);

		static std::vector <boost::shared_ptr<MyMemoryPool>> my_pools_vector;
		static std::vector <boost::shared_ptr<MyMemoryPool>>::iterator pool_choice;
	};
	void Enter_Pools();
}


#endif /* CLASSES_H_ */
