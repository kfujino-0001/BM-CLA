// MathHelpers.hpp

/** 
 * @file
 * Definitions for the MathHelpers class in C++
 */

#ifndef MATH_HELPERS_HPP
#define MATH_HELPERS_HPP

namespace cla {

template<typename T, typename U>
const U map(
	const T& aval, 
	const T& abegin, const T& aend, 
	const U& bbegin, const U& bend
) {
	return static_cast<U>(aval - abegin) * (bend - bbegin) 
		/ static_cast<U>(aend - abegin) + bbegin;
}

} // namespace cla

#endif // MATH_HELPERS_HPP