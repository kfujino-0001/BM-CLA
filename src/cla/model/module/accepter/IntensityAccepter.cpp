// IntensityAccepter.cpp

/**
 * @file
 * Implementation of IntensityAccepter.cpp
 */

#include "cla/model/module/accepter/IntensityAccepter.hpp"

namespace cla {

/************************************************
 * IntensityAccepter public functions.
 ***********************************************/

IntensityAccepter::IntensityAccepter(const htm::UInt intensityThreshold) {
	initialize(intensityThreshold);
}

void IntensityAccepter::initialize(const htm::UInt intensityThreshold) {
	intensityThreshold_ = intensityThreshold;
}

void IntensityAccepter::summary(std::ostream& os) const {
	os << "IntensityAccepter";
}

void IntensityAccepter::detail(std::ostream& os) const {
	os << " -- IntensityAccepter ------------------------------------"
	   << std::endl
	   << std::endl;

	os << "\tintensityThreshold\t\t= " << intensityThreshold_ << "u"
	   << std::endl;
	os << std::endl;
}

const bool IntensityAccepter::isAccept(const htm::SDR& inputSDR) const {
	return static_cast<htm::UInt>(inputSDR.getSparse().size())
		>= intensityThreshold_;
}

} // namespace cla