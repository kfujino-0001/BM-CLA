// VolatileActiveCellReceiver.cpp

/**
 * @file
 * Implementation of VolatileActiveCellReceiver.cpp
 */

#include "cla/model/module/receiver/VolatileActiveCellReceiver.hpp"

#include "cla/utils/Checker.hpp"

namespace cla {

/************************************************
 * VolatileActiveCellReceiver private functions.
 ***********************************************/

void VolatileActiveCellReceiver::update_(
	const htm::SDR& additive,
	std::vector<htm::Real>& volatileDense
) {
	for(const htm::ElemSparse idx : additive.getSparse()) {
		volatileDense.at(idx) = _initVolatile;
	}
}

void VolatileActiveCellReceiver::volatilize_(
	std::vector<htm::Real>& volatileDense
) {
	const auto& volatilizeElem
		= [&](htm::Real& elem) { elem *= volatileRate_; };

	std::for_each(
		volatileDense.begin(), volatileDense.end(), volatilizeElem
	);
}

const htm::SDR_sparse_t VolatileActiveCellReceiver::convertSparse_(
	const std::vector<htm::Real>& volatileDense
) const {
	htm::SDR_sparse_t sparse;

	for(std::size_t i = 0u, size = volatileDense.size(); i < size; ++i) {
		if(volatileDense.at(i) >= volatileThreshold_) {
			sparse.emplace_back(static_cast<htm::ElemSparse>(i));
		}
	}

	return sparse;
}

/************************************************
 * VolatileActiveCellReceiver public functions.
 ***********************************************/

VolatileActiveCellReceiver::VolatileActiveCellReceiver(
	const htm::UInt externalPredictiveInputs,
	const htm::Real volatileRate,
	const htm::Real volatileThreshold
) {
	initialize(externalPredictiveInputs, volatileRate, volatileThreshold);
}

void VolatileActiveCellReceiver::initialize(
	const htm::UInt externalPredictiveInputs,
	const htm::Real volatileRate,
	const htm::Real volatileThreshold
) {
	CLA_ASSERT(externalPredictiveInputs >= 0u);
	CLA_ASSERT(0.0f <= volatileRate && volatileRate <= 1.0f);
	CLA_ASSERT(0.0f <= volatileThreshold && volatileThreshold <= 1.0f);

	volatileRate_ = volatileRate;
	volatileThreshold_ = volatileThreshold;

	volatileActiveDense_.clear();
	volatileWinnerDense_.clear();
	volatileActiveDense_.resize(externalPredictiveInputs);
	volatileWinnerDense_.resize(externalPredictiveInputs);
	std::fill(volatileActiveDense_.begin(), volatileActiveDense_.end(), 0.0f);
	std::fill(volatileWinnerDense_.begin(), volatileWinnerDense_.end(), 0.0f);
}

void VolatileActiveCellReceiver::summary(std::ostream& os) const {
	os << "VolatileActiveCellReceiver";
}

void VolatileActiveCellReceiver::detail(std::ostream& os) const {
	os << " -- VolatileActiveCellReceiver ---------------------------"
	   << std::endl
	   << std::endl;

	os << "\tvolatileRate\t\t\t= " << volatileRate_ << "f" << std::endl;
	os << "\tvolatileThreshold\t\t= " << volatileThreshold_ << "f"
	   << std::endl;
	os << std::endl;
}

void VolatileActiveCellReceiver::receive(
	const PLayerProxy& upperLayer,
	htm::SDR& activeSDR,
	htm::SDR& winnerSDR
) {
	const auto& activeCells = upperLayer->getActiveCells();
	const auto& winnerCells = upperLayer->getWinnerCells();

	CLA_ASSERT(
		activeCells.size
		== static_cast<htm::UInt>(volatileActiveDense_.size())
	);
	CLA_ASSERT(
		winnerCells.size
		== static_cast<htm::UInt>(volatileWinnerDense_.size())
	);

	volatilize_(volatileActiveDense_);
	volatilize_(volatileWinnerDense_);

	if(upperLayer->getStatus() == Status::RUN) {
		update_(activeCells, volatileActiveDense_);
		update_(winnerCells, volatileWinnerDense_);
	}

	activeSDR.initialize(activeCells.dimensions);
	winnerSDR.initialize(winnerCells.dimensions);
	activeSDR.setSparse(convertSparse_(volatileActiveDense_));
	winnerSDR.setSparse(convertSparse_(volatileWinnerDense_));
}

} // namespace cla