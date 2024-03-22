// ActiveCellReceiver.cpp

/**
 * @file
 * Implementation of ActiveCellReceiver.cpp
 */

#include "cla/model/module/receiver/ActiveCellReceiver.hpp"
#include "cla/utils/SdrHelpers.hpp"

namespace cla {

/************************************************
 * ActiveCellReceiver public functions.
 ***********************************************/

void ActiveCellReceiver::summary(std::ostream& os) const {
	os << "ActiveCellReceiver";
}

void ActiveCellReceiver::detail(std::ostream& os) const {
	os << " -- ActiveCellReceiver -----------------------------------"
	   << std::endl
	   << std::endl;
}

void ActiveCellReceiver::receive(
	const PLayerProxy& upperLayer,
	htm::SDR& activeSDR,
	htm::SDR& winnerSDR
) {
	if(upperLayer->getStatus() == Status::RUN) {
		copy(upperLayer->getActiveCells(), activeSDR_);
		copy(upperLayer->getWinnerCells(), winnerSDR_);
	}

	copy(activeSDR_, activeSDR);
	copy(winnerSDR_, winnerSDR);
}

} // namespace cla