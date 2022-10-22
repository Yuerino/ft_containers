#pragma once

#include "functional.hpp"
#include "utility.hpp"
#include "tree.hpp"
#include "algorithm.hpp"

#include <memory>

#ifdef _MAP_DEBUG
#include <iostream>
#define MAP_DEBUG(x) (std::cout << x << std::endl)
#else
#define MAP_DEBUG(x)
#endif

namespace ft {
	template<typename Key,
			typename T,
			typename Compare = ft::less<Key>,
			typename Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
	public:
		typedef Key													key_type;
		typedef T													mapped_type;
		typedef ft::pair<const key_type, mapped_type>				value_type;
		typedef Compare												key_compare;
		typedef Allocator											allocator_type;
		typedef typename Allocator::reference						reference;
		typedef typename Allocator::const_reference					const_reference;
		typedef typename Allocator::pointer							pointer;
		typedef typename Allocator::const_pointer					const_pointer;
		typedef std::ptrdiff_t										difference_type;
		typedef std::size_t											size_type;

		class value_compare : public ft::binary_function<value_type, value_type, bool> {
			friend class map<Key, T, Compare, Allocator>;

			protected:
				key_compare comp;
				value_compare(key_compare c) : comp(c) {}

			public:
				bool operator()(const value_type& x, const value_type& y) const {
					return comp(x.first, y.first);
				}
		};

	private:
		typedef ft::red_black_tree<value_type, value_compare, allocator_type>	tree_type;

	public:
		typedef typename tree_type::iterator						iterator;
		typedef typename tree_type::const_iterator					const_iterator;
		typedef typename tree_type::reverse_iterator				reverse_iterator;
		typedef typename tree_type::const_reverse_iterator			const_reverse_iterator;

	public:
		// construct/copy/destroy

		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) :
			_comp(comp),
			_tree_data(tree_type(value_compare(comp), alloc)) {
			MAP_DEBUG("map default constructor called");
		}

		template<typename InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(), typename ft::iterator_traits<InputIterator>::iterator_category* = 0) :
			_comp(comp),
			_tree_data(tree_type(first, last, value_compare(comp), alloc)) {
			MAP_DEBUG("map range constructor called");
		}

		map(const map& x) :
			_comp(key_compare()),
			_tree_data(x._tree_data) {
			MAP_DEBUG("map copy constructor called");
		}

		~map() {
			MAP_DEBUG("map deconstructor called");
		}

		map& operator=(const map& x) {
			MAP_DEBUG("map assignment operator called");
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

		// element access

		mapped_type& operator[](const key_type& k) {
			iterator tmp = this->find(k);
			if (tmp != this->end())
				return tmp->second;
			return (this->insert(ft::make_pair<key_type, mapped_type>(k, mapped_type())).first)->second;
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

		size_type erase(const key_type& k) {
			return this->_tree_data.erase(ft::make_pair<key_type, mapped_type>(k, mapped_type()));
		}

		void erase(iterator first, iterator last) {
			this->_tree_data.erase(first, last);
		}

		void swap(map& x) {
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

		iterator find(const key_type& k) {
			return this->_tree_data.find(ft::make_pair<key_type, mapped_type>(k, mapped_type()));
		}

		const_iterator find(const key_type& k) const {
			return this->_tree_data.find(ft::make_pair<key_type, mapped_type>(k, mapped_type()));
		};

		size_type count(const key_type& k) const {
			return this->_tree_data.find(ft::make_pair<key_type, mapped_type>(k, mapped_type())) != this->end() ? 1 : 0;
		}

		iterator lower_bound(const key_type& k) {
			iterator it = this->begin();
			iterator ite = this->end();
			for(; it != ite; ++it) {
				if (!this->_comp(it->first, k))
					return it;
			}
			return ite;
		}

		const_iterator lower_bound(const key_type& k) const {
			const_iterator it = this->begin();
			const_iterator ite = this->end();
			for(; it != ite; ++it) {
				if (!this->_comp(it->first, k))
					return it;
			}
			return ite;
		}

		iterator upper_bound(const key_type& k) {
			iterator it = this->begin();
			iterator ite = this->end();
			for(; it != ite; ++it) {
				if (this->_comp(k, it->first))
					return it;
			}
			return ite;
		}

		const_iterator upper_bound(const key_type& k) const {
			const_iterator it = this->begin();
			const_iterator ite = this->end();
			for(; it != ite; ++it) {
				if (this->_comp(k, it->first))
					return it;
			}
			return ite;
		}

		ft::pair<iterator, iterator> equal_range(const key_type& k) {
			return ft::make_pair<iterator, iterator>(this->lower_bound(k), this->upper_bound(k));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
			return ft::make_pair<const_iterator, const_iterator>(this->lower_bound(k), this->upper_bound(k));
		}

		// allocator

		allocator_type get_allocator() const {
			return this->_tree_data.get_allocator();
		}

	private:
		const key_compare _comp;
		tree_type _tree_data;
	};

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		if (lhs.size() == rhs.size())
			return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
		return false;
	}

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator<=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator>(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return rhs < lhs;
	}

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator>=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return !(lhs < rhs);
	}
}

namespace std {
	template<typename Key, typename T, typename Compare, typename Alloc>
	void swap(ft::map<Key, T, Compare, Alloc>& x, ft::map<Key, T, Compare, Alloc>& y) {
		x.swap(y);
	}
}
