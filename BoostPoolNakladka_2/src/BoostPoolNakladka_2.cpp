#include <boost/smart_ptr/shared_ptr.hpp>
#include <stddef.h>
#include <sys/time.h>
#include <ctime>
#include <iostream>

#include "../headers/Classes.h"
#include "../headers/Tests.h"
#include <initializer_list>
using namespace std;



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
        /*This is a very elegant solution to a requirement any allocator has to fulfill: to be able to allocate objects of different types than its template parameter. */
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
      //return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T)));
    	return memory_pool::MyMemoryPool::my_new<T>();
    }
    inline void deallocate(T* p, size_t)
    {
        //::operator delete(p);
    	memory_pool::MyMemoryPool::my_delete(p);
    }

    inline size_t max_size() const
    {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    inline void construct(T* p, /*const*/ T& t)
    {
    	//new(p) T(t);
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


/*
template <typename T>
class Allocator
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
    typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	template <typename U>
	struct rebind
	{
		typedef Allocator<U> other;
	};

public:
	explicit Allocator() {}
	~Allocator() {}
	explicit Allocator(Allocator const&){}

	template <typename U>
	explicit Allocator(Allocator<U> const&){}

	inline pointer address (reference r)
	{
		return &r;
	}

	const pointer address(const reference r)
	{
		return &r;
	}

	T* allocate (size_t cnt, typename std::allocator<void>::const_pointer = 0)
	{
		return reinterpret_cast<T*>(::operator new(cnt * sizeof(T)));
	}

	void deallocate(T* to_erase, size_t)
	{
		::operator delete(to_erase);
	}

	size_type max_size() const
	{
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}


	void construct (pointer place, const T& object)
	{
		new (place) T(object);
	}

	void destroy(pointer place)
	{
		place->~T();
	}

	bool operator ==(Allocator const&)
		{
			return true;
		}

	bool operator !=(Allocator const& a)
		{
			return !operator==(a);
		}
};*/

template <class T>
void func2( std::initializer_list<T> list )
{
    for( auto elem : list )
    {
        std::cout << elem << "\n" ;
    }
}



int main(int argc, char** argv)
{
	memory_pool::Enter_Pools();
	//Memory_Pool::MyMemoryPool obj1;
	Test_0();
	//memory_pool::MyMemoryPool obj1(4, 64);

	//func2({16, 24});

	Allocator<int> my_alloc;

	vector<int, Allocator<int>> my_vec(my_alloc);
	cout <<"capacity: " << my_vec.capacity() << endl;
	my_vec.reserve(20);
	cout <<"capacity: " << my_vec.capacity() << endl;

	my_vec.push_back(15);
	my_vec.push_back(9);
	for(auto iteratorek = my_vec.begin(); iteratorek != my_vec.end(); ++iteratorek)
		std::cout << "vec: " << *iteratorek << endl;




	clock_t start = clock();
	//Performance_Test_Default_New_2();
	//Performance_Test_Pool_2();

    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC; std::cout << fixed << std::endl << std::endl << "Time: " << duration << std::endl;
    return 0;
}

