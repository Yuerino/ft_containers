#include "vector.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"
#include "stack.hpp"
#include "tree.hpp"
#include "map.hpp"
#include "set.hpp"

#include <iostream>
// #include <type_traits> // C++11
#include <vector>
#include <stack>
#include <set>
#include <cstdlib>
#include <sstream>
#include <exception>

struct A {};

// class B {
// public:
//     B() : l(nullptr), i(1) {};
// 	B(B const &b) : l(nullptr), i(b.i) {};
//     B& operator=(B const &b) { this->i = b.i; return *this; };
//     B(const int &ex) {
//         this->i = ex;
//         this->l = new char('a');
//     };
//     virtual ~B() {
// 		if (this->l)
// 			delete this->l;
//         this->l = nullptr;
// 		this->i = 64;
//     };

// 	char *l;
// 	int i;
// };

// class C : public B {
// public:
//     C() : B() {};
// 	C(C const &) : B() {};
//     C& operator=(C const &) = default;
//     C(const B* ex) : B() {
// 		if (ex == nullptr) return;
// 		std::cerr << "C ctor: " << ex->i << "\n";
//         this->l = new char(*(ex->l));
//         this->i = ex->i;
//         if (ex->i < 0) throw "n";
//     }
//     ~C() {
// 		std::cerr << "C dtor: " << this->i << "\n";
// 		if (this->l)
// 			delete this->l;
//         this->l = nullptr;
// 		this->i = 64;
//     };
// };

// struct BaseDerivedInt {
// 	int nbr;
// 	int* leak;

// 	BaseDerivedInt() : nbr(42), leak(0) { }
// 	BaseDerivedInt(const int& n) : nbr(n), leak(new int(69)) { }
// 	BaseDerivedInt(const int& n, int* l) : nbr(n), leak(l) { }
// 	BaseDerivedInt(const BaseDerivedInt& copy) : nbr(copy.nbr), leak(0) { }
// 	BaseDerivedInt& operator=(const BaseDerivedInt& other) {
// 		this->nbr = other.nbr;
// 		return *this;
// 	}
// 	virtual ~BaseDerivedInt() {
// 		this->nbr++;
// 		if (this->leak)
// 			delete this->leak;
// 		this->leak = NULL;
// 	}
// };

// struct DerivedInt : public BaseDerivedInt {
// 	DerivedInt() : BaseDerivedInt() { }
// 	DerivedInt(const DerivedInt& copy) : BaseDerivedInt(copy.nbr, NULL) { }
// 	DerivedInt& operator=(const DerivedInt& other) {
// 		this->nbr = other.nbr;
// 		return *this;
// 	}
// 	DerivedInt(const BaseDerivedInt* base) : BaseDerivedInt() {
// 		if (base == NULL) return;
// 		if (base->nbr < 0) throw "42";
// 		this->nbr = base->nbr;
// 		this->leak = new int(69);
// 	}
// 	~DerivedInt() {
// 		this->nbr++;
// 		if (this->leak)
// 			delete this->leak;
// 		this->leak = NULL;
// 	}
// };

bool g_vector_force_exception = false;

struct DerivedInt {
	int nbr;
	int* leak;

	DerivedInt() : nbr(-1), leak(new int(69)) {
		if (g_vector_force_exception) throw "42";
	}
	DerivedInt(const int& val) : nbr(val), leak(new int(69)) {
		if (g_vector_force_exception) throw "42";
	}
	DerivedInt(const DerivedInt& copy) : nbr(copy.nbr), leak(new int(69)) {
		if (g_vector_force_exception) throw "42";
	}
	DerivedInt& operator=(const DerivedInt& other) {
		if (g_vector_force_exception) throw "42";
		if (&other == this) return *this;
		this->nbr = other.nbr;
		return *this;
	}
	virtual ~DerivedInt() {
		if (this->leak)
			delete this->leak;
		// This is to invalidate data at those memory in case user try to access them
		this->nbr++;
		this->leak = NULL;
	}
};


