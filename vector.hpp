#pragma once

#include "iterator.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "algorithm.hpp"

#include <iterator>

#include <memory>
#include <stdexcept>
#ifdef DEBUG
#include <iostream>
#endif

namespace ft {
	template< typename T, typename Allocator = std::allocator<T> >
	class vector {
	public:
		typedef T													value_type;
		typedef Allocator											allocator_type;
		typedef typename Allocator::reference						reference;
		typedef typename Allocator::const_reference					const_reference;
		typedef typename Allocator::pointer							pointer;
		typedef typename Allocator::const_pointer					const_pointer;
		typedef ft::random_access_iterator<pointer, vector>			iterator;
		typedef ft::random_access_iterator<const_pointer, vector>	const_iterator;
		typedef ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;
		typedef std::ptrdiff_t										difference_type;
		typedef std::size_t											size_type;

	public:
		// construct/copy/destroy

		/**
		 * @brief Creates a vector with no elements.
		 * @param alloc An allocator object.
		 */
		explicit vector(const allocator_type& alloc = allocator_type()) :
			_allocator(alloc),
			_storage_start(0),
			_size(0),
			_capacity(0) {
			#ifdef DEBUG
				std::cout << "vector default constructor called" << std::endl;
			#endif
		}

		/**
		 * @brief Creates a vector with copies of an exemplar element.
		 * This constructor fills the vector with n copies of value.
		 * @param n The number of elements to initially create.
		 * @param val An element to copy.
		 * @param alloc An allocator.
		 */
		explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) :
			_allocator(alloc),
			_storage_start(0),
			_size(0),
			_capacity(0) {
			#ifdef DEBUG
				std::cout << "vector fill constructor called" << std::endl;
			#endif
			this->assign(n, val);
		}

