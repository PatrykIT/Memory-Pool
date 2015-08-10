#include <boost/smart_ptr/shared_ptr.hpp>
#include <stddef.h>
#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <vector>

#include "../headers/Classes.h"
#include "../headers/Tests.h"


class My_memory_pool;
std::vector <boost::shared_ptr<My_memory_pool>>::iterator Pick_Pool(const size_t n_bytes);
std::vector <boost::shared_ptr<My_memory_pool>> my_pools_vector;
std::vector <boost::shared_ptr<My_memory_pool>>::iterator pool_choice;








int main(int argc, char** argv)
{
	clock_t start = clock();
	Enter_Pools();
	Test_0();

	Perfomance_Test();


    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC; std::cout<< std::endl << std::endl << "Time: " << duration << std::endl;
    return 0;
}



/*	//clock_t start; double duration; start = std::clock();
    //duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC; cout<< endl << endl << "Time: " << duration << endl;*/
