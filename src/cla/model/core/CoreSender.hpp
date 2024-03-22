// CoreSender.hpp

/**
 * @file
 * Definitions for the CoreSender class in C++
 */

#ifndef CORE_SENDER_HPP
#define CORE_SENDER_HPP

#include <memory>

#include "htm/types/Sdr.hpp"

#include "cla/model/module/helper/LayerProxy.hpp"

namespace cla {

/**
 * CoreSender implementation in C++.
 *
 * @b Description
 * This class is an abstract class for defining functions that determine
 * the type of output. This class is based on the strategy design pattern.
 */
class CoreSender {

public:

	/**
	 * CoreSender constructor.
	 */
	CoreSender() = default;

	/**
	 * CoreSender destructor.
	 */
	virtual ~CoreSender() = default;

	/**
	 * reset the internal state of the switcher. The function is assumed to
	 * be executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the switcher.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the switcher model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the switcher.
	 * 
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Send the active sdr.
	 *
	 * @param sdrs The proxy of this layer.
	 * @param actives The switched activeSDR from the layer.
	 */
	virtual void send(
		const PLayerProxy& layer,
		htm::SDR& activeSDR
	) const = 0;
};

using PSender = std::unique_ptr<CoreSender>;


} // namespace cla

#endif // CORE_SENDER_HPP