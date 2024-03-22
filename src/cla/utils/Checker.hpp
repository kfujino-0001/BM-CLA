// Checker.hpp

/** 
 * @file
 * Definitions for the Checker class in C++
 */

#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <iostream>

#define CLA_ALERT(msg) \
	std::cerr << __FILE__ << ":" << __LINE__ << ": runtime error: " << msg << std::endl;\
	std::abort();

#define CLA_ASSERT(condition) \
	if(!(condition)){std::abort();}

#define CLA_CHECK(condition, msg) \
	if(!(condition)){\
		CLA_ALERT(msg)}


#endif // CHECKER_HPP