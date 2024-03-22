// FullAccepter.hpp

/**
 * @file
 * Definitions for the FullAccepter class in C++
 */

#ifndef FULL_ACCEPTER_HPP
#define FULL_ACCEPTER_HPP

#include "cla/model/core/CoreAccepter.hpp"

namespace cla {

/**
 * FullAccepter implementation in C++.
 *
 * @b Description
 * The FullAccepter is extended class of CoreAccepter.
 * The FullAccepter is a class that accepts all input sdrs without any
 * conditions.
 */
class FullAccepter : public CoreAccepter {

public:

	/**
	 * FullAccepter constructor.
	 */
	FullAccepter() = default;

	/**
	 * FullAccepter destructor.
	 */
	~FullAccepter() = default;

	/**
	 * reset the internal state of the accepter. The function is assumed to
	 * be executed at every time step.
	 */
	void restate() override {}

	/**
	 * Reset internal state of the accepter.
	 */
	void reset() override {}

	/**
	 * Summarize the accepter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the accepter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

	/**
	 * Check whether an input sdr is accepted.
	 *
	 * @param inputSDR an input sdr
	 * @return true This value means that the input sdr is accepted.
	 * @return false This value means that the input sdr is rejected.
	 */
	const bool isAccept(const htm::SDR& inputSDR) const override;
};

} // namespace cla

#endif // FULL_ACCEPTER_HPP