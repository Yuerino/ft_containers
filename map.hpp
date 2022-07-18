#pragma once

#include "functional.hpp"
#include "utility.hpp"

#include <memory>

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
		typedef ft::bidirectional_iterator<pointer, vector>			iterator;
		typedef ft::bidirectional_iterator<const_pointer, vector>	const_iterator;
		typedef ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;
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
		}

	public:
		// construct/copy/destroy

		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());

		template<typename InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());

		map(const map& x);

		~map();

		map& operator=(const map& x);

		// iterators

		iterator begin();

		const_iterator begin() const;

		reverse_iterator rbegin();

		const_reverse_iterator rbegin() const;

		reverse_iterator rend();

		const_reverse_iterator rend() const;

		// capacity

		bool empty() const;

		size_type size() const;

		size_type max_size() const;

		mapped_type& operator[](const key_type& k);

		// modifiers

		ft::pair<iterator, bool> insert(const value_type& val);

		iterator insert(iterator position, const value_type& val);

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last);

		void erase(iterator position);

		size_type erase(const key_type& k);

		void erase(iterator first, iterator last);

		void swap(map& x);

		void clear();

		// observers

		key_compare key_comp() const;

		value_compare value_comp() const;

		// operations

		iterator find(const key_type& k);

		const_iterator find(const key_type& k) const;

		size_type count(const key_type& k) const;

		iterator lower_bound(const key_type& k);

		const_iterator lower_bound(const key_type& k) const;

		iterator upper_bound(const key_type& k);

		const_iterator upper_bound(const key_type& k) const;

		ft::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

		ft::pair<iterator, iterator> equal_range(const key_type& k);

		// allocator

		allocator_type get_allocator() const;

	private:
		const allocator_type _allocator;
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

	template<typename Key, typename T, typename Compare, typename Alloc>
	void swap(map<Key, T, Compare, Alloc>& x, map<Key, T, Compare, Alloc>& y) {
		x.swap(y);
	}
}
