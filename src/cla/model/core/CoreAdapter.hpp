// CoreAdapter.hpp

/**
 * @file
 * Definitions for the CoreAdapter class in C++
 */

#ifndef CORE_ADAPTER_HPP
#define CORE_ADAPTER_HPP

#include <iostream>
#include <memory>

#include "htm/types/Sdr.hpp"

namespace cla {

/**
 * CoreAdapter implementation in C++.
 *
 * @b Description
 * This class is an abstract class for defining functions that adapt the
 * input for the layer. This class is based on the strategy design pattern.
 */
class CoreAdapter {

public:

	/**
	 * CoreAdapter constructor.
	 */
	CoreAdapter() = default;

	/**
	 * CoreAdapter destructor.
	 */
	virtual ~CoreAdapter() = default;

	/**
	 * reset the internal state of the adapter. The function is assumed to
	 * be executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the adapter.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the adapter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the adapter model.
	 * 
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Adapt the input sdr to an active bits.
	 *
	 * @param inputSDR The input sdr from the lower layer.
	 * @param activeBits The active Bits. (This param has a return value.)
	 */
	virtual void adapt(
		const htm::SDR& inputSDR,
		htm::SDR& activeBits
	) const = 0;
};

using PAdapter = std::unique_ptr<CoreAdapter>;


} // namespace cla

#endif // CORE_ADAPTER_HPP