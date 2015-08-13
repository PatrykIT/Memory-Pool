/*
 * Classes.h
 *
 *  Created on: Aug 10, 2015
 *      Author: patrykit
 */

#ifndef CLASSES_H_
#define CLASSES_H_

#include <boost/pool/pool.hpp>

namespace Memory_Pool
{
	class MyMemoryPool : public boost::pool<>
	{
	private:
		void* allocate(size_t n_bytes, uintptr_t* place);
		void deallocate(void *my_info ,void *to_erase);
		struct compare
		{
		  bool operator() (const boost::shared_ptr<Memory_Pool::MyMemoryPool> left, unsigned int value);
		};

	public:
		MyMemoryPool(size_t max_size);
		static void* my_new(size_t n_bytes);
		static void my_delete(void *to_erase);
		static std::vector <boost::shared_ptr<Memory_Pool::MyMemoryPool>>::iterator Pick_Pool(const size_t n_bytes);
	};
}
void Enter_Pools();
void Pools_Addresses();



#endif /* CLASSES_H_ */
