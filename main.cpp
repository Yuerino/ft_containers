#include "vector.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

#include <iostream>
#include <type_traits> // C++11
#include <vector>

struct A {};

class WrapInt {
public:
	WrapInt(int i) : _nbr(i) {}
	~WrapInt() { std::cout << this->_nbr << std::endl; }
protected:
	int _nbr;
};

int main() {
	// { // ft::distance test
	// 	std::vector<int> v(5, 42);
	// 	std::cout << "ft::distance test: " << ft::distance(v.begin(), v.end()) << std::endl;
	// }
	// { // iterator test
	// 	ft::vector<int> v(5, 42);
	// 	ft::vector<int>::iterator it;
	// 	for (it = v.begin(); it != v.end(); ++it)
	// 		(*it)++;
	// 	for (it = v.begin(); it != v.end(); ++it)
	// 		std::cout << *it << std::endl;
	// 	std::cout << v.size() << std::endl << std::endl;

	// 	ft::vector<int> v1(v);
	// 	for (it = v1.begin(); it != v1.end(); ++it)
	// 		(*it)++;
	// 	for (it = v1.begin(); it != v1.end(); ++it)
	// 		std::cout << *it << std::endl;
	// 	std::cout << v1.size() << std::endl << std::endl;

	// 	ft::vector<int> v2(v1.begin(), v1.end());
	// 	for (it = v2.begin(); it != v2.end(); ++it)
	// 		(*it)++;
	// 	for (it = v2.begin(); it != v2.end(); ++it)
	// 		std::cout << *it << std::endl;
	// 	std::cout << v2.size() << std::endl << std::endl;

	// 	ft::vector<int>::const_iterator cit;
	// 	// for (cit = v2.begin(); cit != v2.end(); ++cit)
	// 	// 	(*cit)++;
	// 	for (cit = v2.begin(); cit != v2.end(); ++cit)
	// 		std::cout << *cit << std::endl;
	// 	std::cout << v2.size() << std::endl << std::endl;
	// }
	// {
	// 	ft::vector<int> myvector;

	// 	for (int i=1;i<10;i++) myvector.push_back(i);

	// 	myvector.resize(5);
	// 	myvector.resize(8,100);
	// 	myvector.resize(12);

	// 	std::cout << "myvector contains:";
	// 	for (int i=0;i<(int)myvector.size();i++)
	// 		std::cout << ' ' << myvector[i];
	// 	std::cout << '\n';
	// }
	// {
	// 	ft::vector<int> myvector(3, 100);
	// 	ft::vector<int>::iterator it;

	// 	it = myvector.begin();
	// 	it = myvector.insert(it, 200);
	// 	std::cout << "myvector contains:";
	// 	for (it=myvector.begin(); it!=myvector.end(); ++it)
	// 	std::cout << ' ' << *it;
	// 	std::cout << '\n';
	// 	it = myvector.begin();

	// 	myvector.insert(it, 2, 300);

	// 	std::cout << "myvector contains:";
	// 	for (it=myvector.begin(); it!=myvector.end(); ++it)
	// 	std::cout << ' ' << *it;
	// 	std::cout << '\n';

	// 	it = myvector.begin();
	// 	it += 2;
	// 	ft::vector<int> anothervector(2,400);
	// 	myvector.insert(it, anothervector.begin(), anothervector.end());
	// 	std::cout << "myvector contains:";
	// 	for (it=myvector.begin(); it!=myvector.end(); ++it)
	// 	std::cout << ' ' << *it;
	// 	std::cout << '\n';

	// 	int myarray [] = { 501,502,503 };
	// 	myvector.insert (myvector.begin(), myarray, myarray+3);
	// 	std::cout << "myvector contains:";
	// 	for (it=myvector.begin(); it!=myvector.end(); ++it)
	// 	std::cout << ' ' << *it;
	// 	std::cout << '\n';
	// }
	// {
	// 	ft::vector<int> myvector;
	// 	ft::vector<int>::iterator it;
	// 	// set some values (from 1 to 10)
	// 	for (int i=1; i<=10; i++) myvector.push_back(i);

	// 	// erase the 6th element
	// 	it = myvector.begin();
	// 	it += 5;
	// 	it = myvector.erase(it);
	// 	std::cout << *it << std::endl;

	// 	// erase the first 3 elements:
	// 	it = myvector.begin();
	// 	it += 3;
	// 	myvector.erase(myvector.begin(), it);

		// std::cout << "myvector contains:";
		// for (unsigned i=0; i<myvector.size(); ++i)
		// 	std::cout << ' ' << myvector[i];
		// std::cout << '\n';
	// }
	// {
	// 	ft::vector<WrapInt> myvector;
	// 	for (int i=1; i<=10; ++i) myvector.push_back(WrapInt(i));

	// 	myvector.pop_back();
	// 	// std::cout << "myvector contains:";
	// 	// for (unsigned i=0; i<myvector.size(); ++i)
	// 	// 	std::cout << ' ' << myvector[i];
	// 	// std::cout << '\n';
	// }
	// {
	// 	ft::vector<int> myvector;
	// 	myvector.push_back(1); myvector.push_back(2);
	// 	ft::vector<int>::iterator it1 = myvector.begin();
	// 	ft::vector<int>::iterator it2 = myvector.begin();
	// 	ft::vector<int>::iterator it3 = myvector.end();
	// 	ft::vector<int>::const_iterator it4 = myvector.begin();

	// 	std::cout << "it1 == it2: " << (it1 == it2) << std::endl;
	// 	std::cout << "it1 == it3: " << (it1 == it3) << std::endl;
	// 	std::cout << "it1 == it4: " << (it1 == it4) << std::endl;
	// }
	{
		ft::vector<int> myvector;
		for(int i=1;i<=5;++i) myvector.push_back(i);
		ft::vector<int>::const_iterator it = myvector.begin();
		ft::vector<int>::const_reverse_iterator rit = myvector.rbegin();
		std::cout << *rit << std::endl;
		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << '\n';
		// ++(*rit);
		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << '\n';
		std::cout << *rit << std::endl;
		std::cout << rit[2] << std::endl;
	}
	return 0;
}

