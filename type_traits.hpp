#pragma once

#include <cstddef>

namespace ft {
	// enable_if
	template<bool, typename>
	struct enable_if {};

	template<typename T>
	struct enable_if<true, T> {
		 typedef T type;
	};

	// integral_constant
	template<typename T, T v>
	struct integral_constant {
		static const T value = v;
		typedef T value_type;
		typedef integral_constant<T, v> type;
		operator T() { return v; }
	};

	template<typename T, T v>
	const T integral_constant<T, v>::value;

	typedef integral_constant<bool, false> false_type;
	typedef integral_constant<bool, true> true_type;

	// is_integral
	template<typename T> struct is_integral :			public false_type {};
	template<> struct is_integral<bool> :				public true_type {};
	template<> struct is_integral<char> :				public true_type {};
	// template<> struct is_integral<char16_t> :			public true_type {}; // C++11
	// template<> struct is_integral<char32_t> :			public true_type {}; // C++11
	template<> struct is_integral<wchar_t> :			public true_type {};
	template<> struct is_integral<signed char> :		public true_type {};
	template<> struct is_integral<short> :				public true_type {};
	template<> struct is_integral<int> :				public true_type {};
	template<> struct is_integral<long> :				public true_type {};
	// template<> struct is_integral<long long> :			public true_type {}; // C++11
	template<> struct is_integral<unsigned char> :		public true_type {};
	template<> struct is_integral<unsigned short> :		public true_type {};
	template<> struct is_integral<unsigned int> :		public true_type {};
	template<> struct is_integral<unsigned long> :		public true_type {};
	// template<> struct is_integral<unsigned long long> :	public true_type {}; // C++11

	// Compare for equality of types.
	template<typename, typename> struct are_same : public false_type {};
	template<typename T> struct are_same<T, T> : public true_type {};
}
