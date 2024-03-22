// ActiveColumnSender.hpp

/**
 * @file
 * Definitions for the ActiveColumnSender class in C++
 */

#ifndef ACTIVE_COLUMN_SENDER_HPP
#define ACTIVE_COLUMN_SENDER_HPP

#include "cla/model/core/CoreSender.hpp"

namespace cla {

/**
 * ActiveColumnSender implementation in C++.
 *
 * @b Description
 * The ActiveColumnSender is extended class of CoreSender.
 * The ActiveColumnSender is a class that sends the active columns on this
 * layer to the upper layer.
 */
class ActiveColumnSender : public CoreSender {

public:

	/**
	 * ActiveColumnSender constructor.
	 */
	ActiveColumnSender() = default;

	/**
	 * ActiveColumnSender destructor.
	 */
	~ActiveColumnSender() = default;

	/**
	 * reset the internal state of the switcher. The function is assumed to
	 * be executed at every time step.
	 */
	void restate() override {}

	/**
	 * Reset internal state of the switcher.
	 */
	void reset() override {}

	/**
	 * Summarize the switcher model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the switcher.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

	/**
	 * Send the active sdr.
	 *
	 * @param sdrs The proxy of this layer.
	 * @param actives The switched activeSDR from the layer.
	 */
	void send(
		const PLayerProxy& layer,
		htm::SDR& activeSDR
	) const override;
};

} // namespace cla

#endif // ACTIVE_COLUMN_SENDER_HPP