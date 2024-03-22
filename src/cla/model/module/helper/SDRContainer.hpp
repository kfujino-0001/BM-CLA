// SDRContainer.hpp

/**
 * @file
 * Definitions for the SDRContainer class in C++
 */

#ifndef SDR_CONTAINER_HPP
#define SDR_CONTAINER_HPP

#include <memory>

#include "cla/extension/types/Psdr.hpp"
#include "htm/types/Sdr.hpp"

namespace cla {

/**
 * SDRContainer implementation in C++.
 *
 * @b Description
 * The SDRContainer is a container class that groups together multiple
 * SDRs. This class manages a group of basic SDRs in multiple types of
 * layers.
 */
struct SDRContainer {

public:

	htm::SDR activeBits;
	htm::SDR activeColumns;
	htm::SDR activeCells;
	htm::SDR winnerCells;

	htm::SDR externalActiveSDR;
	htm::SDR externalWinnerSDR;

	htm::SDR_sparse_t activeSegments;
	htm::SDR_sparse_t preActiveSegments;

public:

	/**
	 * SDRContainer constructor.
	 */
	SDRContainer() = default;

	/**
	 * SDRContainer constructor with the parameters.
	 *
	 * @param bitDimension The bit dimension.
	 * @param columnDimension The column dimension.
	 * @param cellDimension The cell dimension.
	 */
	SDRContainer(
		const std::vector<htm::UInt>& bitDimension,
		const std::vector<htm::UInt>& columnDimension,
		const std::vector<htm::UInt>& cellDimension
	);

	/**
	 * SDRContainer destructor.
	 */
	~SDRContainer() = default;

	/**
	 * Initialize the SDRContainer with the parameters.
	 *
	 * @param bitDimension The bit dimension.
	 * @param columnDimension The column dimension.
	 * @param cellDimension The cell dimension.
	 */
	void initialize(
		const std::vector<htm::UInt>& bitDimension,
		const std::vector<htm::UInt>& columnDimension,
		const std::vector<htm::UInt>& cellDimension
	);

	/**
	 * Increment container state. The previous segment state is saved
	 * to calculate the accuracy, error, etc.
	 */
	void increment();

	/**
	 * Reset sdrs of this container.
	 */
	void reset();
};

using PSDRContainer = std::unique_ptr<SDRContainer>;


} // namespace cla

#endif // SDR_CONTAINER_HPP