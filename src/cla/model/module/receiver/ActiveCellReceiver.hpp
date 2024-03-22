// ActiveCellReceiver.hpp

/**
 * @file
 * Definitions for the ActiveCellReceiver class in C++
 */

#ifndef ACTIVE_CELL_RECEIVER_HPP
#define ACTIVE_CELL_RECEIVER_HPP

#include "cla/model/core/CoreReceiver.hpp"

namespace cla {

/**
 * ActiveCellReceiver implementation in C++.
 *
 * @b Description
 * The ActiveCellReceiver is extended class of CoreReceiver.
 * The ActiveCellReceiver is a class that receives the active cell from the
 * upper layer.
 */
class ActiveCellReceiver : public CoreReceiver {

private:

	htm::SDR activeSDR_;
	htm::SDR winnerSDR_;

public:

	/**
	 * ActiveCellReceiver constructor.
	 */
	ActiveCellReceiver() = default;

	/**
	 * ActiveCellReceiver destructor.
	 */
	~ActiveCellReceiver() = default;

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

#endif // ACTIVE_CELL_RECEIVER_HPP