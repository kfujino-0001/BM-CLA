// DirectAdapter.cpp

/** 
 * @file
 * Implementation of DirectAdapter.cpp
 */

#include "cla/model/module/adapter/DirectAdapter.hpp"

namespace cla {

/************************************************
 * DirectAdapter public functions.
 ***********************************************/

void DirectAdapter::summary(std::ostream& os) const {
	os << "DirectAdapter";
}

void DirectAdapter::detail(std::ostream& os) const {
	os << " -- DirectAdapter ----------------------------------------"
	   << std::endl
	   << std::endl;
}

void DirectAdapter::adapt(
	const htm::SDR& inputSDR, 
	htm::SDR& activeBits
)  const {
	activeBits = inputSDR;
}

} // namespace cla