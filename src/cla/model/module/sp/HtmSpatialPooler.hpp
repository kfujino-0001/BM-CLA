// HtmSpatialPooler.hpp

/**
 * @file
 * Definitions for the HtmSpatialPooler class in C++
 */

#ifndef HTM_SPATIAL_POOLER_HPP
#define HTM_SPATIAL_POOLER_HPP

#include "cla/extension/algorithms/SpatialPoolerExtension.hpp"

#include "cla/model/core/CoreSpatialPooler.hpp"

namespace cla {

/**
 * HtmSpatialPooler implementation in C++.
 *
 * @b Description
 * The HtmSpatialPooler is extended class of CoreSpatialPooler.
 * The HtmSpatialPooler is a class that implements a general HTM spatial
 * pooler behavior. It basically has the htm::SpatialPoolerExtension
 * functions.
 */
class HtmSpatialPooler : public CoreSpatialPooler {

private:

	htm::SpatialPoolerExtension sp_;

public:

	/**
	 * HtmSpatialPooler constructor.
	 */
	HtmSpatialPooler() = default;

	/**
	 * HtmSpatialPooler constructor with the parameters.
	 *
	 * @param inputDimensions
	 * A list of integers representing the dimensions of the input vector.
	 * Format is [height, width, depth, ...], where each value represents
	 * the size of the dimension. For a topology of one dimesion with 100
	 * inputs use [100]. For a two dimensional topology of 10x5 use [10,5].
	 *
	 * @param columnDimensions
	 * A list of integers representing the dimensions of the columns in the
	 * region. Format is [height, width, depth, ...], where each value
	 * represents the size of the dimension. For a topology of one dimesion
	 * with 2000 columns use 2000, or [2000]. For a three dimensional
	 * topology of 32x64x16 use [32, 64, 16].
	 *
	 * @param potentialRadius
	 * This parameter deteremines the extent of the
	 * input that each column can potentially be connected to. This
	 * can be thought of as the input bits that are visible to each
	 * column, or a 'receptive field' of the field of vision. A large
	 * enough value will result in global coverage, meaning
	 * that each column can potentially be connected to every input
	 * bit. This parameter defines a square (or hyper square) area: a
	 * column will have a max square potential pool with sides of
	 * length `(2 * potentialRadius + 1)`, rounded to fit into each
	 * dimension.
	 *
	 * @param potentialPct
	 * The percent of the inputs, within a column's
	 * potential radius, that a column can be connected to. If set to
	 * 1, the column will be connected to every input within its
	 * potential radius. This parameter is used to give each column a
	 * unique potential pool when a large potentialRadius causes
	 * overlap between the columns. At initialization time we choose
	 * ((2*potentialRadius + 1)^(# inputDimensions) * potentialPct)
	 * input bits to comprise the column's potential pool.
	 *
	 * @param globalInhibition
	 * If true, then during inhibition phase the
	 * winning columns are selected as the most active columns from the
	 * region as a whole. Otherwise, the winning columns are selected
	 * with resepct to their local neighborhoods. Global inhibition
	 * boosts performance significantly but there is no topology at the
	 * output.
	 *
	 * @param localAreaDensity
	 * The desired density of active columns within
	 * a local inhibition area (the size of which is set by the
	 * internally calculated inhibitionRadius, which is in turn
	 * determined from the average size of the connected potential
	 * pools of all columns). The inhibition logic will insure that at
	 * most N columns remain ON within a local inhibition area, where
	 * N = localAreaDensity * (total number of columns in inhibition
	 * area)
	 * Default: 0.05 (5%)
	 *
	 * @param stimulusThreshold
	 * This is a number specifying the minimum
	 * number of synapses that must be active in order for a column to
	 * turn ON. The purpose of this is to prevent noisy input from
	 * activating columns.
	 *
	 * @param synPermInactiveDec
	 * The amount by which the permanence of an
	 * inactive synapse is decremented in each learning step.
	 *
	 * @param synPermActiveInc
	 * The amount by which the permanence of an
	 * active synapse is incremented in each round.
	 *
	 * @param synPermConnected
	 * The default connected threshold. Any synapse
	 * whose permanence value is above the connected threshold is
	 * a "connected synapse", meaning it can contribute to
	 * the cell's firing.
	 *
	 * @param synInitPermanence
	 * The amount by which the permanence of an synapse is initialized.
	 *
	 * @param minPctOverlapDutyCycles
	 * A number between 0 and 1.0, used to set
	 * a floor on how often a column should have at least
	 * stimulusThreshold active inputs. Periodically, each column looks
	 * at the overlap duty cycle of all other column within its
	 * inhibition radius and sets its own internal minimal acceptable
	 * duty cycle to: minPctDutyCycleBeforeInh * max(other columns'
	 * duty cycles). On each iteration, any column whose overlap duty
	 * cycle falls below this computed value will get all of its
	 * permanence values boosted up by synPermActiveInc. Raising all
	 * permanences in response to a sub-par duty cycle before
	 * inhibition allows a cell to search for new inputs when either
	 * its previously learned inputs are no longer ever active, or when
	 * the vast majority of them have been "hijacked" by other columns.
	 *
	 * @param dutyCyclePeriod
	 * The period used to calculate duty cycles.
	 * Higher values make it take longer to respond to changes in
	 * boost. Shorter values make it potentially more unstable and
	 * likely to oscillate.
	 *
	 * @param boostStrength
	 * A number greater or equal than 0, used to
	 * control boosting strength. No boosting is applied if it is set to
	 * 0.0, (runs faster due to skipped code). The strength of boosting
	 * increases as a function of boostStrength. Boosting encourages
	 * columns to have similar activeDutyCycles as their neighbors, which
	 * will lead to more efficient use of columns. However, too much
	 * boosting may also lead to instability of SP outputs.
	 *
	 * @param seed
	 * Seed for our random number generator. If seed is < 0
	 * a randomly generated seed is used. The behavior of the spatial
	 * pooler is deterministic once the seed is set.
	 *
	 * @param spVerbosity
	 * spVerbosity level: 0, 1, 2, or 3
	 *
	 * @param wrapAround
	 * Boolean value that determines whether or not inputs
	 * at the beginning and end of an input dimension are considered
	 * neighbors for the purpose of mapping inputs to columns.
	 *
	 * @param constSynInitPermanence
	 * Boolean value that determines whether or not synapse permanence
	 * is initialized by the const value(synInitPermanence).
	 */
	HtmSpatialPooler(
		const std::vector<htm::UInt>& inputDimensions,
		const std::vector<htm::UInt>& columnDimensions,
		const htm::UInt potentialRadius = 16u,
		const htm::Real potentialPct = 0.5f,
		const bool globalInhibition = true,
		const htm::Real localAreaDensity = 0.05f, // 5%
		const htm::UInt stimulusThreshold = 0u,
		const htm::Real synPermInactiveDec = 0.01f,
		const htm::Real synPermActiveInc = 0.05f,
		const htm::Real synPermConnected = 0.1f,
		const htm::Real synInitPermanence = 0.5f,
		const htm::Real minPctOverlapDutyCycles = 0.001f,
		const htm::UInt dutyCyclePeriod = 1000u,
		const htm::Real boostStrength = 0.0f,
		const htm::Int seed = 1,
		const htm::UInt spVerbosity = 0u,
		const bool wrapAround = true,
		const bool constSynInitPermanence = false
	);

