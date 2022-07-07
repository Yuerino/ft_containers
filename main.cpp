#include "vector.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

#include <iostream>
#include <type_traits> // C++11
#include <vector>

struct A {};

int main() {
	#ifdef ASSERT_TEST
	{ // _has_iterator_category test
		static_assert(ft::_has_iterator_category<ft::iterator_traits<int*> >::value, "Iterator should have category");
		static_assert(!ft::_has_iterator_category<int>::value, "Non iterator should have no category");
		static_assert(!ft::_has_iterator_category<A>::value, "Non iterator should have no category");
	}
	{ // is_integral test
		static_assert(ft::is_integral<int>::value, "Int should be integral data type");
		static_assert(!ft::is_integral<ft::vector<int> >::value, "ft::vector should not be integral data type");
	}
	#endif
	// { // ft::distance test
	// 	std::vector<int> v(5, 42);
	// 	std::cout << "ft::distance test: " << ft::distance(v.begin(), v.end()) << std::endl;
	// }
	{ // iterator test
		ft::vector<int> v(5, 42);
		ft::vector<int>::iterator it;
		for (it = v.begin(); it != v.end(); ++it)
			(*it)++;
		for (it = v.begin(); it != v.end(); ++it)
			std::cout << *it << std::endl;
		std::cout << v.size() << std::endl << std::endl;

		ft::vector<int> v1(v);
		for (it = v1.begin(); it != v1.end(); ++it)
			(*it)++;
		for (it = v1.begin(); it != v1.end(); ++it)
			std::cout << *it << std::endl;
		std::cout << v1.size() << std::endl << std::endl;

		ft::vector<int> v2(v1.begin(), v1.end());
		for (it = v2.begin(); it != v2.end(); ++it)
			(*it)++;
		for (it = v2.begin(); it != v2.end(); ++it)
			std::cout << *it << std::endl;
		std::cout << v2.size() << std::endl << std::endl;

		ft::vector<int>::const_iterator cit;
		// for (cit = v2.begin(); cit != v2.end(); ++cit)
		// 	(*cit)++;
		for (cit = v2.begin(); cit != v2.end(); ++cit)
			std::cout << *cit << std::endl;
		std::cout << v2.size() << std::endl << std::endl;
	}
	return 0;
}
