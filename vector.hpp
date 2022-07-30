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
		typedef ft::pointer_iterator<pointer, vector>				iterator;
		typedef ft::pointer_iterator<const_pointer, vector>			const_iterator;
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
			this->reallocate(0);
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
			if (this->_size != n)
				this->reallocate(n);
			if (n > this->_size) {
				for (size_type i = this->_size; i < n; ++i)
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
			this->reallocate(0);
			typedef typename ft::iterator_traits<InputIterator>::iterator_category iterator_category;
			this->_assign(first, last, iterator_category());
		}

		void assign(size_type n, const value_type& val) {
			this->reallocate(0);
			this->reallocate(n);
			for (size_type i = 0; i < n; ++i)
				this->get_allocator().construct(this->_storage_start + i, val);
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
			size_type start = ft::distance(this->begin(), position);
			if (this->_size == this->_capacity)
				this->reallocate(this->_size + 1);
			iterator ite = this->end(); --ite;

			for (size_type i = this->_size; ite != this->begin() + start - 1; --i, --ite) {
				this->get_allocator().construct(this->_storage_start + i, *ite);
				this->_allocator.destroy(this->_storage_start + i - 1);
			}
			this->get_allocator().construct(this->_storage_start + start, val);

			this->_size++;
			return iterator(this->_storage_start + start);
		}

		void insert(iterator position, size_type n, const value_type& val) {
			size_type start = ft::distance(this->begin(), position);
			if (this->_size + n > this->_capacity)
				this->reallocate(this->_size + n);
			iterator ite = this->end(); --ite;

			for (size_type i = this->_size + n - 1; ite != this->begin() + start - 1; --i, --ite) {
				this->get_allocator().construct(this->_storage_start + i, *ite);
				this->_allocator.destroy(this->_storage_start + i - n);
			}
			this->_size = this->_size + n;
			for (; n > 0; ++start, --n)
				this->get_allocator().construct(this->_storage_start + start, val);
		}

		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last, typename ft::iterator_traits<InputIterator>::iterator_category* = 0) {
			size_type start = ft::distance(this->begin(), position);
			size_type n = ft::distance(first, last);
			if (this->_size + n > this->_capacity)
				this->reallocate(this->_size + n);
			iterator ite = this->end(); --ite;

			for (size_type i = this->_size + n - 1; ite != this->begin() + start - 1; --i, --ite) {
				this->get_allocator().construct(this->_storage_start + i, *ite);
				this->_allocator.destroy(this->_storage_start + i - n);
			}

			for (; first != last; ++start, ++first)
				this->get_allocator().construct(this->_storage_start + start, *first);
			this->_size = this->_size + n;
		}

		iterator erase(iterator position) {
			size_type start = ft::distance(this->begin(), position);
			iterator it = this->begin() + start + 1;
			for (size_type i = start; it != this->end(); ++i, ++it) {
				this->get_allocator().construct(this->_storage_start + i, *it);
				this->_allocator.destroy(this->_storage_start + i + 1);
			}
			this->get_allocator().destroy(this->_storage_start + this->_size - 1);
			this->_size--;
			return this->begin() + start;
		}

		iterator erase(iterator first, iterator last) {
			size_type n = ft::distance(first, last);
			size_type start = ft::distance(this->begin(), first);
			iterator it = this->begin() + start + n;
			for (size_type i = start; it != this->end(); ++i, ++it) {
				this->get_allocator().construct(this->_storage_start + i, *it);
				this->_allocator.destroy(this->_storage_start + i + n);
			}
			this->_size -= n;
			for (; n > 0; --n)
				this->get_allocator().destroy(this->_storage_start + this->_size - 1);
			return this->begin() + start;
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
		allocator_type _allocator;
		pointer _storage_start;
		size_type _size;
		size_type _capacity;

		void reallocate(size_type new_capacity) {
			if (this->_capacity == new_capacity) return;
			pointer new_storage = NULL;
			if (new_capacity > 0)
				new_storage = this->get_allocator().allocate(new_capacity);
			if (this->_storage_start) {
				for (size_type i = 0; this->_storage_start != 0 && i < new_capacity && i < this->_size; ++i)
					this->get_allocator().construct(new_storage + i, *(this->_storage_start + i));
				for (size_type i = 0; i < this->_size; ++i)
					this->get_allocator().destroy(this->_storage_start + i);
				this->get_allocator().deallocate(this->_storage_start, this->_capacity);
			}
			this->_capacity = new_capacity;
			this->_size = (this->_size > new_capacity) ? new_capacity : this->_size;
			this->_storage_start = new_storage;
		}

		template<typename InputIterator>
		void _assign(InputIterator first, InputIterator last, std::input_iterator_tag) {
			for (; first != last; ++first)
				this->push_back(*first);
		}

		template<typename InputIterator>
		void _assign(InputIterator first, InputIterator last, std::forward_iterator_tag) {
			size_type len = ft::distance(first, last);
			this->reallocate(len);
			for (size_type i = 0; i < len; ++first, ++i)
				this->get_allocator().construct(this->_storage_start + i, *first);
			this->_size = len;
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
