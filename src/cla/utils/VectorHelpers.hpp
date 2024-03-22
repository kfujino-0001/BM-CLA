// VectorHelpers.hpp

/** 
 * @file
 * Definitions for the VectorHelpers class in C++
 */

#ifndef VECTOR_HELPERS_HPP
#define VECTOR_HELPERS_HPP

#include <iostream>
#include <vector>
#include <algorithm> // for min, max

#include "cla/utils/Checker.hpp"

namespace cla {

/************************************************
 * operators
 ***********************************************/

template<typename T>
std::ostream& operator<<(
	std::ostream& os, 
	const std::vector<T>& vec
) {
	if(vec.empty()) return os;

	for(std::size_t i = 0u, size = vec.size() - 1u; i < size; ++i)
		os << vec.at(i) << ", ";
	os << vec.back();

	return os;
}


template<typename T>
bool operator==(
	const std::vector<T>& vec1, 
	const std::vector<T>& vec2
) {
	if(vec1.size() != vec2.size()) return false;

	for(std::size_t i = 0u, size = vec1.size(); i < size; ++i)
		if(vec1.at(i) != vec2.at(i)) return false;

	return true;
}


/************************************************
 * elems selections.
 ***********************************************/

template<typename T, typename Func>
std::vector<T> select_min_elems(
	const std::vector<T>& vec1,
	const std::vector<T>& vec2,
	const Func& cmp
) {
	CLA_ASSERT(vec1.size() == vec2.size());
	std::vector<T> res;
	res.reserve(vec1.size());

	for(std::size_t i = 0u, size = vec1.size(); i < size; ++i)
		res.emplace_back(std::min(vec1.at(i), vec2.at(i), cmp));

	return res;
}

template<typename T>
std::vector<T> select_min_elems(
	const std::vector<T>& vec1,
	const std::vector<T>& vec2
) {
	CLA_ASSERT(vec1.size() == vec2.size());
	std::vector<T> res;
	res.reserve(vec1.size());

	for(std::size_t i = 0u, size = vec1.size(); i < size; ++i)
		res.emplace_back(std::min(vec1.at(i), vec2.at(i)));

	return res;
}

template<typename T, typename Func>
std::vector<T> select_max_elems(
	const std::vector<T>& vec1,
	const std::vector<T>& vec2,
	const Func& cmp
) {
	CLA_ASSERT(vec1.size() == vec2.size());
	std::vector<T> res;
	res.reserve(vec1.size());

	for(std::size_t i = 0u, size = vec1.size(); i < size; ++i)
		res.emplace_back(std::max(vec1.at(i), vec2.at(i), cmp));

	return res;
}

template<typename T>
std::vector<T> select_max_elems(
	const std::vector<T>& vec1,
	const std::vector<T>& vec2
) {
	CLA_ASSERT(vec1.size() == vec2.size());
	std::vector<T> res;
	res.reserve(vec1.size());

	for(std::size_t i = 0u, size = vec1.size(); i < size; ++i)
		res.emplace_back(std::max(vec1.at(i), vec2.at(i)));

	return res;
}


} // namespace cla

#endif // VECTOR_HELPER_HPP