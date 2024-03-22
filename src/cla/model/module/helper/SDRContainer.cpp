// SDRContainer.cpp

/** 
 * @file
 * Implementation of SDRContainer.cpp
 */

#include "cla/model/module/helper/SDRContainer.hpp"

namespace cla {

/************************************************
 * SDRContainer public functions.
 ***********************************************/

SDRContainer::SDRContainer(
	const std::vector<htm::UInt>& bitDimension,
	const std::vector<htm::UInt>& columnDimension,
	const std::vector<htm::UInt>& cellDimension
) {
	initialize(bitDimension, columnDimension, cellDimension);
}

void SDRContainer::initialize(
	const std::vector<htm::UInt>& bitDimension,
	const std::vector<htm::UInt>& columnDimension,
	const std::vector<htm::UInt>& cellDimension
) {
	reset();

	activeBits.initialize(bitDimension);
	activeColumns.initialize(columnDimension);
	activeCells.initialize(cellDimension);
	winnerCells.initialize(cellDimension);

	externalActiveSDR.initialize({0u});
	externalWinnerSDR.initialize({0u});
}

void SDRContainer::increment() {
	activeBits.zero();
	activeColumns.zero();
	activeCells.zero();
	winnerCells.zero();

	externalActiveSDR.zero();
	externalWinnerSDR.zero();

	std::swap(preActiveSegments, activeSegments);
	activeSegments.clear();
}

void SDRContainer::reset() {
	activeBits.zero();
	activeColumns.zero();
	activeCells.zero();
	winnerCells.zero();

	externalActiveSDR.zero();
	externalWinnerSDR.zero();
	
	activeSegments.clear();
	preActiveSegments.clear();
}

} // namespace cla