	/**
	 * HtmSpatialPooler destructor.
	 */
	~HtmSpatialPooler() = default;

	/**
	 * Initialize the HtmSpatialPooler with the parameters. For more detail
	 * of parameters. See the constructor.
	 */
	void initialize(
		const std::vector<htm::UInt>& inputDimensions,
		const std::vector<htm::UInt>& columnDimensions,
		const htm::UInt potentialRadius = 16u,
		const htm::Real potentialPct = 0.5f,
		const bool globalInhibition = true,
		const htm::Real localAreaDensity = 0.05f, // 5%
		const htm::UInt stimulusThreshold = 0u,
		const htm::Real synPermInactiveDec = 0.01f,
		const htm::Real synPermActiveInc = 0.05f,
		const htm::Real synPermConnected = 0.1f,
		const htm::Real synInitPermanence = 0.5f,
		const htm::Real minPctOverlapDutyCycles = 0.001f,
		const htm::UInt dutyCyclePeriod = 1000u,
		const htm::Real boostStrength = 0.0f,
		const htm::Int seed = 1,
		const htm::UInt spVerbosity = 0u,
		const bool wrapAround = true,
		const bool constSynInitPermanence = false
	);

	/**
	 * reset the internal state of the spatial pooler. The function is
	 * assumed to be executed at every time step.
	 */
	void restate() override {}

	/**
	 * Reset internal state of the spatial pooler.
	 */
	void reset() override {}

	/**
	 * Summarize the spatial pooler.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the spatial pooler.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

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
	void compute(
		const htm::SDR& activeBits,
		const bool learn,
		htm::SDR& activeColumns
	) override;

	/**
	 * Map a column to bits. This function collects bits with the connected
	 * synapses.
	 *
	 * @param column The index of the column.
	 * @return const std::vector<htm::CellIdx> The indexes of bits.
	 */
	const std::vector<htm::CellIdx> bitsForColumn(
		const htm::CellIdx column
	) const override;

	/**
	 * Get the column-synapses of the spatial pooler.
	 *
	 * @return const htm::Connections& the column-synapses.
	 */
	const htm::Connections& getConnections() const override;
};

} // namespace cla

#endif // HTM_SPATIAL_POOLERA_HPP