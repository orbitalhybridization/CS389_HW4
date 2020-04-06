// A testfile

#include "cache.hh"
#include <cassert>
#include "fifo_evictor.hh"
#include <iostream>
#include <cstring>


// no interdependencies
//have 5-12 tests for cache & evictor each


//recquire instead of assert


using byte_type = char;
using val_type = const byte_type*;   // Values for K-V pairs
using size_type = uint32_t;         // Internal indexing to K-V elements


bool test_set(Cache* test_cache, val_type test_val) {
	//cache
	test_cache->set("Chilombo",test_val,2);
	size_type sz = 0;
	//auto out_val = test_cache->get("Chilombo",sz);
	//assert(*out_val == *test_val and "test_cache");
	return true;
}

bool test_get(Cache* test_cache, val_type test_value){
	//cache
	test_cache->set("Aiko",test_value,2);
	size_type sz = 0;
	auto out_val = test_cache->get("Aiko",sz);
	assert(*out_val == *test_value and "test_get");
	return true;
}

//Run all test functions
int main() {
	
	//init evictor-less cache to test
	Cache* test_cache = new Cache("127.0.0.1","6969");
	//auto evictor = new FIFO();
	//init cache w/evictor
	//Cache* test_cache_with_evictor = new Cache(10,0.75,evictor);
	val_type value1 = "R";
	val_type value2 = "U";
	val_type value3 = "M";

	//First test
	if (test_set(test_cache,value1)) {
		
		std::cout << "Set function test passed!" << std::endl;
	}
	//test_cache->reset(); 

	//Second test
	if (test_get(test_cache,value1)) {
		std::cout << "Get function test passed!" << std::endl;
	}
	//test_cache->reset();
	
	std::cout << "All tests passed! Fuck yeah!!" << std::endl;
	delete test_cache;
	delete value1;
	delete value2;
	delete value3;

	return 1;
}
