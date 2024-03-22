// ActiveColumnSender.cpp

/**
 * @file
 * Implementation of ActiveColumnSender.cpp
 */

#include "cla/model/module/sender/ActiveColumnSender.hpp"

namespace cla {

/************************************************
 * ActiveColumnSender public functions.
 ***********************************************/

void ActiveColumnSender::summary(std::ostream& os) const {
	os << "ActiveColumnSender";
}

void ActiveColumnSender::detail(std::ostream& os) const {
	os << " -- ActiveColumnSender -----------------------------------"
	   << std::endl
	   << std::endl;
}

void ActiveColumnSender::send(
	const PLayerProxy& layer,
	htm::SDR& activeSDR
) const {
	activeSDR = layer->getActiveColumns();
}

} // namespace cla