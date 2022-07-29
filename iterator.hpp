#pragma once

#include "type_traits.hpp"

#include <iterator>

// #ifdef _ITERATOR_DEBUG
// #include <iostream>
// #define DEBUG(x) (std::cout << x << std::endl)
// #else
// #define DEBUG(...)
// #endif

namespace ft {
	// ft::iterator_traits
	// SFINAE trick to check if template type has certain member
	// https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function
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

	template<typename T>
	const bool _has_iterator_category<T>::value;

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
		typedef std::random_access_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};

	template<typename T>
	struct iterator_traits<const T*> {
	public:
		typedef std::random_access_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef const T*						pointer;
		typedef const T&						reference;
	};

	// ft::random_access_iterator
	template<typename TPtr, typename Container>
	class random_access_iterator {
	public:
		typedef typename ft::iterator_traits<TPtr>::difference_type		difference_type;
		typedef typename ft::iterator_traits<TPtr>::value_type			value_type;
		typedef typename ft::iterator_traits<TPtr>::pointer				pointer;
		typedef typename ft::iterator_traits<TPtr>::reference			reference;
		typedef typename ft::iterator_traits<TPtr>::iterator_category	iterator_category;

	public:
		random_access_iterator() : _ptr(0) {
			#ifdef DEBUG
				std::cout << "random access iterator default constructor called" << std::endl;
			#endif
		}

		random_access_iterator(pointer ptr) : _ptr(ptr) {
			#ifdef DEBUG
				std::cout << "random access iterator pointer constructor called" << std::endl;
			#endif
		}

		template<typename _TPtr>
		random_access_iterator(const random_access_iterator<_TPtr, typename ft::enable_if<ft::are_same<_TPtr, typename Container::pointer>::value, Container>::type>& copy) :
			_ptr(copy.base()) {
			#ifdef DEBUG
				std::cout << "random access iterator const conversion constructor called" << std::endl;
			#endif
		}

		random_access_iterator(const random_access_iterator& copy) : _ptr(copy.base()) {
			#ifdef DEBUG
				std::cout << "random access iterator copy constructor called" << std::endl;
			#endif
		}

		random_access_iterator& operator=(const random_access_iterator& rhs) {
			#ifdef DEBUG
				std::cout << "random access iterator assignment operator called" << std::endl;
			#endif
			this->_ptr = rhs._ptr;
			return *this;
		}

		~random_access_iterator() {
			#ifdef DEBUG
				std::cout << "random access iterator deconstructor called" << std::endl;
			#endif
		}

		random_access_iterator operator-(difference_type val) const {
			return random_access_iterator(this->_ptr - val);
		}

		random_access_iterator operator-=(difference_type val) {
			this->_ptr -= val;
			return *this;
		}

		random_access_iterator operator+(difference_type val) const {
			return random_access_iterator(this->_ptr + val);
		}

		random_access_iterator operator+=(difference_type val) {
			this->_ptr += val;
			return *this;
		}

		random_access_iterator& operator++() {
			++this->_ptr;
			return *this;
		}

		random_access_iterator operator++(int) {
			return random_access_iterator(this->_ptr++);
		}

		random_access_iterator& operator--() {
			--this->_ptr;
			return *this;
		}

		random_access_iterator operator--(int) {
			return random_access_iterator(this->_ptr--);
		}

		reference operator*() const {
			return *this->_ptr;
		}

		pointer operator->() const {
			return this->_ptr;
		}

		reference operator[](const difference_type& n) const {
			return *(this->_ptr + n);
		}

		pointer base() const {
			return this->_ptr;
		}

	protected:
		pointer _ptr;
	};

	template<typename TPtr1, typename TPtr2, typename Container>
	bool operator==(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<typename TPtr, typename Container>
	bool operator==(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<typename TPtr1, typename TPtr2, typename Container>
	bool operator!=(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() != rhs.base();
	}

	template<typename TPtr, typename Container>
	bool operator!=(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() != rhs.base();
	}

	template<typename TPtr1, typename TPtr2, typename Container>
	bool operator<(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() < rhs.base();
	}

	template<typename TPtr, typename Container>
	bool operator<(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() < rhs.base();
	}

	template<typename TPtr1, typename TPtr2, typename Container>
	bool operator<=(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() <= rhs.base();
	}

	template<typename TPtr, typename Container>
	bool operator<=(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() <= rhs.base();
	}

	template<typename TPtr1, typename TPtr2, typename Container>
	bool operator>(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() > rhs.base();
	}

	template<typename TPtr, typename Container>
	bool operator>(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() > rhs.base();
	}

	template<typename TPtr1, typename TPtr2, typename Container>
	bool operator>=(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template<typename TPtr, typename Container>
	bool operator>=(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template<typename TPtr1, typename TPtr2, typename Container>
	typename ft::iterator_traits<TPtr1>::difference_type operator-(const random_access_iterator<TPtr1, Container>& lhs, const random_access_iterator<TPtr2, Container>& rhs) {
		return lhs.base() - rhs.base();
	}

	template<typename TPtr, typename Container>
	typename ft::iterator_traits<TPtr>::difference_type operator-(const random_access_iterator<TPtr, Container>& lhs, const random_access_iterator<TPtr, Container>& rhs) {
		return lhs.base() - rhs.base();
	}

	template<typename TPtr, typename Container>
	random_access_iterator<TPtr, Container> operator+(typename ft::iterator_traits<TPtr>::difference_type val, const random_access_iterator<TPtr, Container>& rhs) {
		return random_access_iterator<TPtr, Container>(rhs.base() + val);
	}

	// ft:reverse_iterator
	template<typename Iterator>
	class reverse_iterator {
	public:
		typedef Iterator													iterator_type;
		typedef typename ft::iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename ft::iterator_traits<Iterator>::reference			reference;
		typedef typename ft::iterator_traits<Iterator>::iterator_category	iterator_category;

	public:
		reverse_iterator() : _iterator() {
			#ifdef DEBUG
				std::cout << "reverse_iterator default constructor called" << std::endl;
			#endif
		}

		reverse_iterator(iterator_type iterator) : _iterator(iterator) {
			#ifdef DEBUG
				std::cout << "reverse_iterator iterator constructor called" << std::endl;
			#endif
		}

		reverse_iterator(const reverse_iterator& copy) : _iterator(copy.base()) {
			#ifdef DEBUG
				std::cout << "reverse_iterator copy constructor called" << std::endl;
			#endif
		}

		template<typename _Iterator>
		reverse_iterator(const reverse_iterator<_Iterator>& copy) : _iterator(copy.base()) {
			#ifdef DEBUG
				std::cout << "reverse_iterator different iterator type copy constructor called" << std::endl;
			#endif
		}

		reverse_iterator& operator=(const reverse_iterator& rhs) {
			#ifdef DEBUG
				std::cout << "reverse_iterator assignment operator called" << std::endl;
			#endif
			this->_iterator = rhs._iterator;
			return *this;
		}

		~reverse_iterator() {
			#ifdef DEBUG
				std::cout << "reverse_iterator deconstructor called" << std::endl;
			#endif
		}

		reverse_iterator operator-(difference_type val) const {
			return reverse_iterator(this->_iterator + val);
		}

		reverse_iterator operator-=(difference_type val) {
			this->_iterator += val;
			return *this;
		}

		reverse_iterator operator+(difference_type val) const {
			return reverse_iterator(this->_iterator - val);
		}

		reverse_iterator operator+=(difference_type val) {
			this->_iterator -= val;
			return *this;
		}

		reverse_iterator& operator++() {
			--this->_iterator;
			return *this;
		}

		reverse_iterator operator++(int) {
			return reverse_iterator(this->_iterator--);
		}

		reverse_iterator& operator--() {
			++this->_iterator;
			return *this;
		}

		reverse_iterator operator--(int) {
			return reverse_iterator(this->_iterator++);
		}

		reference operator*() const {
			return *iterator_type(this->_iterator - 1);
		}

		pointer operator->() const {
			return &(this->operator*());
		}

		reference operator[](const difference_type& n) const {
			return *(*this + n);
		}

		iterator_type base() const {
			return this->_iterator;
		}

	protected:
		iterator_type _iterator;
	};

	template<typename Iterator1, typename Iterator2>
	bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<typename Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() != rhs.base();
	}

	template<typename Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.base() != rhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return rhs.base() < lhs.base();
	}

	template<typename Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs.base() < lhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return rhs.base() <= lhs.base();
	}

	template<typename Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs.base() <= lhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return rhs.base() > lhs.base();
	}

	template<typename Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs.base() > lhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return rhs.base() >= lhs.base();
	}

	template<typename Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs.base() >= lhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	typename ft::iterator_traits<Iterator1>::difference_type operator-(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return rhs.base() - lhs.base();
	}

	template<typename Iterator>
	typename ft::iterator_traits<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return rhs.base() - lhs.base();
	}

	template<typename Iterator>
	reverse_iterator<Iterator> operator+(typename ft::iterator_traits<Iterator>::difference_type val, const reverse_iterator<Iterator>& rhs) {
		return reverse_iterator<Iterator>(rhs.base() - val);
	}

	template<typename TPtr, typename Tree>
	class bst_iterator {
	private:
		typedef Tree												tree_type;
		typedef typename Tree::node_type							node_type;
		typedef typename Tree::node_pointer							node_pointer;

	public:
		typedef typename ft::iterator_traits<TPtr>::difference_type	difference_type;
		typedef typename ft::iterator_traits<TPtr>::value_type		value_type;
		typedef typename ft::iterator_traits<TPtr>::pointer			pointer;
		typedef typename ft::iterator_traits<TPtr>::reference		reference;
		typedef std::bidirectional_iterator_tag						iterator_category;

	public:
		bst_iterator() : _ptr(0), _tail(0), _isReversed(false) {
			#ifdef DEBUG
				std::cout << "bst_iterator default constructor called" << std::endl;
			#endif
		}

		bst_iterator(node_pointer ptr, node_pointer tail, bool isReversed = false) :
			_ptr(ptr),
			_tail(tail),
			_isReversed(isReversed) {
			#ifdef DEBUG
				std::cout << "bst_iterator pointer constructor called" << std::endl;
			#endif
		}

		template<typename _TPtr>
		bst_iterator(const bst_iterator<_TPtr, typename ft::enable_if<ft::are_same<_TPtr, typename Tree::node_pointer>::value, Tree>::type>& copy) :
			_ptr(copy._ptr),
			_tail(copy._tail),
			_isReversed(copy._isReversed) {
			#ifdef DEBUG
				std::cout << "random access iterator const conversion constructor called" << std::endl;
			#endif
		}

		bst_iterator(const bst_iterator& copy) :
			_ptr(copy._ptr),
			_tail(copy._tail),
			_isReversed(copy._isReversed) {
			#ifdef DEBUG
				std::cout << "random access iterator copy constructor called" << std::endl;
			#endif
		}

		bst_iterator& operator=(const bst_iterator& rhs) {
			#ifdef DEBUG
				std::cout << "random access iterator assignment operator called" << std::endl;
			#endif
			this->_ptr = rhs._ptr;
			this->_tail = rhs._tail;
			this->_isReversed = rhs._isReversed;
			return *this;
		}

		~bst_iterator() {
			#ifdef DEBUG
				std::cout << "random access iterator deconstructor called" << std::endl;
			#endif
		}

		bst_iterator& operator++() {
			this->_ptr = this->tree_increment(this->_ptr);
			return *this;
		}

		bst_iterator operator++(int) {
			bst_iterator tmp = *this;
			this->_ptr = this->tree_increment(this->_ptr);
			return tmp;
		}

		bst_iterator& operator--() {
			this->_ptr = this->tree_decrement(this->_ptr);
			return *this;
		}

		bst_iterator operator--(int) {
			bst_iterator tmp = *this;
			this->_ptr = this->tree_decrement(this->_ptr);
			return tmp;
		}

		reference operator*() const {
			return this->_ptr->value;
		}

		pointer operator->() const {
			return &(this->_ptr->value);
		}

		node_pointer address() const {
			return this->_ptr;
		}

	private:
		node_pointer tree_increment(node_pointer node) {
			if (node == NULL)
				return this->_tail;
			node_pointer current = node_type::getSuccessor(node);
			if (node == current && !this->_isReversed)
				return NULL;
			return current;
		}

		node_pointer tree_decrement(node_pointer node) {
			if (node == NULL)
				return this->_tail;
			node_pointer current = node_type::getPredecessor(node);
			if (node == current && this->_isReversed)
				return NULL;
			return current;
		}

	protected:
		node_pointer _ptr;
		node_pointer _tail;
		bool _isReversed;
	};

	template<typename TPtr1, typename TPtr2, typename Tree>
	bool operator==(const bst_iterator<TPtr1, Tree>& lhs, const bst_iterator<TPtr2, Tree>& rhs) {
		return lhs.address() == rhs.address();
	}

	template<typename TPtr, typename Tree>
	bool operator==(const bst_iterator<TPtr, Tree>& lhs, const bst_iterator<TPtr, Tree>& rhs) {
		return lhs.address() == rhs.address();
	}

	template<typename TPtr1, typename TPtr2, typename Tree>
	bool operator!=(const bst_iterator<TPtr1, Tree>& lhs, const bst_iterator<TPtr2, Tree>& rhs) {
		return lhs.address() != rhs.address();
	}

	template<typename TPtr, typename Tree>
	bool operator!=(const bst_iterator<TPtr, Tree>& lhs, const bst_iterator<TPtr, Tree>& rhs) {
		return lhs.address() != rhs.address();
	}
}
