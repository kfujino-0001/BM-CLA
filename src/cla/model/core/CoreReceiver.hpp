// CoreReceiver.hpp

/**
 * @file
 * Definitions for the CoreReceiver class in C++
 */

#ifndef CORE_RECEIVER_HPP
#define CORE_RECEIVER_HPP

#include <iostream>
#include <memory>

#include "htm/types/Sdr.hpp"
#include "cla/utils/Status.hpp"
#include "cla/model/module/helper/LayerProxy.hpp"

namespace cla {

/**
 * CoreReceiver implementation in C++.
 *
 * @b Description
 * This class is an abstract class for defining functions that select the
 * output sdrs for the lower layer. This class is based on the strategy
 * design pattern.
 */
class CoreReceiver {

public:

	/**
	 * CoreReceiver constructor.
	 */
	CoreReceiver() = default;

	/**
	 * CoreReceiver destructor.
	 */
	virtual ~CoreReceiver() = default;

	/**
	 * reset the internal state of the selector. The function is assumed to
	 * be executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the selector.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the selector model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the selector model.
	 * 
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

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
	virtual void receive(
		const PLayerProxy& upperLayer,
		htm::SDR& activeSDR,
		htm::SDR& winnerSDR
	) = 0;
};

using PReceiver = std::unique_ptr<CoreReceiver>;


} // namespace cla

#endif // CORE_RECEIVER_HPP