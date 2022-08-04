#pragma once

#include "iterator.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "algorithm.hpp"

#include <iterator>
#include <memory>
#include <stdexcept>
#include <algorithm>

#ifdef VECTOR_DEBUG
#include <iostream>
#define VECTOR_LOG(s) (std::clog << __FUNCTION__ << "::" << __LINE__ << ": " << s << std::endl)
#else
#define VECTOR_LOG(s)
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
			_capacity(0) { }

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
			this->assign(x.begin(), x.end());
		}

		/**
		 * @brief vector deconstructor
		 * The deconstructor only erases the elements, and note that if the
		 * elements themselves are pointers, the pointed-to memory is
		 * not touched in any way. Managing the pointer is the user's
		 * responsibility.
		 */
		~vector() {
			this->destroy_from_end(this->_size);
			this->_allocator.deallocate(this->_storage_start, this->_capacity);
		}

		/**
		 * @brief vector assignment operator.
		 * All the elements are copied, but any extra memory will not be copied.
		 * Unlike the copy constructor, the allocator object is not copied.
		 * @param x A vector of identical element and allocator types.
		 */
		vector& operator=(const vector& x) {
			if (&x == this) return *this;
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
			if (n < this->_size) {
				this->destroy_from_end(this->_size - n);
			} else if (n > this->_size) {
				this->_insert(this->end(), n - this->_size, val);
			}
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
				throw std::length_error("vector: reverse");
			if (n > this->_capacity)
				this->increase_capacity(n);
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
			typedef typename ft::iterator_traits<InputIterator>::iterator_category iterator_category;
			this->_assign(first, last, iterator_category());
		}

		void assign(size_type n, const value_type& val) {
			if (n > this->_capacity)
				this->assign_realloc(n, val);
			else if (n > this->_size) {
				std::fill(this->begin(), this->end(), val);
				std::uninitialized_fill_n(this->end(), n - this->_size, val);
				this->_size = n;
			} else {
				std::fill_n(this->begin(), n, val);
				this->destroy_from_end(this->_size - n);
			}
		}

		void push_back(const value_type& val) {
			if (this->size() == this->capacity())
				this->increase_capacity(this->get_new_size(1, "vector: push_back"));
			this->get_allocator().construct(this->_storage_start + this->size(), val);
			this->_size++;
		}

		void pop_back() {
			this->get_allocator().destroy(this->_storage_start + this->_size - 1);
			this->_size--;
		}

		iterator insert(iterator position, const value_type& val) {
			size_type position_idx = ft::distance(this->begin(), position);
			this->_insert(position, 1, val);
			return iterator(this->_storage_start + position_idx);
		}

		void insert(iterator position, size_type n, const value_type& val) {
			this->_insert(position, n, val);
		}

		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last, typename ft::iterator_traits<InputIterator>::iterator_category* = 0) {
			typedef typename ft::iterator_traits<InputIterator>::iterator_category iterator_category;
			this->range_insert(position, first, last, iterator_category());
		}

		iterator erase(iterator position) {
			if (position + 1 != this->end())
				std::copy(position + 1, this->end(), position);
			--this->_size;
			this->_allocator.destroy(this->_storage_start + this->_size);
			return position;
		}

		iterator erase(iterator first, iterator last) {
			if (first == last) return first;
			if (last != this->end())
				std::copy(last, this->end(), first);
			this->destroy_from_end(ft::distance(first, last));
			return first;
		}

		void swap(vector& x) {
			ft::swap(this->_storage_start, x._storage_start);
			ft::swap(this->_size, x._size);
			ft::swap(this->_capacity, x._capacity);
		}

		void clear() {
			this->destroy_from_end(this->_size);
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

		/**
		 * @brief Range assign with forward iterator base
		 * @note Assign vector with [first, last) value and destroy the leftover
		 * in the vector.
		 * @note If [first, last) size exceed the vector size, add the rest of
		 * [first, last) value by inserting to the vector at the end
		 */
		template<typename InputIterator>
		void _assign(InputIterator first, InputIterator last, std::input_iterator_tag) {
			pointer _begin = this->_storage_start;
			pointer _end = this->_storage_start + this->_size;
			for (; first != last && _begin != _end ; ++first, ++_begin)
				*_begin = *first;
			if (first == last)
				this->destroy_from_end(_end - _begin);
			else
				this->insert(this->end(), first, last);
		}

		/**
		 * @brief Range assign with forward iterator base
		 * @note If not enough capacity: create temporary pointer and assign the
		 * new value in there, in case of failure vector should be valid and
		 * stay the same
		 * @note If enough capacity, assign vector with [first, last) value,
		 * destroy the leftover in the vector if there are or if [first, last)
		 * exceed vector size then construct the rest of the new value
		 */
		template<typename InputIterator>
		void _assign(InputIterator first, InputIterator last, std::forward_iterator_tag) {
			size_type len = ft::distance(first, last);
			if (len > this->_capacity) {
				size_type new_capacity = len;
				pointer new_storage_start = this->_allocator.allocate(new_capacity);
				size_type new_size = new_capacity;
				try {
					std::uninitialized_copy(first, last, new_storage_start);
				} catch (...) {
					this->_allocator.deallocate(new_storage_start, new_capacity);
					throw;
				}
				this->destroy_from_end(this->_size);
				this->_allocator.deallocate(this->_storage_start, this->_capacity);
				this->_storage_start = new_storage_start;
				this->_size = new_size;
				this->_capacity = new_capacity;
			}
			else if (len > this->_size) {
				InputIterator mid = first;
				std::advance(mid, this->_size);
				std::copy(first, mid, this->_storage_start);
				std::uninitialized_copy(mid, last, this->_storage_start + this->_size);
				this->_size = len;
			} else {
				std::copy(first, last, this->_storage_start);
				this->destroy_from_end(this->_size - len);
			}
		}

		/**
		 * @brief Reallocate vector and fill it with val value
		 * @note This doesn't check if new_capacity is within max_size
		 * @note This will create temporary pointer with new_capacity capacity
		 * and fill it with val value so in case of failure this will guarantee
		 * vector will be valid and stays the same (Strong guarantee)
		 */
		void assign_realloc(size_type new_capacity, const value_type& val) {
			// Strong guarantee
			pointer new_storage_start = this->_allocator.allocate(new_capacity);
			size_type new_size = new_capacity;
			try {
				std::uninitialized_fill_n(new_storage_start, new_size, val);
			} catch (...) {
				this->_allocator.deallocate(new_storage_start, new_capacity);
				throw;
			}
			this->destroy_from_end(this->_size);
			this->_allocator.deallocate(this->_storage_start, this->_capacity);
			this->_storage_start = new_storage_start;
			this->_size = new_size;
			this->_capacity = new_capacity;
		}

		/**
		 * @brief Increase the capacity of the vector with current elements
		 * @note This doesn't check if new_capacity is within max_size
		 * @note This will create temporary pointer with new_capacity capacity
		 * and copy everything over so in case of failure this will guarantee
		 * vector will be valid and stays the same (Strong guarantee)
		 */
		void increase_capacity(size_type new_capacity) {
			// Strong guarantee
			pointer new_storage_start = this->_allocator.allocate(new_capacity);
			size_type new_size = this->_size;
			try {
				std::uninitialized_copy(this->_storage_start, this->_storage_start + this->_size, new_storage_start);
			} catch (...) {
				this->_allocator.deallocate(new_storage_start, new_capacity);
				throw;
			}
			this->destroy_from_end(this->_size);
			this->_allocator.deallocate(this->_storage_start, this->_capacity);
			this->_storage_start = new_storage_start;
			this->_size = new_size;
			this->_capacity = new_capacity;
		}

		/**
		 * @brief Range insert with input iterator
		 * @note If position is at the end, simply insert it one by one
		 * @note Otherwise, create a temporary with [first, last) elements and
		 * call insert again with random iterator
		 */
		template<typename InputIterator>
		void range_insert(iterator position, InputIterator first, InputIterator last, std::input_iterator_tag) {
			if (position != this->end()) {
				ft::vector<value_type> tmp(first, last, this->get_allocator());
				this->insert(position, tmp.begin(), tmp.end());
			} else {
				for(; first != last; ++first)
					this->push_back(*first);
			}
		}

		/**
		 * @brief Range insert with forward iterator base
		 * @note If not enough capacity: Create new temporary pointer and
		 * perform copy in there, in case of failure destroy the temporary
		 * (Strong guarantee)
		 * @note If enough capacity and insert at the end: Simply construct the
		 * new elements, in case of failure destroy the already constructed new
		 * element up until then. (Strong guarantee)
		 * @note Otherwise relocate the current elements to make place for new
		 * elements and copy the new elements over. In case of failure,
		 * guarantee the vector will be in valid state but elements may change
		 * depends on where the failure occur (Basic guarantee)
		 */
		template<typename InputIterator>
		void range_insert(iterator position, InputIterator first, InputIterator last, std::forward_iterator_tag) {
			if (first == last) return;
			size_type n = ft::distance(first, last);
			if (this->_capacity - this->_size >= n) {
				if (position != this->end()) {
					// Basic guarantee
					size_type position_idx = ft::distance(this->begin(), position);
					this->relocate_safe(position_idx, position_idx + n);
					std::copy(first, last, position);
				} else {
					// Strong guarantee
					std::uninitialized_copy(first, last, position);
					this->_size += n;
				}
			} else {
				// Strong guarantee
				size_type new_capacity = get_new_size(n, "vector: range insert");
				pointer new_storage_start = this->_allocator.allocate(new_capacity);
				size_type new_size = 0;
				pointer new_storage_end = NULL;
				try {
					new_storage_end = std::uninitialized_copy(this->_storage_start, position.base(), new_storage_start);
					new_storage_end = std::uninitialized_copy(first, last, new_storage_end);
					new_storage_end = std::uninitialized_copy(position.base(), this->_storage_start + this->_size, new_storage_end);
					new_size = new_storage_end - new_storage_start;
				} catch (...) {
					if (new_storage_end != NULL) {
						new_size = new_storage_end - new_storage_start;
						for (size_type i = 0; i < new_size; ++i)
							this->_allocator.destroy(new_storage_start + i);
					}
					this->_allocator.deallocate(new_storage_start, new_capacity);
					throw;
				}
				this->destroy_from_end(this->_size);
				this->_allocator.deallocate(this->_storage_start, this->_capacity);
				this->_storage_start = new_storage_start;
				this->_size = new_size;
				this->_capacity = new_capacity;
			}
		}

		/**
		 * @brief Move every elements backward from from_position to the new
		 * to_position safely
		 * @note If vector doesn't have enough capacity for the new elements,
		 * this causes undefined behaviour.
		 * @note If to_position is at uninitialized position and not at the end
		 * of vector, it will automatically initialize position between the end
		 * and to_position with default_val. This is to make sure in case
		 * of moving elements to to_position fails, vector is still valid state.
		 * @note This will first move possible elements to uninitialized
		 * position by simply constructing it. Then copy the rest backwardly to
		 * avoid overlapping.
		 * @exception Guarantee vector will be in valid state but elements may
		 * change. (Basic guarantee)
		 */
		void relocate_safe(size_type from_position, size_type to_position, const value_type& default_val = value_type()) {
			size_type distance = std::min(this->_size, to_position) - from_position;
			pointer end_of_storage = this->_storage_start + this->_size;
			if (to_position > this->_size) {
				std::uninitialized_fill(end_of_storage, end_of_storage + to_position, default_val);
				this->_size += to_position - this->_size;
			}
			std::uninitialized_copy(end_of_storage - distance, end_of_storage, this->_storage_start + std::max(this->_size, to_position));
			this->_size += distance;
			std::copy_backward(this->_storage_start + from_position, end_of_storage - distance, end_of_storage);
		}

		/**
		 * @brief Get new size to extend vector
		 * @param size_to_extend size to extend the vector
		 * @param s string to add trace where this could be called from
		 * @exception length_error if desired new size is bigger than max size
		 */
		size_type get_new_size(size_type size_to_extend, const char *s) {
			if (this->max_size() - this->_size < size_to_extend)
				throw std::length_error(s);

			// size_type new_size = this->_size + std::max(this->_size, size_to_extend); // gcc
			size_type new_size = this->_capacity + std::max(this->_capacity, size_to_extend); // clang
			return new_size > this->max_size() ? this->max_size() : new_size;
		}

		/**
		 * @brief Fill insert
		 * @note If not enough capacity: Create new temporary pointer and
		 * perform copy in there, in case of failure destroy the temporary
		 * (Strong guarantee)
		 * @note If enough capacity and insert at the end: Simply fill new
		 * values by constructing them, in case of failure destroy the already
		 * constructed new element up until then. (Strong guarantee)
		 * @note Otherwise relocate the current elements to make place for new
		 * elements and fill the new elements. In case of failure, guarantee the
		 * vector will be in valid state but elements may change depends on
		 * where the failure occur (Basic guarantee)
		 */
		void _insert(iterator position, size_type n, const value_type& val) {
			if (n == 0) return;
			if (this->_capacity - this->_size >= n) {
				if (position != this->end()) {
					// Basic guarantee
					size_type position_idx = ft::distance(this->begin(), position);
					this->relocate_safe(position_idx, position_idx + n, val);
					std::fill_n(position, n, val);
				} else {
					// Strong guarantee
					std::uninitialized_fill_n(this->_storage_start + this->_size, n, val);
					this->_size += n;
				}
			} else {
				// Strong guarantee
				size_type new_capacity = get_new_size(n, "vector: _insert");
				pointer new_storage_start = this->_allocator.allocate(new_capacity);
				size_type new_size = 0;
				pointer new_storage_end = NULL;
				try {
					new_storage_end = std::uninitialized_copy(this->_storage_start, position.base(), new_storage_start);
					std::uninitialized_fill_n(new_storage_end, n, val);
					new_storage_end += n;
					new_storage_end = std::uninitialized_copy(position.base(), this->_storage_start + this->_size, new_storage_end);
					new_size = new_storage_end - new_storage_start;
				} catch (...) {
					if (new_storage_end != NULL) {
						new_size = new_storage_end - new_storage_start;
						for (size_type i = 0; i < new_size; ++i)
							this->_allocator.destroy(new_storage_start + i);
					}
					this->_allocator.deallocate(new_storage_start, new_capacity);
					throw;
				}
				this->destroy_from_end(this->_size);
				this->_allocator.deallocate(this->_storage_start, this->_capacity);
				this->_storage_start = new_storage_start;
				this->_size = new_size;
				this->_capacity = new_capacity;
			}
		}

		/**
		 * @brief Destroy n amounts of element from end of storage
		 * @param n size of elements to destroy
		 * @note If n equal size of vector, this destroy all elements.
		 * @note If n bigger than size of vector this causes undefined behaviour.
		 */
		void destroy_from_end(size_type n) {
			for (size_type i = this->_size - 1; n > 0; --i, --n) {
				this->_allocator.destroy(this->_storage_start + i);
				this->_size--;
			}
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

namespace std {
	template<typename T, typename Alloc>
	void swap(ft::vector<T, Alloc>& x, ft::vector<T, Alloc>& y) {
		x.swap(y);
	}
}
