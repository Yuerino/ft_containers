#pragma once

#include "vector.hpp"
#ifdef DEBUG
#include <iostream>
#endif

namespace ft {
	template< typename T, typename Container = ft::vector<T> >
	class stack {
	public:
		typedef Container							container_type;
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		// typedef typename Container::reference		reference; // C++11
		// typedef typename Container::const_reference	const_reference; // C++11

	public:
		explicit stack(const container_type& ctnr = container_type()) : c(ctnr) {
			#ifdef DEBUG
				std::cout << "stack default constructor called" << std::endl;
			#endif
		}

		stack(const stack& copy) : c(copy.c) {
			#ifdef DEBUG
				std::cout << "stack copy constructor called" << std::endl;
			#endif
		}

		stack& operator=(const stack& other) {
			#ifdef DEBUG
				std::cout << "stack assignment operator called" << std::endl;
			#endif
			this->c = other.c;
			return *this;
		}

		~stack() {
			#ifdef DEBUG
				std::cout << "stack deconstructor called" << std::endl;
			#endif
		}

		bool empty() const {
			return this->c.empty();
		}

		size_type size() const {
			return this->c.size();
		}

		value_type& top() {
			return this->c.back();
		}

		const value_type& top() const {
			return this->c.back();
		}

		void push(const value_type& val) {
			this->c.push_back(val);
		}

		void pop() {
			this->c.pop_back();
		}

		template<class _T, class _Container>
		friend bool operator==(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

		template<class _T, class _Container>
		friend bool operator!=(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

		template<class _T, class _Container>
		friend bool operator<(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

		template<class _T, class _Container>
		friend bool operator<=(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

		template<class _T, class _Container>
		friend bool operator>(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

		template<class _T, class _Container>
		friend bool operator>=(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs);

	protected:
		container_type c;
	};

	template<class T, class Container>
	bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs.c == rhs.c);
	}

	template<class T, class Container>
	bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs.c != rhs.c);
	}

	template<class T, class Container>
	bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs.c < rhs.c);
	}

	template<class T, class Container>
	bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs.c <= rhs.c);
	}

	template<class T, class Container>
	bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs.c > rhs.c);
	}

	template<class T, class Container>
	bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs.c >= rhs.c);
	}
}
