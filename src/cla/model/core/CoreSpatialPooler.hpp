// CoreSpatialPooler.hpp

/**
 * @file
 * Definitions for the CoreSpatialPooler class in C++
 */

#ifndef CORE_SPATIAL_POOLER_HPP
#define CORE_SPATIAL_POOLER_HPP

#include <memory>

#include "htm/algorithms/Connections.hpp"
#include "htm/types/Sdr.hpp"

namespace cla {

/**
 * CoreSpatialPooler implementation in C++.
 *
 * @b Description
 * The CoreSpatialPooler is the base (interface) class for the spatial
 * poolers. This class defines necessary functions for the spatial pooler
 * on the cla layer.
 */
class CoreSpatialPooler {

public:

	/**
	 * CoreSpatialPooler constructor.
	 */
	CoreSpatialPooler() = default;

	/**
	 * CoreSpatialPooler destructor.
	 */
	virtual ~CoreSpatialPooler() = default;

	/**
	 * reset the internal state of the spatial pooler. The function is
	 * assumed to be executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the spatial pooler.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the spatial pooler.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the spatial pooler.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Compute the input active bits.
	 * The function converts the input active bits to an active column
	 * pattern. Furthermore, the function updates column-synapses based on
	 * the input active bits and the inner state.
	 *
	 * @param activeBits The active bits sdr
	 * @param learn The boolean value whether or not learning is enabled.
	 * @param activeColumns The active columns SDR. (This param has a
	 * return value.)
	 */
	virtual void compute(
		const htm::SDR& activeBits,
		const bool learn,
		htm::SDR& activeColumns
	) = 0;

	/**
	 * Map a column to bits. This function collects bits with the connected
	 * synapses.
	 *
	 * @param column The index of the column.
	 * @return const std::vector<htm::CellIdx> The indexes of bits.
	 */
	virtual const std::vector<htm::CellIdx> bitsForColumn(
		const htm::CellIdx column
	) const = 0;

	/**
	 * Get the column-synapses of the spatial pooler.
	 *
	 * @return const htm::Connections& the column-synapses.
	 */
	virtual const htm::Connections& getConnections() const = 0;
};

using PSpatialPooler = std::shared_ptr<CoreSpatialPooler>;


} // namespace cla

#endif // CORE_SPATIAL_POOLER_HPP