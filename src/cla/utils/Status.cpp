// Status.cpp

/**
 * @file
 * Implementation of Status.cpp
 */

#include <algorithm> // for find_if

#include "cla/utils/Checker.hpp"
#include "cla/utils/Status.hpp"

namespace cla {

/************************************************
 * StatusFuncs static public functions.
 ***********************************************/

const Status StatusFuncs::toStatus(const std::string& name) {
	CLA_CHECK(isExist(name), "The inputted name is not exist in status.")

	return _map.at(name);
}

const std::string StatusFuncs::toName(const Status& status) {
	const auto cmp
		= [&](const auto& value) { return value.second == status; };

	// Processing assumes that the input status is in _map.
	return std::find_if(_map.begin(), _map.end(), cmp)->first;
}

const bool StatusFuncs::isExist(const std::string& name) {
	return _map.find(name) != _map.end();
}

} // namespace cla