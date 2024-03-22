// HtmTemporalMemory.cpp

/**
 * @file
 * Implementation of HtmTemporalMemory.cpp
 */

#include "cla/model/module/tm/HtmTemporalMemory.hpp"
#include "cla/extension/algorithms/SegmentSelector.hpp"

#include "cla/utils/VectorHelpers.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

/************************************************
 * HtmTemporalMemory public functions.
 ***********************************************/

HtmTemporalMemory::HtmTemporalMemory(
	const htm::UInt numRegions,
	const std::vector<htm::CellIdx>& columnDimensions,
	const htm::CellIdx cellsPerColumn,
	const htm::SynapseIdx activationThreshold,
	const htm::Permanence initialPermanence,
	const htm::Permanence connectedPermanence,
	const htm::SynapseIdx minThreshold,
	const htm::SynapseIdx maxNewSynapseCount,
	const htm::Permanence permanenceIncrement,
	const htm::Permanence permanenceDecrement,
	const htm::Permanence predictedSegmentDecrement,
	const htm::Int seed,
	const htm::SegmentIdx maxSegmentsPerCell,
	const htm::SynapseIdx maxSynapsesPerSegment,
	const bool checkInputs,
	const bool exceptionHandling,
	const htm::UInt externalPredictiveInputs,
	const htm::Real synapseDestinationWeight,
	const htm::Real createSynWeight,
	const htm::Real destroySynWeight,
	const htm::Real activateWeight,
	const htm::UInt capacityOfNbActiveSegments,
	const htm::UInt capacityOfNbMatchingSegments,
	const htm::SSMode innerSegmentSelectorMode,
	const htm::SSMode outerSegmentSelectorMode,
	const htm::TemporalMemoryExtension::ANMode anomalyMode
) {
	initialize(
		numRegions, columnDimensions, cellsPerColumn, activationThreshold,
		initialPermanence, connectedPermanence, minThreshold,
		maxNewSynapseCount, permanenceIncrement, permanenceDecrement,
		predictedSegmentDecrement, seed, maxSegmentsPerCell,
		maxSynapsesPerSegment, checkInputs, exceptionHandling,
		externalPredictiveInputs, synapseDestinationWeight,
		createSynWeight, destroySynWeight, activateWeight,
		capacityOfNbActiveSegments, capacityOfNbMatchingSegments,
		innerSegmentSelectorMode, outerSegmentSelectorMode,
		anomalyMode
	);
}

void HtmTemporalMemory::initialize(
	const htm::UInt numRegions,
	const std::vector<htm::CellIdx>& columnDimensions,
	const htm::CellIdx cellsPerColumn,
	const htm::SynapseIdx activationThreshold,
	const htm::Permanence initialPermanence,
	const htm::Permanence connectedPermanence,
	const htm::SynapseIdx minThreshold,
	const htm::SynapseIdx maxNewSynapseCount,
	const htm::Permanence permanenceIncrement,
	const htm::Permanence permanenceDecrement,
	const htm::Permanence predictedSegmentDecrement,
	const htm::Int seed,
	const htm::SegmentIdx maxSegmentsPerCell,
	const htm::SynapseIdx maxSynapsesPerSegment,
	const bool checkInputs,
	const bool exceptionHandling,
	const htm::UInt externalPredictiveInputs,
	const htm::Real synapseDestinationWeight,
	const htm::Real createSynWeight,
	const htm::Real destroySynWeight,
	const htm::Real activateWeight,
	const htm::UInt capacityOfNbActiveSegments,
	const htm::UInt capacityOfNbMatchingSegments,
	const htm::SSMode innerSegmentSelectorMode,
	const htm::SSMode outerSegmentSelectorMode,
	const htm::TemporalMemoryExtension::ANMode anomalyMode
) {
	tm_.initialize(
		numRegions, columnDimensions, cellsPerColumn, activationThreshold,
		initialPermanence, connectedPermanence, minThreshold,
		maxNewSynapseCount, permanenceIncrement, permanenceDecrement,
		predictedSegmentDecrement, seed, maxSegmentsPerCell,
		maxSynapsesPerSegment, checkInputs, exceptionHandling,
		externalPredictiveInputs, synapseDestinationWeight,
		createSynWeight, destroySynWeight, activateWeight,
		capacityOfNbActiveSegments, capacityOfNbMatchingSegments,
		innerSegmentSelectorMode, outerSegmentSelectorMode,
		anomalyMode
	);
}

void HtmTemporalMemory::reset() {
	tm_.reset();
}

void HtmTemporalMemory::summary(std::ostream& os) const {
	os << "HtmTemporalMemory";
}