#ifdef ASSERT_TEST
	// _has_iterator_category assert
	static_assert(ft::_has_iterator_category<ft::iterator_traits<int*> >::value, "Iterator should have category");
	static_assert(!ft::_has_iterator_category<int>::value, "Non iterator should have no category");
	static_assert(!ft::_has_iterator_category<A>::value, "Non iterator should have no category");

	// is_integral assert
	static_assert(ft::is_integral<int>::value, "Int should be integral data type");
	static_assert(!ft::is_integral<ft::vector<int> >::value, "ft::vector should not be integral data type");

	// vector default constructor assert
	static_assert(std::is_constructible<ft::vector<int> >::value, "vector should have default constructor");
	static_assert(std::is_constructible<ft::vector<int>, std::allocator<int> >::value, "vector should have default constructor");

	// vector fill constructor assert
	static_assert(std::is_constructible<ft::vector<int>, int>::value, "vector should have fill constructor");
	static_assert(std::is_constructible<ft::vector<int>, int, int>::value, "vector should have fill constructor");
	static_assert(std::is_constructible<ft::vector<int>, int, int, std::allocator<int> >::value, "vector should have fill constructor");

	// vector range constructor assert
	static_assert(std::is_constructible<ft::vector<int>, ft::vector<int>::iterator, ft::vector<int>::iterator>::value, "vector should have range constructor");
	static_assert(std::is_constructible<ft::vector<int>, ft::vector<int>::iterator, ft::vector<int>::iterator, std::allocator<int> >::value, "vector should have range constructor");

	// vector copy constructor assert
	static_assert(std::is_constructible<ft::vector<int>, ft::vector<int> >::value, "vector constructor should have copy constructor");

	// vector constructor fail assert
	static_assert(!std::is_constructible<ft::vector<int>, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, A, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, int, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, A, int>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, A, A, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, A, int, int>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, int, A, int>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, int, int, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, A, A, int>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, A, int, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");
	static_assert(!std::is_constructible<ft::vector<int>, int, A, A>::value, "vector constructor shouldn't work with non-iterator class or non-integral type");

#endif
