// CoreAccepter.hpp

/**
 * @file
 * Definitions for the CoreAccepter class in C++
 */

#ifndef CORE_ACCEPTER_HPP
#define CORE_ACCEPTER_HPP

#include <iostream>
#include <memory>

#include "htm/types/Sdr.hpp"

namespace cla {

/**
 * CoreAccepter implementation in C++.
 *
 * @b Description
 * This class is an abstract class for defining functions that check
 * whether an input is accepted. This class is based on the strategy design
 * pattern.
 */
class CoreAccepter {

public:

	/**
	 * CoreAccepter constructor.
	 */
	CoreAccepter() = default;

	/**
	 * CoreAccepter destructor.
	 */
	virtual ~CoreAccepter() = default;

	/**
	 * reset the internal state of the accepter. The function is assumed to
	 * be executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the accepter.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the accepter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the accepter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Check whether an input sdr is accepted.
	 *
	 * @param inputSDR an input sdr
	 * @return true This value means that the input sdr is accepted.
	 * @return false This value means that the input sdr is rejected.
	 */
	virtual const bool isAccept(const htm::SDR& inputSDR) const = 0;
};

using PAccepter = std::unique_ptr<CoreAccepter>;


} // namespace cla

#endif // CORE_ACCEPTER_HPP