std::ostream& operator<<(std::ostream& os, const DerivedInt& i) {
	os << i.nbr;
	return os;
}

template<class T, class U>
void	isEqual(T const & first, U const & second)
{
	if (first == second)
		std::cout << "[OK]";
	else
		std::cout << "[KO]" << first << "!=" << second;
}

template<class M1, class M2>
void	BasicMapStats(M1 const & map1, M2 const & map2)
{
	isEqual(map1.size(), map2.size());
	isEqual(map1.empty(), map2.empty());
}

template <class T1, class T2>
void	RelationalTest(T1 const & lhs1, T1 const & rhs1, T2 const & lhs2, T2 const & rhs2)
{
	isEqual(lhs1 != rhs1, lhs2 != rhs2);
	isEqual(lhs1 == rhs1, lhs2 == rhs2);
	isEqual(lhs1 < rhs1, lhs2 < rhs2);
	isEqual(lhs1 <= rhs1, lhs2 <= rhs2);
	isEqual(lhs1 > rhs1, lhs2 > rhs2);
	isEqual(lhs1 >= rhs1, lhs2 >= rhs2);
}

template<class Iter1First, class Iter1Last, class Iter2First, class Iter2Last>
void	RBTSetIteratorTest(Iter1First my_it, Iter1Last my_ite, Iter2First std_it, Iter2Last std_ite)
{
	isEqual(*my_it, *std_it);
	isEqual(*(my_it++), *(std_it++));
	isEqual(*(++my_it), *(++std_it));
	isEqual(*(--my_ite), *(--std_ite));
	isEqual(*(my_ite--), *(std_ite--));
}

