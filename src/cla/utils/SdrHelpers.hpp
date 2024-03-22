// SdrHelpers.hpp

/** 
 * @file
 * Definitions for the SdrHelpers class in C++
 */

#ifndef SDR_HELPERS_HPP
#define SDR_HELPERS_HPP

#include "htm/types/Sdr.hpp"
#include "cla/extension/types/SdrExtension.hpp"
#include "cla/extension/types/Psdr.hpp"
#include "cla/extension/types/PSdrExtension.hpp"

namespace cla {

void copy(
	const htm::SDR& origin,
	htm::SDR& copied
);

void copy(
	const htm::PSDR& origin,
	htm::PSDR& copied
);

template<typename ExData>
void copy(
	const htm::SDRex<ExData>& origin,
	htm::SDRex<ExData>& copied
) {
	copied.initialize(origin.dimensions);
	copied = origin;
}

template<typename ExData>
void copy(
	const htm::PSDRex<ExData>& origin,
	htm::PSDRex<ExData>& copied
) {
	copied.initialize(
		origin.dimensions,
		origin.getSplitNum()
	);
	copied = origin;
}

} // namespace cla

#endif // SDR_HELPERS_HPP