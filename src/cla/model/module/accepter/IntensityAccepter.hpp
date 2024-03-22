// IntensityAccepter.hpp

/**
 * @file
 * Definitions for the IntensityAccepter class in C++
 */

#ifndef INTENSITY_ACCEPTER_HPP
#define INTENSITY_ACCEPTER_HPP

#include "cla/model/core/CoreAccepter.hpp"

namespace cla {

/**
 * IntensityAccepter implementation in C++.
 *
 * @b Description
 * The IntensityAccepter is extended class of CoreAccepter.
 * The IntensityAccepter is a class that accepts input sdrs having a mount
 * of actives more than the threshold.
 */
class IntensityAccepter : public CoreAccepter {

private:

	htm::UInt intensityThreshold_;

public:

	/**
	 * IntensityAccepter constructor with the parameters.
	 *
	 * @param intensityThreshold The threshold value whether or not
	 * determined to accept input sdr by active intensity.
	 */
	IntensityAccepter(const htm::UInt intensityThreshold = 25u);

	/**
	 * IntensityAccepter destructor.
	 */
	~IntensityAccepter() = default;

	/**
	 * Initialize the IntensityAccepter with the parameters.
	 *
	 * @param intensityThreshold The threshold value whether or not
	 * determined to accept input sdr by active intensity.
	 */
	void initialize(const htm::UInt intensityThreshold = 25u);

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

#endif // INTENSITY_ACCEPTER_HPP