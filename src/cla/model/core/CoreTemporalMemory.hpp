// CoreTemporalMemory.hpp

/**
 * @file
 * Definitions for the CoreTemporalMemory class in C++
 */

#ifndef CORE_TEMPORAL_MEMORY_HPP
#define CORE_TEMPORAL_MEMORY_HPP

#include <memory>

#include "htm/algorithms/Connections.hpp"
#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"
#include "htm/types/Sdr.hpp"

namespace cla {

/**
 * CoreTemporalMemory implementation in C++.
 *
 * @b Description
 * The CoreTemporalMemory is the base (interface) class for the temporal poolers. This
 * class defines the necessary functions for temporal memory on the cla
 * layer.
 */
class CoreTemporalMemory {

public:

	/**
	 * CoreTemporalMemory constructor.
	 */
	CoreTemporalMemory() = default;

	/**
	 * CoreTemporalMemory destructor.
	 */
	virtual ~CoreTemporalMemory() = default;

	/**
	 * reset the internal state of the temporal memory. The function is
	 * assumed to be executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the temporal memory.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the temporal memory.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the temporal memory.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Compute the input active columns.
	 * The function converts the input active columns to an active cells
	 * pattern and a winner cells pattern. Futhermore, the function updates
	 * cell-synapses based on the active columns and the inner state.
	 *
	 * @param activeColumns The active column sdr
	 * @param learn The boolean value whether or not learning is enabled.
	 * @param activeCells The active cell sdr (This param has a
	 * return value.)
	 * @param winnerCells The winner cell sdr (This param has a
	 * return value.)
	 */
	virtual void compute(
		const htm::SDR& activeColumns,
		const bool learn,
		htm::SDR& activeCells,
		htm::SDR& winnerCells
	) = 0;

	/**
	 * Activate segments on the cells.
	 * The function select segments to be activated based on the internal
	 * active sdr and internal winner sdr, external active sdr.
	 *
	 * @param learn The boolean value whether or not learning is enabled.
	 * @param activeSegments The active segment sdr (This param has a
	 * return value.)
	 */
	virtual void activate(
		const bool learn,
		htm::SDR_sparse_t& activeSegments
	) = 0;

	/**
	 * Activate segments on the cells.
	 * The function select segments to be activated based on the internal
	 * active sdr, internal winner sdr, external active sdr, and external
	 * winner sdr.
	 *
	 * @param externalActiveSDR The external active sdr.
	 * @param externalWinnerSDR The external winner sdr.
	 * @param learn The boolean value whether or not learning is enabled.
	 * @param activeSegments The active segment sdr (This param has a
	 * return value.)
	 */
	virtual void activate(
		const htm::SDR& externalActiveSDR,
		const htm::SDR& externalWinnerSDR,
		const bool learn,
		htm::SDR_sparse_t& activeSegments
	) = 0;

	/**
	 * Map a segment to a cell.
	 *
	 * @param segment The index of the segment.
	 * @return htm::CellIdx The index of the cell.
	 */
	virtual htm::CellIdx cellForSegment(const htm::Segment segment) const = 0;

	/**
	 * Map a cell to a column
	 * @param cell The index of the cell.
	 * @return htm::CellIdx The index of the column.
	 */
	virtual htm::CellIdx columnForCell(const htm::CellIdx cell) const = 0;

	/**
	 * Get the cell-synapses of the temporal memory.
	 *
	 * @return const htm::Connections& the cell-synapses.
	 */
	virtual const htm::Connections& getConnections() const = 0;

	/**
	 * Get the cell-synapses connection handler. This handler has the
	 * synapses log on each step.
	 *
	 * @return const htm::TMConnectionsHandler& The connection handler.
	 */
	virtual const htm::TMConnectionsHandler& getConnectionHandler() const = 0;

	/**
	 * Get the anomaly value.
	 * 
	 * @return const double The anomaly value.
	 */
	virtual const htm::Real getAnomaly() const = 0;

};

using PTemporalMemory = std::shared_ptr<CoreTemporalMemory>;


} // namespace cla

#endif // CORE_TEMPORAL_MEMORY_HPP