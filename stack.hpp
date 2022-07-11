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
		explicit stack(const container_type& ctnr = container_type()) : _container(ctnr) {
			#ifdef DEBUG
				std::cout << "stack default constructor called" << std::endl;
			#endif
		}

		stack(const stack& copy) : _container(copy._container) {
			#ifdef DEBUG
				std::cout << "stack copy constructor called" << std::endl;
			#endif
		}

		stack& operator=(const stack& other) {
			#ifdef DEBUG
				std::cout << "stack assignment operator called" << std::endl;
			#endif
			this->_container = other._container;
			return *this;
		}

		~stack() {
			#ifdef DEBUG
				std::cout << "stack deconstructor called" << std::endl;
			#endif
		}

		bool empty() const {
			return this->_container.empty();
		}

		size_type size() const {
			return this->_container.size();
		}

		value_type& top() {
			return this->_container.back();
		}

		const value_type& top() const {
			return this->_container.back();
		}

		void push(const value_type& val) {
			this->_container.push_back(val);
		}

		void pop() {
			this->_container.pop_back();
		}

	protected:
		container_type _container;
	};

	template<class T, class Container>
	bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs._container == rhs._container);
	}

	template<class T, class Container>
	bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs._container != rhs._container);
	}

	template<class T, class Container>
	bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs._container < rhs._container);
	}

	template<class T, class Container>
	bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs._container <= rhs._container);
	}

	template<class T, class Container>
	bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs._container > rhs._container);
	}

	template<class T, class Container>
	bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
		return (lhs._container >= rhs._container);
	}
}
