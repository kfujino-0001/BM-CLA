// SdrHelpers.cpp

/** 
 * @file
 * Implementation of SdrHelpers.cpp
 */

#include "cla/utils/SdrHelpers.hpp"

namespace cla{

/************************************************
 * Implementation of functions.
 ***********************************************/

void copy(
	const htm::SDR& origin,
	htm::SDR& copied
) {
	copied.initialize(origin.dimensions);
	copied = origin;
}

void copy(
	const htm::PSDR& origin,
	htm::PSDR& copied
) {
	copied.initialize(
		origin.dimensions,
		origin.getSplitNum()
	);
	copied = origin;
}

} // namespace cla