#pragma once

namespace ft {
	template<typename InputIterator1, typename InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
		for (; first1 != last1; ++first1, ++first2)
			if (*first1 != *first2)
				return false;
		return true;
	}

	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred) {
		for (; first1 != last1; ++first1, ++first2)
			if (!pred(*first1, *first2))
				return false;
		return true;
	}

	template<typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (*first2 < *first1)
				return false;
			else if (*first1 < *first2)
				return true;
		}
		return first2 != last2;
	}

	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate pred) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (pred(*first2, *first1))
				return false;
			else if (pred(*first1, *first2))
				return true;
		}
		return first2 != last2;
	}
}
