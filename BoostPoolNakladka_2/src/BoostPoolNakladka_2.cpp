#include <boost/smart_ptr/shared_ptr.hpp>
#include <stddef.h>
#include <sys/time.h>
#include <ctime>
#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"
#include <initializer_list>
using namespace std;


//Custom allocator, that uses memory pool. It crashes when STL containters resize themselves.
template<typename T>
class Allocator {
public :
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public :
    //    convert an allocator<T> to allocator<U>
    template<typename U>
    struct rebind
	{
        typedef Allocator<U> other;
        //This is a very elegant solution to a requirement any allocator has to fulfill: to be able to allocate objects of different types than its template parameter.
    };

public :
    inline explicit Allocator() {}
    inline ~Allocator() {}
    inline explicit Allocator(Allocator const&) {}
    template<typename U>
    inline explicit Allocator(Allocator<U> const&) {}

    inline T* address(T& r)
    {
    	return &r;
    }

    inline const T* address(const T& r)
    {
    	return &r;
    }

    inline T* allocate(size_t cnt, typename std::allocator<void>::const_pointer = 0)
    {
    	return memory_pool::MyMemoryPool::my_new<T>();
    }
    inline void deallocate(T* p, size_t)
    {
    	memory_pool::MyMemoryPool::my_delete(p);
    }

    inline size_t max_size() const
    {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    inline void construct(T &t)
    {
    	t.T::T();
    }

    inline void destroy(T* p)
    {
    	p->~T();
    }

    inline bool operator==(Allocator const&)
	{
		return true; //hardcoded to return true
	}
    inline bool operator!=(Allocator const& a)
	{
		return !operator==(a); //hardcoded to return false
	}
};


int main(int argc, char** argv)
{
	//memory_pool::MyMemoryPool::Register_Pools({10, 23});
	memory_pool::MyMemoryPool::Register_Pools({ {65, 31}, {60, 83} });
	Test_0();

	/*//Example of Allocator usage.
	Allocator<int> my_alloc;
	vector<int, Allocator<int>> my_vec(my_alloc);
	my_vec.reserve(2); //We do not want vector to resize, because it would cause an error.
	my_vec.push_back(15); my_vec.push_back(9);
	for(auto iterator = my_vec.begin(); iterator != my_vec.end(); ++iterator)
		std::cout << "vector_element: " << *iterator << endl; */



	clock_t start = clock();
	//Performance_Test_Default_New();
	//Performance_Test_Pool();

    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC; std::cout << fixed << std::endl << std::endl << "Time: " << duration << std::endl;
    return 0;
}

