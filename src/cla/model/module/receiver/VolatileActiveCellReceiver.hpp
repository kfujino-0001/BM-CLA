// VolatileActiveCellReceiver.hpp

/**
 * @file
 * Definitions for the VolatileActiveCellReceiver class in C++
 */

#ifndef VOLATILE_ACTIVE_CELL_RECEIVER_HPP
#define VOLATILE_ACTIVE_CELL_RECEIVER_HPP

#include "cla/model/core/CoreReceiver.hpp"

namespace cla {

/**
 * VolatileActiveCellReceiver implementation in C++.
 *
 * @b Description
 * The VolatileActiveCellReceiver is extended class of CoreReceiver.
 * The VolatileActiveCellReceiver is a class that receives the active cell
 * from the upper layer and volatilizes the active cell.
 */
class VolatileActiveCellReceiver : public CoreReceiver {

private:

	inline static const htm::Real _initVolatile = 1.0f;

	htm::Real volatileRate_;
	htm::Real volatileThreshold_;

	std::vector<htm::Real> volatileActiveDense_;
	std::vector<htm::Real> volatileWinnerDense_;

private:

	void update_(
		const htm::SDR& additive,
		std::vector<htm::Real>& volatileDense
	);

	void volatilize_(std::vector<htm::Real>& volatileDense);

	const htm::SDR_sparse_t convertSparse_(
		const std::vector<htm::Real>& volatileDense
	) const;

public:

	/**
	 * VolatileActiveCellReceiver constructor.
	 */
	VolatileActiveCellReceiver() = default;

	/**
	 * VolatileActiveCellReceiver constructor with the parameters.
	 *
	 * @param externalPredictiveInputs
	 * Number of external predictive inputs.  These values are not related
	 * to this TM, they represent input from a different region.  This TM
	 * will form synapses with these inputs in addition to the cells which
	 * are part of this TemporalMemoryExtension.  If this is given (and
	 * greater than 0) then the active cells and winner cells of these
	 * external inputs must be given to methods TM.compute and
	 * TM.activateDendrites
	 * @param volatileRate The volatile rate used each step.
	 * @param volatileThreshold The volatile threshold to determine if
	 * completely volatile.
	 */
	VolatileActiveCellReceiver(
		const htm::UInt externalPredictiveInputs,
		const htm::Real volatileRate = 0.5f,
		const htm::Real volatileThreshold = 0.2f
	);

	/**
	 * VolatileActiveCellReceiver destructor.
	 */
	~VolatileActiveCellReceiver() = default;

	/**
	 * Initialize the VolatileActiveCellReceiver with the parameters.
	 *
	 * @param externalPredictiveInputs
	 * Number of external predictive inputs.  These values are not related
	 * to this TM, they represent input from a different region.  This TM
	 * will form synapses with these inputs in addition to the cells which
	 * are part of this TemporalMemoryExtension.  If this is given (and
	 * greater than 0) then the active cells and winner cells of these
	 * external inputs must be given to methods TM.compute and
	 * TM.activateDendrites
	 * @param volatileRate The volatile rate used each step.
	 * @param volatileThreshold The volatile threshold to determine if
	 * completely volatile.
	 */
	void initialize(
		const htm::UInt externalPredictiveInputs,
		const htm::Real volatileRate = 0.5f,
		const htm::Real volatileThreshold = 0.2f
	);

	/**
	 * reset the internal state of the selector. The function is assumed to
	 * be executed at every time step.
	 */
	void restate() override {}

	/**
	 * Reset internal state of the selector.
	 */
	void reset() override {}

	/**
	 * Summarize the selector model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the selector model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

	/**
	 * Receive the sdrs from the proxy of the layer.
	 *
	 * @param upperLayer The proxy of the upper layer. This function receives a kind of sdr
	 * from this proxy.
	 * @param activeSDR The active sdr in this layer to activate the lower
	 * layer.
	 * @param winnerSDR The winner sdr in this layer to activate the lower
	 * layer.
	 */
	void receive(
		const PLayerProxy& upperLayer,
		htm::SDR& activeSDR,
		htm::SDR& winnerSDR
	) override;
	
};

} // namespace cla

#endif // VOLATILE_ACTIVE_CELL_RECEIVER_HPP