int main() {
	// {
	// 	std::unique_ptr<BaseDerivedInt> k2(new BaseDerivedInt(5));
	// 	std::unique_ptr<BaseDerivedInt> k3(new BaseDerivedInt(6));
	// 	std::unique_ptr<BaseDerivedInt> k4(new BaseDerivedInt(-2));
	// 	std::vector<BaseDerivedInt*> v1(5);
	// 	v1.reserve(10);
	// 	v1.push_back(&(*k2));
	// 	v1.push_back(&(*k3));
	// 	v1.push_back(&(*k4));

	// 	// ft::vector<DerivedInt> vv(2);
	// 	// ft::vector<BaseDerivedInt*> v1(5);
		// std::vector<DerivedInt> vv;
		// vv.reserve(20);
		// vv.erase();

	// 	// try { vv.insert(vv.end(), v1.end() - 1, v1.end() - 1); }
	// 	// catch (...) {
	// 	// 	std::cout << "here\n";
	// 	// }
	// 	try { vv.insert(vv.begin(), v1.begin(), v1.end()); }
	// 	catch (...) {
	// 		std::cout << "here1\n";
	// 	}
	// 	std::cout << "size: " << vv.size();
	// 	std::cout << " cap: " << vv.capacity() << std::endl;
	// 	for (auto& e : vv) {
	// 			std::cout << "here1: " << e << " " << &e << std::endl;
	// 	}
	// }
	// {
	// 	std::vector<int> v1;
	// 	for (int i=0; i < 5; ++i) v1.push_back(i);

	// 	ft::vector<int> v2;
	// 	for (int i=10; i < 15; ++i) v2.push_back(i);

	// 	v2.insert(v2.end(), v1.rbegin(), v1.rend());
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
	// {
	// 	ft::vector<int> myvector;
	// 	myvector.push_back(1); myvector.push_back(2);
	// 	ft::vector<int>::rai_reverse_iterator rit1 = myvector.rbegin();
	// 	ft::vector<int>::rai_reverse_iterator rit2 = myvector.rbegin();
	// 	ft::vector<int>::rai_reverse_iterator rit3 = myvector.rend();
	// 	ft::vector<int>::const_reverse_iterator rit4 = myvector.rbegin();

	// 	std::cout << "rit1 == rit2: " << (rit1 == rit2) << std::endl;
	// 	std::cout << "rit1 == rit3: " << (rit1 == rit3) << std::endl;
	// 	std::cout << "rit1 == rit4: " << (rit1 == rit4) << std::endl;
	// }
	// {
	// 	ft::vector<int> myvector;
	// 	for(int i=1;i<=5;++i) myvector.push_back(i);
	// 	ft::vector<int>::const_iterator it = myvector.begin();
	// 	ft::vector<int>::const_reverse_iterator rit = myvector.rbegin();
	// 	std::cout << *rit << std::endl;
	// 	std::cout << "myvector contains:";
	// 	for (unsigned i=0; i<myvector.size(); ++i)
	// 		std::cout << ' ' << myvector[i];
	// 	std::cout << '\n';
	// 	// ++(*rit);
	// 	std::cout << "myvector contains:";
	// 	for (unsigned i=0; i<myvector.size(); ++i)
	// 		std::cout << ' ' << myvector[i];
	// 	std::cout << '\n';
	// 	std::cout << *rit << std::endl;
	// 	std::cout << rit[2] << std::endl;
	// }
	// {
	// 	ft::stack<int> mystack;
	// 	for (int i=0; i<5; ++i) mystack.push(i);

	// 	std::cout << "Popping out elements...";
	// 	while (!mystack.empty())
	// 	{
	// 		std::cout << ' ' << mystack.top();
	// 		mystack.pop();
	// 	}
	// 	std::cout << '\n';
	// }
	// {
	// 	ft::red_black_tree<int> rbt;
	// 	rbt.insert(50);
	// 	rbt.insert(30);
	// 	rbt.insert(65);
	// 	rbt.insert(55);
	// 	rbt.insert(35);
	// 	rbt.insert(70);
	// 	rbt.insert(15);
	// 	rbt.insert(68);
	// 	rbt.insert(80);
	// 	rbt.insert(90);
	// 	rbt.insert(69);
	// 	rbt.insert(120);
	// 	rbt.insert(56);

	// 	ft::red_black_tree<int>::iterator it = rbt.begin();
	// 	for (; it != rbt.end(); ++it) {
	// 		std::cout << *it << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// 	rbt.printTree();
	// 	std::cout << std::endl;

	// 	rbt.erase(55);
	// 	rbt.erase(30);
	// 	rbt.erase(80);
	// 	rbt.erase(50);
	// 	rbt.erase(35);
	// 	rbt.erase(15);
	// 	rbt.erase(70);
	// 	rbt.erase(65);
	// 	// rbt.erase(68);
	// 	// for (it = rbt.begin(); it != rbt.end(); ++it) {
	// 	// 	std::cout << *it << std::endl;
	// 	// }
	// 	rbt.printTree();
	// 	std::cout << rbt.size() << std::endl;
	// 	// --it;
	// 	// std::cout << *it << std::endl;
	// 	// --it;
	// 	// std::cout << *it << std::endl;
	// }
	// {
	// 	std::vector<int> myvector;
	// 	myvector.push_back(5);
	// 	myvector.push_back(10);
	// 	std::vector<int>::iterator ite = myvector.end();
	// 	std::cout << *ite << std::endl;
	// 	++ite;
	// 	std::cout << *ite << std::endl;
	// 	++ite;
	// 	std::cout << *ite << std::endl;
	// }
	// {
	// 	std::set<int> myset;
	// 	myset.insert(5);
	// 	myset.insert(2);
	// 	std::set<int>::iterator it = myset.begin();
	// 	std::set<int>::iterator ite = myset.end();
	// 	for (; it != ite; ++it)
	// 		std::cout << *it << std::endl;
	// }
	// {
	// 	std::set<int> myset;
	// 	myset.insert(5);
	// 	myset.insert(2);
	// 	std::set<int>::iterator ite = myset.end(); // one past the end
	// 	--ite; // increment past one past the end, on mac segfault but on linux it will be 2
	// 	--ite; // 5 on linux
	// }
	// {
	// 	ft::map<int, std::string> mymap;
	// 	mymap.insert(ft::make_pair<int, std::string>(42, "hello"));
	// 	mymap.insert(ft::make_pair<int, std::string>(69, "nice"));

	// 	ft::map<int, std::string>::iterator it = mymap.begin();
	// 	for(; it != mymap.end(); ++it) {
	// 		std::cout << "key: " << it->first << ", value: " << it->second << std::endl;
	// 	}
	// }
	// {
	// 	int size = 100;
	// 	ft::map<int, int> m;
	// 	for (int i = 0; i < size; ++i) {
	// 		int a = std::rand() % size;
	// 		int b = std::rand() % size;
	// 		std::cout << a << " " << b << std::endl;
	// 		m.insert(ft::make_pair<int, int>(a, b));
	// 	}

	// 	for (int i = 0; i < size; ++i) {
	// 		int b = std::rand() % size;
	// 		std::cout << "delete: " << b << std::endl;
	// 		m.erase(b);
	// 	}

	// 	ft::map<int, int>::iterator it = m.begin();
	// 	for(; it != m.end(); ++it) {
	// 		std::cout << "key: " << it->first << ", value: " << it->second << std::endl;
	// 	}
	// }
	// {
	// 	ft::map<int, std::string> mymap;
	// 	typedef ft::map<int, std::string>::value_type value_type;
	// 	typedef ft::map<int, std::string>::iterator iterator;
	// 	// ft::pair<iterator, bool> tmp;

	// 	// tmp = mymap.insert(value_type(42, "lol"));
	// 	// std::cout << (tmp.first)->first << std::endl;
	// 	// std::cout << "Created new node: " << tmp.second << std::endl;
	// 	iterator it = mymap.insert(mymap.begin(), value_type(42, "lol"));
	// 	std::cout << it->first << std::endl;
	// }
	// {
	// 	std::istringstream str("54 34 5754 344 5 6456 345 0 -1 42 69 55123");
	// 	ft::vector<int> v((std::istream_iterator<int>(str)), std::istream_iterator<int>());
	// 	for (ft::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
	// 		std::cout << *it << std::endl;
	// }
	// {
	// 	ft::vector<DerivedInt> v;
	// 	v.push_back(5);
	// 	// for (int i=0; i<10; ++i) v.push_back(WrapInt(i));
	// 	// v.clear();
	// 	v.insert(v.begin(), 2, 42);

	// 	ft::vector<DerivedInt>::iterator it = v.begin();
	// 	ft::vector<DerivedInt>::iterator ite = v.end();
	// 	for (; it != ite; ++it) std::cout << *it << std::endl;
	// 	// std::vector<int> v;
	// 	// // v.push_back(5);
	// 	// // v.push_back(6);
	// 	// // v.push_back(7);
	// 	// // v.push_back(8);
	// 	// for (int i=0; i<10; ++i) v.push_back(i);
	// 	// // v.clear();
	// 	// v.insert(v.begin(), 2, 42);

	// 	// std::vector<int>::iterator it = v.begin();
	// 	// std::vector<int>::iterator ite = v.end();
	// 	// for (; it != ite; ++it) std::cout << *it << std::endl;
	// }
	// {
	// 	ft::vector<int> vector;
	// 	ft::vector<int> v;
	// 	vector.assign(9900 * 1, 1);
	// 	vector.resize(5000 * 1);
	// 	vector.reserve(5000 * 1);
	// 	v.push_back(vector.size());
	// 	v.push_back(vector.capacity());
	// 	vector.resize(7000 * 1);
	// 	v.push_back(vector.size());
	// 	v.push_back(vector.capacity());
	// 	vector.resize(15300 * 1, int());
	// 	v.push_back(vector.size());
	// 	v.push_back(vector.capacity());

	// 	std::cout << std::endl << "std: [ ";
	// 	for (ft::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	// 		std::cout << *it << " ";
	// 	std::cout << " ]\n";
	// }
	{
		std::vector<DerivedInt> v(10, 42);
		std::cerr << "here1\n";
		g_vector_force_exception = true;
		v.resize(8);
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