		/**
		 * @brief Builds a vector from a range.
		 * Create a vector consisting of copies of the elements from (first,last)
		 * with each element constructed from its corresponding element
		 * in that range, in the same order.
		 * @param first An input iterator.
		 * @param last An input iterator.
		 * @param alloc An allocator.
		 */
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
				typename ft::iterator_traits<InputIterator>::iterator_category* = 0) :
			_allocator(alloc),
			_storage_start(0),
			_size(0),
			_capacity(0) {
			#ifdef DEBUG
				std::cout << "vector range constructor called" << std::endl;
			#endif
			this->assign(first, last);
		}

		/**
		 * @brief vector copy constructor
		 * All the elements are copied, but any extra memory will not be copied.
		 * @param x A vector of identical element and allocator types.
		 */
		vector(const vector& x) :
			_allocator(x.get_allocator()),
			_storage_start(0),
			_size(0),
			_capacity(0){
			#ifdef DEBUG
				std::cout << "vector copy constructor called" << std::endl;
			#endif
			this->operator=(x);
		}

		/**
		 * @brief vector deconstructor
		 * The deconstructor only erases the elements, and note that if the
		 * elements themselves are pointers, the pointed-to memory is
		 * not touched in any way. Managing the pointer is the user's
		 * responsibility.
		 */
		~vector() {
			#ifdef DEBUG
				std::cout << "vector deconstructor called" << std::endl;
			#endif
			if (this->_storage_start && this->_capacity > 1)
				this->get_allocator().deallocate(this->_storage_start, this->_capacity);
		}

		/**
		 * @brief vector assignment operator.
		 * All the elements are copied, but any extra memory will not be copied.
		 * Unlike the copy constructor, the allocator object is not copied.
		 * @param x A vector of identical element and allocator types.
		 */
		vector& operator=(const vector& x) {
			#ifdef DEBUG
				std::cout << "vector assignment operator called" << std::endl;
			#endif
			this->clear();
			this->assign(x.begin(), x.end());
			return *this;
		}

		// iterators

		/**
		 * @brief Return a read/write iterator that points to the first
		 * elements in the vector. Iteration is done in ordinary element order.
		 */
		iterator begin() {
			return iterator(this->_storage_start);
		}

		/**
		 * @brief Return a read-only (constant) iterator that points to the first
		 * elements in the vector. Iteration is done in ordinary element order.
		 */
		const_iterator begin() const {
			return const_iterator(this->_storage_start);
		}

		/**
		 * @brief Returns a read/write iterator that points one past the last
		 * element in the vector. Iteration is done in ordinary element order.
		 */
		iterator end() {
			return iterator(this->_storage_start + this->size());
		}

		/**
		 * @brief Returns a read-only (constant) iterator that points one past the last
		 * element in the vector. Iteration is done in ordinary element order.
		 */
		const_iterator end() const {
			return const_iterator(this->_storage_start + this->size());
		}

		/**
		 * @brief Return a read/write reverse iterator that points to the last
		 * element in the vector. Iteration is done in reverse element order.
		 */
		reverse_iterator rbegin() {
			return reverse_iterator(this->end());
		}

		/**
		 * @brief Return a read-only (constant) reverse iterator that points to the last
		 * element in the vector. Iteration is done in reverse element order.
		 */
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(this->end());
		}

		/**
		 * @brief Return a read/write reverse iterator that points one before the first
		 * element in the vector. Iteration is done in reverse element order.
		 */
		reverse_iterator rend() {
			return reverse_iterator(this->begin());
		}

		/**
		 * @brief Return a read-only (constant) reverse iterator that points one before the first
		 * element in the vector. Iteration is done in reverse element order.
		 */
		const_reverse_iterator rend() const {
			return const_reverse_iterator(this->begin());
		}

		// capacity

		/**
		 * @brief Returns the number of elements in the vector.
		 */
		size_type size() const {
			return this->_size;
		}

		/**
		 * @brief Returns the size() of the largest possible vector.
		 */
		size_type max_size() const {
			return this->get_allocator().max_size();
		}

		/**
		 * @brief Resizes the vector to the specified number of elements.
		 * This function will resize the vector to the specified
		 * number of elements. If the number is smaller than the
		 * vector's current size the vector is truncated, otherwise
		 * the vector is extended and new elements are populated with
		 * given data.
		 * @param n Number of elements the vector should contain.
		 * @param val Data with which new elements should be populated.
		 */
		void resize(size_type n, value_type val = value_type()) {
			size_type old_size = this->_size;
			if (old_size != n)
				this->reallocate(n);
			if (n > old_size) {
				for (size_type i = old_size; i < n; ++i)
					this->get_allocator().construct(this->_storage_start + i, val);
			}
			this->_size = n;
		}

		/**
		 * @brief Returns the total number of elements that the vector can
		 * hold before needing to allocate more memory.
		 */
		size_type capacity() const {
			return this->_capacity;
		}

		/**
		 * @brief Returns true if the vector is empty.
		 * (Thus begin() would equal end())
		 */
		bool empty() const {
			return this->_size == 0;
		}

		/**
		 * @brief Attempt to preallocate enough memory for specified number of elements.
		 * This function attempts to reserve enough memory for the
		 * vector to hold the specified number of elements. If the
		 * number requested is more than max_size(), length_error is thrown.
		 * @param n Number of elements required.
		 * @throw std::length_error if n exceeds max_size().
		 */
		void reserve(size_type n) {
			if (n > this->max_size())
				throw std::length_error("reverse size exceeds max size");
			if (n > this->capacity())
				this->reallocate(n);
		}

		// element access

		/**
		 * @brief Subscript access to the data contained in the vector.
		 * This operator allows for easy, array-style, data access.
		 * Note that data access with this operator is unchecked and
		 * out_of_range lookups are not defined. (For checked lookups
		 * see at())
		 * @param n The index of the element for which data should be accessed.
		 * @return Read/write reference to data.
		 */
		reference operator[](size_type n) {
			return *(this->_storage_start + n);
		}

		const_reference operator[](size_type n) const {
			return *(this->_storage_start + n);
		}

		reference at(size_type n) {
			if (n >= this->size())
				throw std::out_of_range("out of range");
			return *(this->_storage_start + n);
		}

		const_reference at(size_type n) const {
			if (n >= this->size())
				throw std::out_of_range("out of range");
			return *(this->_storage_start + n);
		}

		reference front() {
			return *this->_storage_start;
		}

		const_reference front() const {
			return *this->_storage_start;
		}

		reference back() {
			return *(this->_storage_start + this->size() - 1);
		}

		const_reference back() const {
			return *(this->_storage_start + this->size() - 1);
		}

		// modifiers

		template<typename InputIterator>
		void assign(InputIterator first, InputIterator last, typename ft::iterator_traits<InputIterator>::iterator_category* = 0) {
			// TODO: compatiable with other iterator category
			size_type len = std::distance(first, last);
			if (len > this->capacity())
				this->reallocate(len);
			for (size_type i = 0; first != last; ++first, ++i)
				this->get_allocator().construct(this->_storage_start + i, *first);
			this->_size = len;
		}

		void assign(size_type n, const value_type& val) {
			if (n > this->capacity())
				this->reallocate(n);
			for (size_type i = 0; i < n; ++i) {
				this->get_allocator().construct(this->_storage_start + i, val);
			}
			this->_size = n;
		}

		void push_back(const value_type& val) {
			if (this->size() == this->capacity())
				this->reallocate((this->capacity() * 2) | 1);
			this->get_allocator().construct(this->_storage_start + this->size(), val);
			this->_size++;
		}

		void pop_back() {
			this->get_allocator().destroy(this->_storage_start + this->_size - 1);
			this->_size--;
		}

		iterator insert(iterator position, const value_type& val) {
			iterator it = this->begin();
			iterator it_end = this->end();
			if (this->_size == this->_capacity)
				this->reallocate(this->_size + 1);
			size_type i = std::distance(it, position);
			this->_size++;
			it += i;
			iterator it_ret = iterator(this->_storage_start + i);
			this->get_allocator().construct(this->_storage_start + i, val);
			for (++i; it != it_end; ++i, ++it)
				this->get_allocator().construct(this->_storage_start + i, *it);
			return it_ret;
		}

		void insert(iterator position, size_type n, const value_type& val) {
			iterator it = this->begin();
			iterator it_end = this->end();
			if (this->_size + n > this->_capacity)
				this->reallocate(this->_size + n);
			size_type i = std::distance(it, position);
			this->_size = this->_size + n;
			it += i;
			for (; n > 0; ++i, --n)
				this->get_allocator().construct(this->_storage_start + i, val);
			for (; it != it_end; ++i, ++it)
				this->get_allocator().construct(this->_storage_start + i, *it);
		}

		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last, typename ft::iterator_traits<InputIterator>::iterator_category* = 0) {
			iterator it = this->begin();
			iterator it_end = this->end();
			size_type n = std::distance(first, last);
			if (this->_size + n > this->_capacity)
				this->reallocate(this->_size + n);
			size_type i = std::distance(it, position);
			this->_size = this->_size + n;
			it += i;
			for (; first != last; ++i, ++first)
				this->get_allocator().construct(this->_storage_start + i, *first);
			for (; it != it_end; ++i, ++it)
				this->get_allocator().construct(this->_storage_start + i, *it);
		}

		iterator erase(iterator position) {
			iterator it = this->begin();
			iterator it_end = this->end();
			size_type i = std::distance(it, position);
			it += i;
			iterator it_ret = it;
			for (++it; it != it_end; ++i, ++it)
				this->get_allocator().construct(this->_storage_start + i, *it);
			this->get_allocator().destroy(this->_storage_start + this->_size - 1);
			this->_size--;
			return it_ret;
		}

		iterator erase(iterator first, iterator last) {
			iterator it = this->begin();
			iterator it_end = this->end();
			size_type i = std::distance(it, first);
			size_type n = std::distance(first, last) - 1;
			it += i + n;
			iterator it_ret = it - n;
			for (++it; it != it_end; ++i, ++it)
				this->get_allocator().construct(this->_storage_start + i, *it);
			for (++n; n > 0; --n) {
				this->get_allocator().destroy(this->_storage_start + this->_size - 1);
				this->_size--;
			}
			return it_ret;
		}

		void swap(vector& x) {
			ft::swap(this->_storage_start, x._storage_start);
			ft::swap(this->_size, x._size);
			ft::swap(this->_capacity, x._capacity);
		}

		void clear() {
			this->reallocate(0);
		}

		// allocator

		/**
		 * @brief Get a copy of the memory allocation object.
		 */
		allocator_type get_allocator() const {
			return allocator_type(this->_allocator);
		}

	private:
		const allocator_type _allocator;
		pointer _storage_start;
		size_type _size;
		size_type _capacity;

		void reallocate(size_type new_capacity) {
			if (this->_capacity == new_capacity) return;
			pointer new_storage = this->get_allocator().allocate(new_capacity);
			if (this->_storage_start) {
				for (size_type i = 0; this->_storage_start != 0 && i < new_capacity && i < this->_size; ++i)
					this->get_allocator().construct(new_storage + i, *(this->_storage_start + i));
				this->get_allocator().deallocate(this->_storage_start, this->_capacity);
			}
			this->_capacity = new_capacity;
			this->_size = (this->_size > new_capacity) ? new_capacity : this->_size;
			this->_storage_start = new_storage;
		}
	};

	template<typename T, typename Alloc>
	bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		if (lhs.size() == rhs.size())
			return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
		return false;
	}

	template<typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template<typename T, typename Alloc>
	bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename T, typename Alloc>
	bool operator<=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template<typename T, typename Alloc>
	bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return rhs < lhs;
	}

	template<typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template<typename T, typename Alloc>
	void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
		x.swap(y);
	}
}
