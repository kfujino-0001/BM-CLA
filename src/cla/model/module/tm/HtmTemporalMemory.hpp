// HtmTemporalMemory.hpp

/**
 * @file
 * Definitions for the HtmTemporalMemory class in C++
 */

#ifndef HTM_TEMPORAL_MEMORY_HPP
#define HTM_TEMPORAL_MEMORY_HPP

#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"

#include "cla/model/core/CoreTemporalMemory.hpp"

namespace cla {

/**
 * HtmTemporalMemory implementation in C++.
 *
 * @b Description
 * The HtmTemporalMemory is extended class of CoreTemporalMemory.
 * The HtmTemporalMemory is a class that implements a general HTM temporal
 * memory behavior. It basically has the htm::TemporalMemoryExtension
 * functions.
 */
class HtmTemporalMemory : public CoreTemporalMemory {

private:

	htm::TemporalMemoryExtension tm_;

public:

	/**
	 * HtmTemporalMemory constructor with the parameters.
	 *
	 * @param numRegions
	 * The number of the regions on this layer. (// TODO remove this
	 * parameter.)
	 *
	 * @param columnDimensions
	 * Dimensions of the column space
	 *
	 * @param cellsPerColumn
	 * Number of cells per column
	 *
	 * @param activationThreshold
	 * If the number of active connected synapses on a segment is at least
	 * this threshold, the segment is said to be active.
	 *
	 * @param initialPermanence
	 * Initial permanence of a new synapse.
	 *
	 * @param connectedPermanence
	 * If the permanence value for a synapse is greater than this value, it
	 * is said to be connected.
	 *
	 * @param minThreshold
	 * If the number of potential synapses active on a segment is at least
	 * this threshold, it is said to be "matching" and is eligible for
	 * learning.
	 *
	 * @param maxNewSynapseCount
	 * The maximum number of synapses added to a segment during learning.
	 *
	 * @param permanenceIncrement
	 * Amount by which permanences of synapses are incremented during
	 * learning.
	 *
	 * @param permanenceDecrement
	 * Amount by which permanences of synapses are decremented during
	 * learning.
	 *
	 * @param predictedSegmentDecrement
	 * Amount by which segments are punished for incorrect predictions.
	 *
	 * Note: A good value is just a bit larger than
	 * (the column-level sparsity * permanenceIncrement). So, if
	 * column-level sparsity is 2% and permanenceIncrement is 0.01, this
	 * parameter should be something like 4% * 0.01 = 0.0004).
	 *
	 * @param seed
	 * Seed for the random number generator.
	 *
	 * @param maxSegmentsPerCell
	 * The maximum number of segments per cell.
	 * The value you can choose here is limited by the type SegmentIdx
	 * in Connections.hpp, change it if you need larger values.
	 *
	 * @param maxSynapsesPerSegment
	 * The maximum number of synapses per segment.
	 * The value you can choose here is limited by the type SynapseIdx
	 * in Connections.hpp, change it there if you needed to use large
	 * values.
	 *
	 * @param checkInputs
	 * Whether to check that the activeColumns are sorted without
	 * duplicates. Disable this for a small speed boost.
	 * DEPRECATED: The SDR class now enforces these properties.
	 *
	 * @param externalPredictiveInputs
	 * Number of external predictive inputs.  These values are not related
	 * to this TM, they represent input from a different region.  This TM
	 * will form synapses with these inputs in addition to the cells which
	 * are part of this TemporalMemoryExtension.  If this is given (and
	 * greater than 0) then the active cells and winner cells of these
	 * external inputs must be given to methods TM.compute and
	 * TM.activateDendrites
	 *
	 * @param anomalyMode (optional, default `ANMode::RAW`)from enum
	 * ANMode, how is `TM.anomaly` computed. Options ANMode {DISABLED, RAW,
	 * LIKELIHOOD, LOGLIKELIHOOD}
	 *
	 * @param synapseDestinationWeight
	 * The weight represents the ratio of the number of creating synapses
	 * from the internal cells or external cells.
	 *
	 * @param createSynWeight
	 * The creating weight parameter for ASA. If the weight is 0.0, ASA
	 * creates new synapses by ASA.
	 *
	 * @param destroySynWeight
	 * The destroy weight parameter for ASA. If the weight is 0.0, ASA
	 * destroys excessive synapses by ASA.
	 *
	 * @param activateWeight
	 * The weight parameter for ASA. This parameter controls the number of
	 * creating/destroying synapses by ASA.
	 *
	 * @param capacityOfNbActiveSegments
	 * The number capacity for active segments. (// TODO: Remove this
	 * parameter.)
	 *
	 * @param capacityOfNbMatchingSegments
	 * The number capacity for matching segments. (// TODO: Remove this
	 * parameter.)
	 *
	 * @param innerSegmentSelectorMode
	 * The mode for inner segment selector. (// TODO: Remove this
	 * parameter.)
	 *
	 * @param outerSegmentSelectorMode
	 * The mode for outer segment selector. (// TODO: Remove this
	 * parameter.)
	 *
	 */
	HtmTemporalMemory(
		const htm::UInt numRegions = 1u,
		const std::vector<htm::CellIdx>& columnDimensions = {2048u},
		const htm::CellIdx cellsPerColumn = 32u,
		const htm::SynapseIdx activationThreshold = 13u,
		const htm::Permanence initialPermanence = 0.21f,
		const htm::Permanence connectedPermanence = 0.50f,
		const htm::SynapseIdx minThreshold = 10u,
		const htm::SynapseIdx maxNewSynapseCount = 20u,
		const htm::Permanence permanenceIncrement = 0.10f,
		const htm::Permanence permanenceDecrement = 0.10f,
		const htm::Permanence predictedSegmentDecrement = 0.0f,
		const htm::Int seed = 42,
		const htm::SegmentIdx maxSegmentsPerCell = 255u,
		const htm::SynapseIdx maxSynapsesPerSegment = 255u,
		const bool checkInputs = true,
		const bool exceptionHandling = false,
		const htm::UInt externalPredictiveInputs = 0u,
		const htm::Real synapseDestinationWeight = 0.5f,
		const htm::Real createSynWeight = 0.0f,
		const htm::Real destroySynWeight = 0.0f,
		const htm::Real activateWeight = 1.0f,
		const htm::UInt capacityOfNbActiveSegments = 0u,
		const htm::UInt capacityOfNbMatchingSegments = 0u,
		const htm::SSMode innerSegmentSelectorMode
		= static_cast<htm::SSMode>(0),
		const htm::SSMode outerSegmentSelectorMode
		= static_cast<htm::SSMode>(0),
		const htm::TemporalMemoryExtension::ANMode anomalyMode
		= htm::TemporalMemoryExtension::ANMode::RAW
	);

