#pragma once

#include "type_traits.hpp"

#include <iterator>

// #define DEBUG

// TODO: documentation needed

namespace ft {
	// iterator category
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	// iterator_traits
	template<typename T>
	struct _has_iterator_category {
		private:
			template<class _T>
			static char _test(typename _T::iterator_category* = 0);
			template<class _T>
			static int _test(...);

		public:
			static const bool value = sizeof(_test<T>(0)) == 1;
	};

	template<typename, bool>
	struct _iterator_traits {};

	template<typename Iterator>
	struct _iterator_traits<Iterator, true> {
	public:
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	template<typename Iterator>
	struct iterator_traits : public _iterator_traits<Iterator, ft::_has_iterator_category<Iterator>::value> {};

	template<typename T>
	struct iterator_traits<T*> {
	public:
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef std::ptrdiff_t				difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<typename T>
	struct iterator_traits<const T*> {
	public:
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef std::ptrdiff_t				difference_type;
		typedef const T*					pointer;
		typedef const T&					reference;
	};

	template<typename TPtr, typename Container>
	class random_access_iterator {
	public:
		typedef typename ft::iterator_traits<TPtr>::difference_type		difference_type;
		typedef typename ft::iterator_traits<TPtr>::value_type			value_type;
		typedef typename ft::iterator_traits<TPtr>::pointer				pointer;
		typedef typename ft::iterator_traits<TPtr>::reference			reference;
		typedef typename ft::iterator_traits<TPtr>::iterator_category	iterator_category;

	public:
		random_access_iterator() : _ptr(NULL) {
			#ifdef DEBUG
				std::cout << "random access iterator default constructor called" << std::endl;
			#endif
		}

		random_access_iterator(pointer ptr) : _ptr(ptr) {
			#ifdef DEBUG
				std::cout << "random access iterator pointer constructor called" << std::endl;
			#endif
		}

		template<typename Iterator>
		random_access_iterator(const random_access_iterator<Iterator, typename ft::enable_if<(ft::are_same<Iterator, typename Container::pointer>::value), Container>::type>& rhs) :
			_ptr(rhs.address()) {}

		random_access_iterator(const random_access_iterator& copy) : _ptr(copy.address()) {
			#ifdef DEBUG
				std::cout << "random access iterator copy constructor called" << std::endl;
			#endif
		}

		random_access_iterator& operator=(const random_access_iterator& rhs) {
			#ifdef DEBUG
				std::cout << "random access iterator assignment operator called" << std::endl;
			#endif
			this->_ptr = rhs.address();
			return *this;
		}

		~random_access_iterator() {
			#ifdef DEBUG
				std::cout << "random access iterator deconstructor called" << std::endl;
			#endif
		}

		random_access_iterator operator-(int val) {
				random_access_iterator tmp(*this);
				tmp -= val;
				return tmp;
			}

		random_access_iterator& operator++() {
			++this->_ptr;
			return *this;
		}

		random_access_iterator operator++(int) {
			random_access_iterator tmp(*this);
			this->operator++();
			return tmp;
		}

		bool operator!=(const random_access_iterator& rhs) const {
			return this->address() != rhs.address();
		}

		reference operator*() {
			return *this->address();
		}

		pointer address() const {
			return this->_ptr;
		}

	protected:
		pointer _ptr;
	};

	template<typename Iterator1, typename Iterator2, typename Container>
	typename ft::iterator_traits<Iterator1>::difference_type operator-(const random_access_iterator<Iterator1, Container>& lhs, const random_access_iterator<Iterator2, Container>& rhs) {
		return lhs.address() - rhs.address();
	}

	// ft::distance
	// TODO: compatible with other iterator category
	template<typename Iterator>
	typename ft::iterator_traits<Iterator>::difference_type _do_distance(Iterator first, Iterator last, input_iterator_tag) {
		typedef typename ft::iterator_traits<Iterator>::difference_type difference_type;

		difference_type result = 0;
		while (first != last) {
			++first;
			++result;
		}
		return result;
	}

	template<typename Iterator>
	typename ft::iterator_traits<Iterator>::difference_type _do_distance(Iterator first, Iterator last, random_access_iterator_tag) {
		return last - first;
	}

	template<typename Iterator>
	typename ft::iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last) {
		return ft::_do_distance(first, last, typename ft::iterator_traits<Iterator>::iterator_category());
	}
}
