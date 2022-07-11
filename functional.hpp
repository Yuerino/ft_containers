#pragma once

namespace ft {
	// ft::binary_function
	template<typename Arg1, typename Arg2, typename Result>
	struct binary_function {
		typedef Arg1	first_argument_type;
		typedef Arg2	second_argument_type;
		typedef Result	result_type;
	};

	// ft::less
	template<typename T>
	struct less : binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x < y;
		}
	};
}
