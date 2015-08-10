/*
 * Classes.h
 *
 *  Created on: Aug 10, 2015
 *      Author: patrykit
 */

#ifndef CLASSES_H_
#define CLASSES_H_

#include <boost/pool/pool.hpp>
class My_memory_pool : public boost::pool<>
{
private:
	void* allocate(size_t n_bytes);
	void deallocate(void *my_info ,void *to_erase);

public:
	My_memory_pool(size_t max_size);
	void Set_Min_Size(int distance);
	static void* my_new(size_t n_bytes);
	static void my_delete(void *to_erase);
};

void Enter_Pools();

struct compare
{
	bool operator() (const boost::shared_ptr<My_memory_pool> left, unsigned int value);
};

std::vector <boost::shared_ptr<My_memory_pool>>::iterator Pick_Pool(const size_t n_bytes);

#endif /* CLASSES_H_ */