void HtmTemporalMemory::detail(std::ostream& os) const {
	
	using ANMode = htm::TemporalMemoryExtension::ANMode;

	std::string ANModeStr;

	switch (tm_.getAnomalyMode()) {
		case ANMode::DISABLED: ANModeStr = "DISABLED"; break;
		case ANMode::RAW: ANModeStr = "RAW"; break;
		case ANMode::LIKELIHOOD: ANModeStr = "LIKELIHOOD"; break;
		case ANMode::LOGLIKELIHOOD: ANModeStr = "LOGLIKELIHOOD"; break;
		default: break;
	}
	
	os << " -- HtmTemporalMemory ------------------------------------"
	   << std::endl
	   << std::endl;

	os << "\tactivationThreshold\t\t= " << tm_.getActivationThreshold() << "u" << std::endl;
	os << "\tinitialPermanence\t\t= " << tm_.getInitialPermanence() << "f" << std::endl;
	os << "\tconnectedPermanence\t\t= " << tm_.getConnectedPermanence() << "f" << std::endl;
	os << "\tminThreshold\t\t\t= " << tm_.getMinThreshold() << "u" << std::endl;
	os << "\tmaxNewSynapseCount\t\t= " << tm_.getMaxNewSynapseCount() << "u" << std::endl;
	os << "\tpermanenceIncrement\t\t= " << tm_.getPermanenceIncrement() << "f" << std::endl;
	os << "\tpermanenceDecrement\t\t= " << tm_.getPermanenceDecrement() << "f" << std::endl;
	os << "\tpredictedSegmentDecrement\t= " << tm_.getPredictedSegmentDecrement() << "f" << std::endl;
	os << "\tmaxSegmentsPerCell\t\t= " << tm_.getMaxSegmentsPerCell() << "u" << std::endl;
	os << "\tmaxSynapsesPerSegment\t\t= " << tm_.getMaxSynapsesPerSegment() << "u" << std::endl;
	os << "\tcheckInputs\t\t\t= " << ((tm_.getCheckInputs()) ? "true" : "false") << std::endl;
	os << "\texceptionHandling\t\t= " << ((tm_.getExceptionHandling()) ? "true" : "false") << std::endl;
	os << "\texternalPredictiveInputs\t= " << tm_.getExternalPredictiveInputs() << "u" << std::endl;
	os << "\tsynapseDestinationWeight\t= " << tm_.getSynapseDestinationWeight() << std::endl;
	os << "\tcreateSynWeight\t\t\t= " << tm_.getCreateSynWeight() << "f" << std::endl;
	os << "\tdestroySynWeight\t\t= " << tm_.getDestroySynWeight() << "f" << std::endl;
	os << "\tactivateWeight\t\t\t= " << tm_.getActivateWeight() << "f" << std::endl;
	os << "\tactivateSegmentsCapacity\t= " << tm_.getActivateSegmentsCapacity() << "u" << std::endl;
	os << "\tmatchingSegmentsCapacity\t= " << tm_.getMatchingSegmentsCapacity() << "u" << std::endl;
	os << "\tinnerSegmentSelectorMode\t= SSMode::" << htm::SegmentSelectors::getName(tm_.getInnerSegmentSelectorMode()) << std::endl;
	os << "\touterSegmentSelectorMode\t= SSMode::" << htm::SegmentSelectors::getName(tm_.getOuterSegmentSelectorMode()) << std::endl;
	os << "\tanomalyMode\t\t\t= ANMode::" << ANModeStr << std::endl;
	os << std::endl;

}

void HtmTemporalMemory::compute(
	const htm::SDR& activeColumns,
	const bool learn,
	htm::SDR& activeCells,
	htm::SDR& winnerCells
) {
	tm_.compute(activeColumns, learn);
	tm_.getActiveCells(activeCells);
	tm_.getWinnerCells(winnerCells);
}

void HtmTemporalMemory::activate(
	const bool learn, 
	htm::SDR_sparse_t& activeSegments
) {
	tm_.activateDendrites(learn);
	activeSegments = tm_.getActiveSegments();
}

void HtmTemporalMemory::activate(
	const htm::SDR& externalActiveSDR,
	const htm::SDR& externalWinnerSDR,
	const bool learn,
	htm::SDR_sparse_t& activeSegments
) {
	tm_.activateDendrites(learn, externalActiveSDR, externalWinnerSDR);
	activeSegments = tm_.getActiveSegments();
}

htm::CellIdx HtmTemporalMemory::cellForSegment(const htm::Segment segment) const {
	return tm_.connections.cellForSegment(segment);
}

htm::CellIdx HtmTemporalMemory::columnForCell(const htm::CellIdx cell) const {
	return tm_.columnForCell(cell);
}

const htm::Connections& HtmTemporalMemory::getConnections() const {
	return tm_.connections;
}

const htm::TMConnectionsHandler& HtmTemporalMemory::getConnectionHandler() const {
	return tm_.getSynapseHandler();
}

const htm::Real HtmTemporalMemory::getAnomaly() const {
	return tm_.anomaly;
}

} // namespace cla