	/**
	 * HtmTemporalMemory destructor.
	 */
	~HtmTemporalMemory() = default;

	/**
	 * Initialize the HtmTemporalMemory with the parameters. For more
	 * detail of parameters, See the constructor.
	 */
	void initialize(
		const htm::UInt numRegions = 1u,
		const std::vector<htm::CellIdx>& columnDimensions = {2048u},
		const htm::CellIdx cellsPerColumn = 32u,
		const htm::SynapseIdx activationThreshold = 13u,
		const htm::Permanence initialPermanence = 0.21f,
		const htm::Permanence connectedPermanence = 0.50f,
		const htm::SynapseIdx minThreshold = 10u,
		const htm::SynapseIdx maxNewSynapseCount = 20u,
		const htm::Permanence permanenceIncrement = 0.10f,
		const htm::Permanence permanenceDecrement = 0.10f,
		const htm::Permanence predictedSegmentDecrement = 0.0f,
		const htm::Int seed = 42,
		const htm::SegmentIdx maxSegmentsPerCell = 255u,
		const htm::SynapseIdx maxSynapsesPerSegment = 255u,
		const bool checkInputs = true,
		const bool exceptionHandling = false,
		const htm::UInt externalPredictiveInputs = 0u,
		const htm::Real synapseDestinationWeight = 0.5f,
		const htm::Real createSynWeight = 0.0f,
		const htm::Real destroySynWeight = 0.0f,
		const htm::Real activateWeight = 1.0f,
		const htm::UInt capacityOfNbActiveSegments = 0u,
		const htm::UInt capacityOfNbMatchingSegments = 0u,
		const htm::SSMode innerSegmentSelectorMode
		= static_cast<htm::SSMode>(0),
		const htm::SSMode outerSegmentSelectorMode
		= static_cast<htm::SSMode>(0),
		const htm::TemporalMemoryExtension::ANMode anomalyMode
		= htm::TemporalMemoryExtension::ANMode::RAW
	);

	/**
	 * reset the internal state of the temporal memory. The function is
	 * assumed to be executed at every time step.
	 */
	void restate() override {}

	/**
	 * Reset internal state of the temporal memory.
	 */
	void reset() override;

	/**
	 * Summarize the temporal memory.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the temporal memory.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

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
	void compute(
		const htm::SDR& activeColumns,
		const bool learn,
		htm::SDR& activeCells,
		htm::SDR& winnerCells
	) override;

	/**
	 * Activate segments on the cells.
	 * The function select segments to be activated based on the internal
	 * active sdr and internal winner sdr, external active sdr.
	 *
	 * @param learn The boolean value whether or not learning is enabled.
	 * @param activeSegments The active segment sdr (This param has a
	 * return value.)
	 */
	void activate(
		const bool learn, 
		htm::SDR_sparse_t& activeSegments
	) override;

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
	void activate(
		const htm::SDR& externalActiveSDR,
		const htm::SDR& externalWinnerSDR,
		const bool learn,
		htm::SDR_sparse_t& activeSegments
	) override;

	/**
	 * Map a segment to a cell.
	 *
	 * @param segment The index of the segment.
	 * @return htm::CellIdx The index of the cell.
	 */
	htm::CellIdx cellForSegment(const htm::Segment segment) const override;

	/**
	 * Map a cell to a column
	 * @param cell The index of the cell.
	 * @return htm::CellIdx The index of the column.
	 */
	htm::CellIdx columnForCell(const htm::CellIdx cell) const override;

	/**
	 * Get the cell-synapses of the temporal memory.
	 *
	 * @return const htm::Connections& the cell-synapses.
	 */
	const htm::Connections& getConnections() const override;

	/**
	 * Get the cell-synapses connection handler. This handler has the
	 * synapses log on each step.
	 *
	 * @return const htm::TMConnectionsHandler& The connection handler.
	 */
	const htm::TMConnectionsHandler& getConnectionHandler() const override;

	/**
	 * Get the anomaly value.
	 *
	 * @return const double The anomaly value.
	 */
	const htm::Real getAnomaly() const override;

};

} // namespace cla

#endif // HTM_TEMPORAL_MEMORY_HPP