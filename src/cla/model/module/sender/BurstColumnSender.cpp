// BurstColumnSender.cpp

/** 
 * @file
 * Implementation of BurstColumnSender.cpp
 */

#include "cla/model/module/sender/BurstColumnSender.hpp"

namespace cla {

/************************************************
 * BurstColumnSender public functions.
 ***********************************************/

void BurstColumnSender::summary(std::ostream& os) const {
	os << "BurstColumnSender";
}

void BurstColumnSender::detail(std::ostream& os) const {
	os << " -- BurstColumnSender ------------------------------------"
	   << std::endl
	   << std::endl;
}

void BurstColumnSender::send(
	const PLayerProxy& layer,
	htm::SDR& activeSDR
) const {
	activeSDR = layer->getBurstColumns();
}

} // namespace cla