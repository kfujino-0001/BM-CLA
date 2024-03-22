// BurstColumnSender.hpp

/** 
 * @file
 * Definitions for the BurstColumnSender class in C++
 */

#ifndef BURST_COLUMN_SENDER_HPP
#define BURST_COLUMN_SENDER_HPP

#include "cla/model/core/CoreSender.hpp"

namespace cla {

/**
 * BurstColumnSender implementation in C++.
 * 
 * @b Description
 * The BurstColumnSender is extended class of CoreSender.
 * The BurstColumnSender is a class that sends the burst columns on this
 * layer to the upper layer.
 */
class BurstColumnSender : public CoreSender {

public:

	/**
	 * BurstColumnSender constructor.
	 */
	BurstColumnSender() = default;

	/**
	 * BurstColumnSender destructor.
	 */
	~BurstColumnSender() = default;

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

#endif // BURST_COLUMN_SENDER_HPP