#pragma once

#include "functional.hpp"
#include "utility.hpp"
#include "tree.hpp"
#include "algorithm.hpp"

#include <memory>

namespace ft {
	template<typename T,
			typename Compare = ft::less<T>,
 			typename Allocator = std::allocator<T> >
	class set {
	public:
		typedef T													key_type;
		typedef T													value_type;
		typedef Compare												key_compare;
		typedef Compare												value_compare;
		typedef Allocator											allocator_type;
		typedef typename Allocator::reference						reference;
		typedef typename Allocator::const_reference					const_reference;
		typedef typename Allocator::pointer							pointer;
		typedef typename Allocator::const_pointer					const_pointer;
		typedef std::ptrdiff_t										difference_type;
		typedef std::size_t											size_type;

	private:
		typedef ft::red_black_tree<value_type, value_compare, allocator_type>	tree_type;

	public:
		typedef typename tree_type::const_iterator						iterator;
		typedef typename tree_type::const_iterator					const_iterator;
		typedef typename tree_type::const_reverse_iterator				reverse_iterator;
		typedef typename tree_type::const_reverse_iterator			const_reverse_iterator;

	public:
		// construct/copy/destroy

		explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) :
			_comp(comp),
			_tree_data(tree_type(value_compare(comp), alloc)) {
		}

		template<typename InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(), typename ft::iterator_traits<InputIterator>::iterator_category* = 0) :
			_comp(comp),
			_tree_data(tree_type(first, last, value_compare(comp), alloc)) {
		}

		set(const set& x) :
			_comp(key_compare()),
			_tree_data(x._tree_data) {
		}

		~set() {}

		set& operator=(const set& x) {
			if (this == &x) return *this;
			this->_tree_data = x._tree_data;
			return *this;
		}

		// iterators

		iterator begin() {
			return this->_tree_data.begin();
		}

		const_iterator begin() const {
			return this->_tree_data.begin();
		}

		iterator end() {
			return this->_tree_data.end();
		}

		const_iterator end() const {
			return this->_tree_data.end();
		}

		reverse_iterator rbegin() {
			return this->_tree_data.rbegin();
		}

		const_reverse_iterator rbegin() const {
			return this->_tree_data.rbegin();
		}

		reverse_iterator rend() {
			return this->_tree_data.rend();
		}

		const_reverse_iterator rend() const {
			return this->_tree_data.rend();
		}

		// capacity

		bool empty() const {
			return this->_tree_data.empty();
		}

		size_type size() const {
			return this->_tree_data.size();
		}

		size_type max_size() const {
			return this->_tree_data.max_size();
		}

		// modifiers

		ft::pair<iterator, bool> insert(const value_type& val) {
			return this->_tree_data.insert(val);
		}

		// @note: ignore position hint for now (minimal performance lost)
		iterator insert(iterator position, const value_type& val) {
			(void)position;
			return this->_tree_data.insert(val).first;
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last, typename ft::iterator_traits<InputIterator>::iterator_category* = 0) {
			this->_tree_data.insert(first, last);
		}

		void erase(iterator position) {
			this->_tree_data.erase(position);
		}

		size_type erase(const value_type& val) {
			return this->_tree_data.erase(val);
		}

		void erase(iterator first, iterator last) {
			this->_tree_data.erase(first, last);
		}

		void swap(set& x) {
			if (this == &x) return;
			this->_tree_data.swap(x._tree_data);
		}

		void clear() {
			this->_tree_data.clear();
		}

		// observers

		key_compare key_comp() const {
			return key_compare();
		}

		value_compare value_comp() const {
			return value_compare(this->_comp);
		}

		// operations

		iterator find(const value_type& val) {
			return this->_tree_data.find(val);
		}

		const_iterator find(const value_type& val) const {
			return this->_tree_data.find(val);
		};

		size_type count(const value_type& val) const {
			return this->_tree_data.find(val) != this->end() ? 1 : 0;
		}

		iterator lower_bound(const value_type& val) {
			iterator it = this->begin();
			iterator ite = this->end();
			for(; it != ite; ++it) {
				if (!this->_comp(*it, val))
					return it;
			}
			return ite;
		}

		const_iterator lower_bound(const value_type& val) const {
			const_iterator it = this->begin();
			const_iterator ite = this->end();
			for(; it != ite; ++it) {
				if (!this->_comp(*it, val))
					return it;
			}
			return ite;
		}

		iterator upper_bound(const value_type& val) {
			iterator it = this->begin();
			iterator ite = this->end();
			for(; it != ite; ++it) {
				if (this->_comp(val, *it))
					return it;
			}
			return ite;
		}

		const_iterator upper_bound(const value_type& val) const {
			const_iterator it = this->begin();
			const_iterator ite = this->end();
			for(; it != ite; ++it) {
				if (this->_comp(val, *it))
					return it;
			}
			return ite;
		}

		ft::pair<iterator, iterator> equal_range(const value_type& val) {
			return ft::make_pair<iterator, iterator>(this->lower_bound(val), this->upper_bound(val));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const value_type& val) const {
			return ft::make_pair<const_iterator, const_iterator>(this->lower_bound(val), this->upper_bound(val));
		}

		// allocator

		allocator_type get_allocator() const {
			return this->_tree_data.get_allocator();
		}

	private:
		const key_compare _comp;
		tree_type _tree_data;
	};

	template<typename T, typename Compare, typename Alloc>
	bool operator==(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs) {
		if (lhs.size() == rhs.size())
			return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
		return false;
	}

	template<typename T, typename Compare, typename Alloc>
	bool operator!=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template<typename T, typename Compare, typename Alloc>
	bool operator<(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename T, typename Compare, typename Alloc>
	bool operator<=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template<typename T, typename Compare, typename Alloc>
	bool operator>(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs) {
		return rhs < lhs;
	}

	template<typename T, typename Compare, typename Alloc>
	bool operator>=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs) {
		return !(lhs < rhs);
	}
}

namespace std {
	template<typename T, typename Compare, typename Alloc>
	void swap(ft::set<T, Compare, Alloc>& x, ft::set<T, Compare, Alloc>& y) {
		x.swap(y);
	}
}
