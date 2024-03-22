// FullAccepter.cpp

/** 
 * @file
 * Implementation of FullAccepter.cpp
 */

#include "cla/model/module/accepter/FullAccepter.hpp"

namespace cla {

/************************************************
 * FullAccepter public functions.
 ***********************************************/

void FullAccepter::summary(std::ostream& os) const {
	os << "FullAccepter";
}

void FullAccepter::detail(std::ostream& os) const {
	os << " -- FullAccepter -----------------------------------------"
	   << std::endl
	   << std::endl;
}

const bool FullAccepter::isAccept(const htm::SDR& inputSDR) const {
	return true;
}

} // namespace cla