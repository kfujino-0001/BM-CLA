//TemporalMemoryExtension.cpp

#include <algorithm> //is_sorted
#include <climits>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <set>

#include <htm/utils/GroupBy.hpp>
#include <htm/algorithms/Anomaly.hpp>

#include "cla/extension/algorithms/SegmentSelector.hpp"
#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"


using namespace htm;


/**
 * TemporalMemoryExtensionParameters methods
 */

std::ostream& htm::operator<<(
	std::ostream& os, 
	const TemporalMemoryExtensionParameters& params
) {
	
	std::string ANModeStr;

	switch (params.anomalyMode) {
		case TemporalMemoryExtension::ANMode::DISABLED: ANModeStr = "DISABLED"; break;
		case TemporalMemoryExtension::ANMode::RAW: ANModeStr = "RAW"; break;
		case TemporalMemoryExtension::ANMode::LIKELIHOOD: ANModeStr = "LIKELIHOOD"; break;
		case TemporalMemoryExtension::ANMode::LOGLIKELIHOOD: ANModeStr = "LOGLIKELIHOOD"; break;
		default: break;
	}

	os << "\tcellsPerColumn\t\t\t= " << params.cellsPerColumn << std::endl;
	os << "\tactivationThreshold\t\t= " << params.activationThreshold << "u" << std::endl;
	os << "\tinitialPermanence\t\t= " << params.initialPermanence << "f" << std::endl;
	os << "\tconnectedPermanence\t\t= " << params.connectedPermanence << "f" << std::endl;
	os << "\tminThreshold\t\t\t= " << params.minThreshold << "u" << std::endl;
	os << "\tmaxNewSynapseCount\t\t= " << params.maxNewSynapseCount << "u" << std::endl;
	os << "\tpermanenceIncrement\t\t= " << params.permanenceIncrement << "f" << std::endl;
	os << "\tpermanenceDecrement\t\t= " << params.permanenceDecrement << "f" << std::endl;
	os << "\tpredictedSegmentDecrement\t= " << params.predictedSegmentDecrement << "f" << std::endl;
	os << "\tseed\t\t\t\t= " << params.seed << std::endl;
	os << "\tmaxSegmentsPerCell\t\t= " << params.maxSegmentsPerCell << "u" << std::endl;
	os << "\tmaxSynapsesPerSegment\t\t= " << params.maxSynapsesPerSegment << "u" << std::endl;
	os << "\tcheckInputs\t\t\t= " << ((params.checkInputs) ? "true" : "false") << std::endl;
	os << "\texceptionHandling\t\t= " << ((params.exceptionHandling) ? "true" : "false") << std::endl;
	os << "\texternalPredictiveInputs\t= " << params.externalPredictiveInputs << "u" << std::endl;
	os << "\tsynapseDestinationWeight\t= " << params.synapseDestinationWeight << std::endl;
	os << "\tcreateSynWeight\t\t\t= " << params.createSynWeight << "f" << std::endl;
	os << "\tdestroySynWeight\t\t= " << params.destroySynWeight << "f" << std::endl;
	os << "\tactivateWeight\t\t\t= " << params.activateWeight << "f" << std::endl;
	os << "\tactivateSegmentsCapacity\t= " << params.activateSegmentsCapacity << "u" << std::endl;
	os << "\tmatchingSegmentsCapacity\t= " << params.matchingSegmentsCapacity << "u" << std::endl;
	os << "\tinnerSegmentSelectorMode\t= SSMode::" << SegmentSelectors::getName(params.innerSegmentSelectorMode) << std::endl;
	os << "\touterSegmentSelectorMode\t= SSMode::" << SegmentSelectors::getName(params.outerSegmentSelectorMode) << std::endl;
	os << "\tanomalyMode\t\t\t= ANMode::" << ANModeStr << std::endl;
	os << std::endl;

	return os;
}

void htm::to_json(json& j, const TemporalMemoryExtensionParameters& p){
	j = json{
		{"cellsPerColumn", p.cellsPerColumn},
		{"activationThreshold", p.activationThreshold},
		{"initialPermanence", p.initialPermanence},
		{"connectedPermanence", p.connectedPermanence},
		{"minThreshold", p.minThreshold},
		{"maxNewSynapseCount", p.maxNewSynapseCount},
		{"permanenceIncrement", p.permanenceIncrement},
		{"permanenceDecrement", p.permanenceDecrement},
		{"predictedSegmentDecrement", p.predictedSegmentDecrement},
		{"maxSegmentsPerCell", p.maxSegmentsPerCell},
		{"maxSynapsesPerSegment", p.maxSynapsesPerSegment},
		{"checkInputs", p.checkInputs},
		{"exceptionHandling", p.exceptionHandling},
		{"createSynWeight", p.createSynWeight},
		{"destroySynWeight", p.destroySynWeight},
		{"activateWeight", p.activateWeight},
		{"activateSegmentsCapacity", p.activateSegmentsCapacity},
		{"matchingSegmentsCapacity", p.matchingSegmentsCapacity},
		{"innerSegmentSelectorMode", SegmentSelectors::getName(p.innerSegmentSelectorMode)},
		{"outerSegmentSelectorMode", SegmentSelectors::getName(p.outerSegmentSelectorMode)},
		{"anomalyMode", static_cast<int>(p.anomalyMode)},
	};
}

void htm::from_json(const json& j, TemporalMemoryExtensionParameters& p){
	int anomalyMode;
	std::string innerSegmentModeStr;
	std::string outerSegmentModeStr;
	
	j.at("cellsPerColumn").get_to(p.cellsPerColumn);
	j.at("activationThreshold").get_to(p.activationThreshold);
	j.at("initialPermanence").get_to(p.initialPermanence);
	j.at("connectedPermanence").get_to(p.connectedPermanence);
	j.at("minThreshold").get_to(p.minThreshold);
	j.at("maxNewSynapseCount").get_to(p.maxNewSynapseCount);
	j.at("permanenceIncrement").get_to(p.permanenceIncrement);
	j.at("permanenceDecrement").get_to(p.permanenceDecrement);
	j.at("predictedSegmentDecrement").get_to(p.predictedSegmentDecrement);
	j.at("maxSegmentsPerCell").get_to(p.maxSegmentsPerCell);
	j.at("maxSynapsesPerSegment").get_to(p.maxSynapsesPerSegment);
	j.at("checkInputs").get_to(p.checkInputs);
	j.at("exceptionHandling").get_to(p.exceptionHandling);
	j.at("createSynWeight").get_to(p.createSynWeight);
	j.at("destroySynWeight").get_to(p.destroySynWeight);
	j.at("activateWeight").get_to(p.activateWeight);
	j.at("activateSegmentsCapacity").get_to(p.activateSegmentsCapacity);
	j.at("matchingSegmentsCapacity").get_to(p.matchingSegmentsCapacity);
	j.at("innerSegmentSelectorMode").get_to(innerSegmentModeStr);
	j.at("outerSegmentSelectorMode").get_to(outerSegmentModeStr);
	j.at("anomalyMode").get_to(anomalyMode);

	p.anomalyMode 
		= static_cast<TemporalMemoryExtension::ANMode>(anomalyMode);
	p.innerSegmentSelectorMode = SegmentSelectors::getMode(innerSegmentModeStr);
	p.outerSegmentSelectorMode = SegmentSelectors::getMode(outerSegmentModeStr);

}



/**
 * SegmentDutyCycle methods
 */
SegmentDutyCycle::SegmentDutyCycle(const UInt32 cycle) {
	initialize(cycle);
}

void SegmentDutyCycle::initialize(const UInt32 cycle) {
	cycle_ = cycle;
	reset();
}

void SegmentDutyCycle::reset() {
	dutyCycle_.clear();
}

void SegmentDutyCycle::updateDutyCycle(
	const Segment segment,
	const bool correct
) {
	auto [period, duty] = dutyCycle_.at(segment);
	period = std::min(period + 1u, cycle_);

	Real decay = static_cast<Real>(period - 1u) / static_cast<Real>(period);
	duty *= decay;

	if(correct) duty += 1.0f / static_cast<Real>(period);

	dutyCycle_.at(segment) = std::make_pair(period, duty);
}

const bool SegmentDutyCycle::isExistSegment(const Segment segment) const {
	return dutyCycle_.count(segment) != 0u;
}

const std::pair<UInt32, Real>& SegmentDutyCycle::getDutyCycle(const Segment segment) const {
	return dutyCycle_.at(segment);
}

void SegmentDutyCycle::onCreateSegment(Segment segment) {
	dutyCycle_.insert(std::make_pair(segment, std::make_pair(1u, 1.0f)));
}

void SegmentDutyCycle::onDestroySegment(Segment segment) {
	dutyCycle_.erase(segment);
}



/**
 * TemporalMemoryConnectionsHandler methods.
 */
TemporalMemoryConnectionsHandler::TemporalMemoryConnectionsHandler(){
	initialize();
}

void TemporalMemoryConnectionsHandler::reset() {
	createdSegments_.clear();
	destroyedSegments_.clear();
	createdSynapses_.clear();
	destroyedSynapses_.clear();
	updatePermanences_.clear();
}

void TemporalMemoryConnectionsHandler::onCreateSegment(Segment segment) {
	createdSegments_.emplace_back(segment);
}

void TemporalMemoryConnectionsHandler::onDestroySegment(Segment segment) {
	destroyedSegments_.emplace_back(segment);
}

void TemporalMemoryConnectionsHandler::onCreateSynapse(Synapse synapse) {
	createdSynapses_.emplace_back(synapse);
}

void TemporalMemoryConnectionsHandler::onDestroySynapse(Synapse synapse) {
	destroyedSynapses_.emplace_back(synapse);
}

void TemporalMemoryConnectionsHandler::onUpdateSynapsePermanence(
	Synapse synapse,
	Permanence permanence
) {
	updatePermanences_.emplace_back(std::make_pair(synapse, permanence));
}

/**
 * TemporalMemoryExtension methods
 */

static const UInt TM_VERSION = 2;


TemporalMemoryExtension::TemporalMemoryExtension(
	const vector<CellIdx>& columnDimensions, 
	const CellIdx cellsPerColumn,
	const SynapseIdx activationThreshold, 
	const Permanence initialPermanence,
	const Permanence connectedPermanence, 
	const SynapseIdx minThreshold, 
	const SynapseIdx maxNewSynapseCount,
	const Permanence permanenceIncrement, 
	const Permanence permanenceDecrement,
	const Permanence predictedSegmentDecrement, 
	const Int seed, 
	const SegmentIdx maxSegmentsPerCell,
	const SynapseIdx maxSynapsesPerSegment, 
	const bool checkInputs, 
	const UInt externalPredictiveInputs,
	const ANMode anomalyMode
){

	initialize(
		columnDimensions, cellsPerColumn, activationThreshold,
		initialPermanence, connectedPermanence, minThreshold,
		maxNewSynapseCount, permanenceIncrement, permanenceDecrement,
		predictedSegmentDecrement, seed, maxSegmentsPerCell,
		maxSynapsesPerSegment, checkInputs, externalPredictiveInputs, anomalyMode
	);
}

TemporalMemoryExtension::TemporalMemoryExtension(const TMEParameters& params){
	initialize(params);
}

void TemporalMemoryExtension::initialize(
	const vector<CellIdx>& columnDimensions, 
	const CellIdx cellsPerColumn,
	const SynapseIdx activationThreshold, 
	const Permanence initialPermanence,
	const Permanence connectedPermanence, 
	const SynapseIdx minThreshold, 
	const SynapseIdx maxNewSynapseCount,
	const Permanence permanenceIncrement, 
	const Permanence permanenceDecrement,
	const Permanence predictedSegmentDecrement, 
	const Int seed, 
	const SegmentIdx maxSegmentsPerCell,
	const SynapseIdx maxSynapsesPerSegment, 
	const bool checkInputs, 
	const UInt externalPredictiveInputs,
	const ANMode anomalyMode
){

	// Validate all input parameters
	NTA_CHECK(columnDimensions.size() > 0) << "Number of column dimensions must be greater than 0";
	NTA_CHECK(cellsPerColumn > 0) << "Number of cells per column must be greater than 0";

	NTA_CHECK(initialPermanence >= 0.0 && initialPermanence <= 1.0);
	NTA_CHECK(connectedPermanence >= 0.0 && connectedPermanence <= 1.0);
	NTA_CHECK(permanenceIncrement >= 0.0 && permanenceIncrement <= 1.0);
	NTA_CHECK(permanenceDecrement >= 0.0 && permanenceDecrement <= 1.0);
	NTA_CHECK(minThreshold <= activationThreshold);

	// Save member variables

	numColumns_ = 1;
	columnDimensions_.clear();
	for (auto &columnDimension : columnDimensions) {
		numColumns_ *= columnDimension;
		columnDimensions_.push_back(columnDimension);
	}

	
	cellsPerColumn_ = cellsPerColumn; //TODO add checks
	activationThreshold_ = activationThreshold;
	initialPermanence_ = initialPermanence;
	connectedPermanence_ = connectedPermanence;
	minThreshold_ = minThreshold;
	maxNewSynapseCount_ = maxNewSynapseCount;
	checkInputs_ = checkInputs;
	permanenceIncrement_ = permanenceIncrement;
	permanenceDecrement_ = permanenceDecrement;
	predictedSegmentDecrement_ = predictedSegmentDecrement;
	externalPredictiveInputs_ = externalPredictiveInputs;

	// Initialize member variables
	connections_ = Connections(
		static_cast<CellIdx>(numberOfColumns() * cellsPerColumn_), 
		connectedPermanence_
	);
	rng_ = Random(seed);

	maxSegmentsPerCell_ = maxSegmentsPerCell;
	maxSynapsesPerSegment_ = maxSynapsesPerSegment;

	tmAnomaly_.mode_ = anomalyMode;

	reset();
}

void TemporalMemoryExtension::initialize(
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
	const htm::UInt activateSegmentsCapacity,
	const htm::UInt matchingSegmentsCapacity,
	const SSMode innerSegmentSelectorMode,
	const SSMode outerSegmentSelectorMode,
	const TemporalMemoryExtension::ANMode anomalyMode
){
	initialize(
		columnDimensions, cellsPerColumn, activationThreshold,
		initialPermanence, connectedPermanence, minThreshold,
		maxNewSynapseCount, permanenceIncrement, permanenceDecrement,
		predictedSegmentDecrement, seed, maxSegmentsPerCell,
		maxSynapsesPerSegment, checkInputs, externalPredictiveInputs, anomalyMode
	);

	synapseDestinationWeight_ 
		= (externalPredictiveInputs_ == 0u)? 1.0f : synapseDestinationWeight;

	exceptionHandling_ = exceptionHandling;

	createSynWeight_ = createSynWeight;
	destroySynWeight_ = destroySynWeight;
	activateWeight_ = activateWeight;

	activateSegmentsCapacity_ = activateSegmentsCapacity;
	matchingSegmentsCapacity_ = matchingSegmentsCapacity;
	innerSegmentSelectorMode_ = innerSegmentSelectorMode;
	outerSegmentSelectorMode_ = outerSegmentSelectorMode;

	innerSelector_ 
		= SegmentSelectors::createSelector(
			innerSegmentSelectorMode_,
			&connections_,
			static_cast<UInt>(numberOfCells()),
			numRegions
		);
	outerSelector_
		= SegmentSelectors::createSelector(
			outerSegmentSelectorMode_,
			&connections_,
			static_cast<UInt>(numberOfCells()),
			numRegions
		);

	handler_.initialize();
	segmentDutyCycle_.initialize(1000u);

	connections_.subscribe(&handler_);
	connections_.subscribe(&segmentDutyCycle_);
}

void TemporalMemoryExtension::initialize(const TMEParameters& params){
	initialize(
		params.numRegions,
		params.columnDimensions,
		params.cellsPerColumn,
		params.activationThreshold,
		params.initialPermanence,
		params.connectedPermanence,
		params.minThreshold,
		params.maxNewSynapseCount,
		params.permanenceIncrement,
		params.permanenceDecrement,
		params.predictedSegmentDecrement,
		params.seed,
		params.maxSegmentsPerCell,
		params.maxSynapsesPerSegment,
		params.checkInputs,
		params.exceptionHandling,
		params.externalPredictiveInputs,
		params.synapseDestinationWeight,
		params.createSynWeight,
		params.destroySynWeight,
		params.activateWeight,
		params.activateSegmentsCapacity,
		params.matchingSegmentsCapacity,
		params.innerSegmentSelectorMode,
		params.outerSegmentSelectorMode,
		params.anomalyMode
	);	
}

CellIdx TemporalMemoryExtension::getLeastUsedCell_(const CellIdx column){
	if(cellsPerColumn_ == 1) return column;

	vector<CellIdx> cells = cellsForColumn(column);

	//TODO: decide if we need to choose randomly from the "least used" cells, or if 1st is fine. 
	//In that case the line below is not needed, and this method can become const, deterministic results in tests need to be updated
	//un/comment line below: 
	rng_.shuffle(cells.begin(), cells.end()); //as min_element selects 1st minimal element, and we want to randomly choose 1 from the minimals.

	const auto compareByNumSegments 
		= [&](const CellIdx a, const CellIdx b) {
			if(connections.numSegments(a) == connections.numSegments(b)) 
			return a < b; //TODO rm? 
			else return connections.numSegments(a) < connections.numSegments(b);
		};
	
	return *std::min_element(cells.begin(), cells.end(), compareByNumSegments);
}


void TemporalMemoryExtension::growSynapses_(
	const Segment& segment,
	const SynapseIdx nDesiredNewSynapses,
	const vector<CellIdx> &prevWinnerCells
){
	
	vector<CellIdx> candidates(prevWinnerCells.begin(), prevWinnerCells.end());
	NTA_ASSERT(std::is_sorted(candidates.begin(), candidates.end()));

	//figure the number of new synapses to grow
	const size_t nActual = std::min(static_cast<size_t>(nDesiredNewSynapses), candidates.size());
	// ..Check if we're going to surpass the maximum number of synapses.
	Int overrun = static_cast<Int>(connections.numSynapses(segment) + nActual - maxSynapsesPerSegment_);
	
	if (overrun > 0) {
		connections_.destroyMinPermanenceSynapses(segment, static_cast<Int>(overrun), prevWinnerCells);
	}
	// ..Recalculate in case we weren't able to destroy as many synapses as needed.
	const size_t nActualWithMax = std::min(nActual, static_cast<size_t>(maxSynapsesPerSegment_) - connections.numSynapses(segment));


	/********************************************************************
	 * for synapse destination weights
	 * *****************************************************************/

	if(externalPredictiveInputs_ > 0u){
		const auto& border
			= find_if(
				candidates.begin(), candidates.end(),
				[&](const auto& cell){
					return cell >= numberOfCells();
				}
			);

		vector<CellIdx> insideCandidates(candidates.begin(), border);
		vector<CellIdx> outsideCandidates(border, candidates.end());

		rng_.shuffle(insideCandidates.begin(), insideCandidates.end());
		rng_.shuffle(outsideCandidates.begin(), outsideCandidates.end());

		candidates.clear();
		candidates.reserve(nActualWithMax);

		const htm::Int64 requestInsideCandidatesNum 
			= static_cast<htm::Int64>(static_cast<double>(nActualWithMax) * synapseDestinationWeight_);
		const htm::Int64 requestOutsideCandidatesNum 
			= static_cast<htm::Int64>(static_cast<double>(nActualWithMax) * (1.0 - synapseDestinationWeight_));
	
		const htm::Int64 insideSize = static_cast<htm::Int64>(insideCandidates.size());
		const htm::Int64 outsideSize = static_cast<htm::Int64>(outsideCandidates.size());

		htm::Int64 diffInsideCandidatesNum = insideSize - requestInsideCandidatesNum;
		htm::Int64 diffOutsizeCandidatesNum = outsideSize - requestOutsideCandidatesNum;

		// If the value is positive, the number of cells does not
		// exceed, round to zero
		diffInsideCandidatesNum = std::min(diffInsideCandidatesNum, 0LL);
		diffOutsizeCandidatesNum = std::min(diffOutsizeCandidatesNum, 0LL);

		// For the requested number of cells, the missing number of
		// cells is pushed to the opposite side.
		const htm::Int64 requestAdjustInsideNum
			= requestInsideCandidatesNum - diffOutsizeCandidatesNum;
		const htm::Int64 requestAdjustOutsizeNum
			= requestOutsideCandidatesNum - diffInsideCandidatesNum;

		const htm::Int64 requestInsideNum = std::min(requestAdjustInsideNum, insideSize);
		const htm::Int64 requestOutsideNum = std::min(requestAdjustOutsizeNum, outsideSize);

		std::copy(
			insideCandidates.begin(),
			insideCandidates.begin() + requestInsideNum,
			back_inserter(candidates)
		);

		std::copy(
			outsideCandidates.begin(),
			outsideCandidates.begin() + requestOutsideNum,
			back_inserter(candidates)
		);

	}else{
		// Pick nActual cells randomly.
		rng_.shuffle(candidates.begin(), candidates.end());
	}

	/*******************************************************************/

	const size_t nDesired = connections.numSynapses(segment) + nActualWithMax; //num synapses on seg after this function (+-), see #COND
	for (const auto syn : candidates) {
		// #COND: this loop finishes two folds: a) we ran out of candidates (above), b) we grew the desired number of new synapses (below)
		if(connections.numSynapses(segment) == nDesired) break;

		connections_.createSynapse(segment, syn, initialPermanence_); //TODO createSynapse consider creating a vector of new synapses at once?
	}
}

void TemporalMemoryExtension::activatePredictedColumn_(
	vector<Segment>::const_iterator columnActiveSegmentsBegin,
	vector<Segment>::const_iterator columnActiveSegmentsEnd,
	const SDR &prevActiveCells,
	const vector<CellIdx> &prevWinnerCells,
	const bool learn
){

	auto activeSegment = columnActiveSegmentsBegin;
	do {
		const CellIdx cell = connections.cellForSegment(*activeSegment);
		activeCells_.push_back(cell);
		winnerCells_.push_back(cell);

		// This cell might have multiple active segments.
		do {
			if (learn) {
				segmentDutyCycle_.updateDutyCycle(*activeSegment, true);
				connections_.adaptSegment(*activeSegment, prevActiveCells,
							permanenceIncrement_, permanenceDecrement_, true);

				if(segmentDutyCycle_.isExistSegment(*activeSegment)) {
					const auto& [period, eacc] = segmentDutyCycle_.getDutyCycle(*activeSegment);

					const auto& segData = connections_.dataForSegment(*activeSegment);
					const Real synWeight = std::min(
							static_cast<Real>(segData.synapses.size()) 
								/ static_cast<Real>(maxNewSynapseCount_), 1.0f);

					const Real nDesired =
						static_cast<Real>(maxNewSynapseCount_) -
						static_cast<Real>(numActivePotentialSynapsesForSegment_[*activeSegment]);

					const Int32 nGrowDesired =
						static_cast<Int32>(
							(1.0 - createSynWeight_ * synWeight *
								adjuster_(eacc, activateWeight_)) * nDesired);

					if (nGrowDesired > 0) {
						growSynapses_(*activeSegment, nGrowDesired, prevWinnerCells);
					}
				}
			}
		} while (++activeSegment != columnActiveSegmentsEnd &&
				connections.cellForSegment(*activeSegment) == cell);
	} while (activeSegment != columnActiveSegmentsEnd);
}

void TemporalMemoryExtension::burstColumn_(
	const UInt column,
	vector<Segment>::const_iterator columnMatchingSegmentsBegin,
	vector<Segment>::const_iterator columnMatchingSegmentsEnd,
	const SDR &prevActiveCells,
	const vector<CellIdx> &prevWinnerCells,
	const bool learn
){

	// Calculate the active cells: active become ALL the cells in this mini-column
	const auto newCells = cellsForColumn(column);
	activeCells_.insert(activeCells_.end(), newCells.begin(), newCells.end());

	const auto bestMatchingSegment =
		std::max_element(columnMatchingSegmentsBegin, columnMatchingSegmentsEnd,
						[&](Segment a, Segment b) {
							return (numActivePotentialSynapsesForSegment_[a] <
									numActivePotentialSynapsesForSegment_[b]);
						});

	const CellIdx winnerCell =
		(bestMatchingSegment != columnMatchingSegmentsEnd)
			? connections.cellForSegment(*bestMatchingSegment)
			: getLeastUsedCell_(column); //TODO replace (with random?) this is extremely costly, removing makes TM 6x faster!

	winnerCells_.push_back(winnerCell);

	// Learn.
	if (learn) {
		if (bestMatchingSegment != columnMatchingSegmentsEnd) {
			// Learn on the best matching segment.
			connections_.adaptSegment(*bestMatchingSegment, prevActiveCells,
						permanenceIncrement_, permanenceDecrement_, true);

			const SynapseIdx numActiveSynapses = numActivePotentialSynapsesForSegment_[*bestMatchingSegment];
			const SynapseIdx numWinnerSynapses = numWinnerPotentialSynapsesForSegment_[*bestMatchingSegment];

			const Int32 nGrowDesired 
				= (exceptionHandling_  && isExceptionHandling_(numActiveSynapses,numWinnerSynapses))
					? (maxNewSynapseCount_ - numWinnerSynapses):
						(maxNewSynapseCount_ - numActiveSynapses);

			// const Int32 nGrowDesired = maxNewSynapseCount_ - numActiveSynapses;
			
			if (nGrowDesired > 0) {
				growSynapses_(*bestMatchingSegment, nGrowDesired, prevWinnerCells);
			}
		} else {
			// No matching segments.
			// Grow a new segment and learn on it.

			// Don't grow a segment that will never match.
			
			/************************************************
			 * test
			 ***********************************************/
			const UInt32 nGrowExact =
			 	std::min(static_cast<UInt32>(maxNewSynapseCount_), static_cast<UInt32>(prevWinnerCells.size()));
			// const UInt32 nGrowExact
			//	= static_cast<UInt32>(prevWinnerCells.size());

			/************************************************
			 * test
			 ***********************************************/
			
			if (nGrowExact > 0) {
				const Segment segment =
					connections_.createSegment(winnerCell, maxSegmentsPerCell_);

				growSynapses_(segment, nGrowExact, prevWinnerCells);
				// NTA_ASSERT(connections.numSynapses(segment) == nGrowExact);
			}
		}
	}
}

void TemporalMemoryExtension::punishPredictedColumn_(
	vector<Segment>::const_iterator columnMatchingSegmentsBegin,
	vector<Segment>::const_iterator columnMatchingSegmentsEnd,
	const SDR &prevActiveCells
) {
	if (predictedSegmentDecrement_ > 0.0) {
		for (auto matchingSegment = columnMatchingSegmentsBegin;
			matchingSegment != columnMatchingSegmentsEnd; matchingSegment++) {
			connections_.adaptSegment(*matchingSegment, prevActiveCells,
						-predictedSegmentDecrement_, 0.0, true);
		}
	}
}

void TemporalMemoryExtension::destroySynapsesByASA_(
	vector<Segment>::const_iterator columnActiveSegmentBegin,
	vector<Segment>::const_iterator columnActiveSegmentEnd
) {
	for(auto activeSegment = columnActiveSegmentBegin;
		activeSegment != columnActiveSegmentEnd; activeSegment++) {
		
		if(segmentDutyCycle_.isExistSegment(*activeSegment)) {
			segmentDutyCycle_.updateDutyCycle(*activeSegment, false);
			
			const auto& [period, eacc] = segmentDutyCycle_.getDutyCycle(*activeSegment);
			const auto& segData = connections_.dataForSegment(*activeSegment);

			Int nDestroySyns
				= static_cast<Int>(
					destroySynWeight_ * (1.0 - adjuster_(eacc, activateWeight_))
					* static_cast<Real>(segData.synapses.size())
				);

			if(nDestroySyns > 0)
				connections_.destroyMinPermanenceSynapses(*activeSegment, nDestroySyns);
		}
	}
}

const bool TemporalMemoryExtension::isExceptionHandling_(
	const SynapseIdx numActivePotentialSynapses,
	const SynapseIdx numWinnerPotentialSynapses
) const {
	return
		(numActivePotentialSynapses >= maxNewSynapseCount_)
		&&(numWinnerPotentialSynapses < maxNewSynapseCount_);
}

void TemporalMemoryExtension::activateCells(
	const SDR &activeColumns,
	const bool learn
){
	NTA_CHECK(columnDimensions_.size() > 0) << "TM constructed using the default TM() constructor, which may only be used for serialization. "
		<< "Use TM constructor where you provide at least column dimensions, eg: TM tm({32});";

	NTA_CHECK( activeColumns.dimensions.size() == columnDimensions_.size() )  //this "hack" because columnDimensions_, and SDR.dimensions are vectors
		//of different type, so we cannot directly compare
		<< "TM invalid input dimensions: " << activeColumns.dimensions.size() << " vs. " << columnDimensions_.size();
	
	for(size_t i=0; i< columnDimensions_.size(); i++) {
		NTA_CHECK(static_cast<size_t>(activeColumns.dimensions[i]) == static_cast<size_t>(columnDimensions_[i])) << "Dimensions must be the same.";
	}
		
	auto &sparse = activeColumns.getSparse();

	SDR prevActiveCells({static_cast<CellIdx>(numberOfCells() + externalPredictiveInputs_)});
	prevActiveCells.setSparse(activeCells_);
	activeCells_.clear();

	const vector<CellIdx> prevWinnerCells = std::move(winnerCells_);

	//maps segment S to a new segment that is at start of a column where
	//S belongs. 
	//for 3 cells per columns: 
	//s1_1, s1_2, s1_3, s2_1, s2_2, s2_3, ...
	//columnForSegment (for short here CFS)
	//CFS(s1_1) = s1_1 = "start of column 1"
	//CFS(s1_2) = s1_1
	//CFS(s1_3) = s1_1
	//CFS(s2_1) = s2_1 = "column 2"
	//CFS(s2_2) = s2_1
	//...
	const auto toColumns = [&](const Segment segment) {
		return connections.cellForSegment(segment) / cellsPerColumn_;
	};
	const auto identity = [](const ElemSparse a) {return a;}; //TODO use std::identity when c++20

	handler_.reset();

	for (auto &&columnData : groupBy( //group by columns, and convert activeSegments & matchingSegments to cols. 
			sparse, identity,
			activeSegmentsForInner_,   toColumns,
			matchingSegmentsForInner_, toColumns)) {

		Segment column; //we say "column", but it's the first segment of n-segments/cells that belong to the column
		vector<Segment>::const_iterator activeColumnsBegin, activeColumnsEnd, 
									columnActiveSegmentsBegin, columnActiveSegmentsEnd, 
										columnMatchingSegmentsBegin, columnMatchingSegmentsEnd;

		// for column in activeColumns (the 'sparse' above):
		//   get its active segments ( >= connectedThr)
		//   get its matching segs   ( >= TODO
		std::tie(column, 
				activeColumnsBegin, activeColumnsEnd, 
				columnActiveSegmentsBegin, columnActiveSegmentsEnd, 
				columnMatchingSegmentsBegin, columnMatchingSegmentsEnd
		) = columnData;

		const bool isActiveColumn = activeColumnsBegin != activeColumnsEnd;
		if (isActiveColumn) { //current active column...
			if (columnActiveSegmentsBegin != columnActiveSegmentsEnd) {
			//...was also predicted -> learn :o)
				activatePredictedColumn_(
					columnActiveSegmentsBegin, columnActiveSegmentsEnd,
					prevActiveCells, prevWinnerCells, learn);
			} else {
			//...has not been predicted -> 
				burstColumn_(column,
							columnMatchingSegmentsBegin, columnMatchingSegmentsEnd,
							prevActiveCells, prevWinnerCells, 
					learn);
			}

		} else { // predicted but not active column -> unlearn
			if (learn) {
				punishPredictedColumn_(columnMatchingSegmentsBegin, columnMatchingSegmentsEnd, prevActiveCells);
				// punishPredictedColumn_(columnActiveSegmentsBegin, columnActiveSegmentsEnd, prevActiveCells);
				destroySynapsesByASA_(columnActiveSegmentsBegin, columnActiveSegmentsEnd);
			}
		} //else: not predicted & not active -> no activity -> does not show up at all
	}
	segmentsValid_ = false;
}

void TemporalMemoryExtension::activateDendrites(
	const bool learn,
	const SDR &externalPredictiveInputsActive,
	const SDR &externalPredictiveInputsWinners
){
	if( externalPredictiveInputs_ > 0 ){
		NTA_CHECK( externalPredictiveInputsActive.size  == externalPredictiveInputs_ );
		NTA_CHECK( externalPredictiveInputsWinners.size == externalPredictiveInputs_ );
		NTA_CHECK( externalPredictiveInputsActive.dimensions == externalPredictiveInputsWinners.dimensions);

#ifdef NTA_ASSERTIONS_ON
		SDR both(externalPredictiveInputsActive.dimensions);
		both.intersection(externalPredictiveInputsActive, externalPredictiveInputsWinners);
		NTA_ASSERT(both == externalPredictiveInputsWinners) << "externalPredictiveInputsWinners must be a subset of externalPredictiveInputsActive";
#endif
	} else {
		NTA_CHECK( externalPredictiveInputsActive.getSum() == 0u && externalPredictiveInputsWinners.getSum() == 0u )
			<< "External predictive inputs must be declared to TM constructor!";
	}

	if( segmentsValid_ ) return;

	for(const auto &active : externalPredictiveInputsActive.getSparse()) {
		NTA_ASSERT( active < externalPredictiveInputs_ );
		activeCells_.push_back( static_cast<CellIdx>(active + numberOfCells()) ); 
	}

	for(const auto &winner : externalPredictiveInputsWinners.getSparse()) {
		NTA_ASSERT( winner < externalPredictiveInputs_ );
		winnerCells_.push_back( static_cast<CellIdx>(winner + numberOfCells()) );
	}

	const size_t length = connections.segmentFlatListLength();

	numWinnerPotentialSynapsesForSegment_.assign(length, 0);
	numWinnerConnectedSynapsesForSegment_
		= connections_.computeActivity(
			numWinnerPotentialSynapsesForSegment_,
			winnerCells_,
			false
		);

	numActivePotentialSynapsesForSegment_.assign(length, 0);
	numActiveConnectedSynapsesForSegment_
		= connections_.computeActivity(
			numActivePotentialSynapsesForSegment_,
			activeCells_,
			learn
		);


	// Active segments, connected synapses.

	activeSegmentsForInner_.clear();
	activeSegmentsForOuter_.clear();

	innerSelector_->select(
		numActiveConnectedSynapsesForSegment_,
		connections_.getActiveRelationsForSegments(),
		{activateSegmentsCapacity_, activationThreshold_},
		activeSegmentsForInner_
	);

	outerSelector_->select(
		numActiveConnectedSynapsesForSegment_,
		connections_.getActiveRelationsForSegments(),
		{activateSegmentsCapacity_, activationThreshold_},
		activeSegmentsForOuter_
	);


	// Update segment bookkeeping.
	if (learn) {
		for (const auto segment : activeSegmentsForInner_) {
			connections_.dataForSegment(segment).lastUsed
				= connections.iteration(); //TODO the destroySegments based on LRU is expensive. Better random? or "energy" based on sum permanences?
		}
	}

	// Matching segments, potential synapses.
	matchingSegmentsForInner_.clear();
	innerSelector_->select(
		numActivePotentialSynapsesForSegment_,
		connections_.getMatchingRelationsForSegments(),
		{matchingSegmentsCapacity_, minThreshold_},
		matchingSegmentsForInner_
	);
	
	segmentsValid_ = true;
}

void TemporalMemoryExtension::compute(
	const SDR &activeColumns, 
	const bool learn,
	const SDR &externalPredictiveInputsActive,
	const SDR &externalPredictiveInputsWinners
){
	activateDendrites(learn, externalPredictiveInputsActive, externalPredictiveInputsWinners);

	calculateAnomalyScore_(activeColumns);

	activateCells(activeColumns, learn);
}

void TemporalMemoryExtension::calculateAnomalyScore_(const SDR &activeColumns){

	// Update Anomaly Metric.  The anomaly is the percent of active columns that
	// were not predicted. 
	// Must be computed here, between `activateDendrites()` and `activateCells()`.
	switch(tmAnomaly_.mode_) {

		case ANMode::DISABLED: {
			tmAnomaly_.anomaly_ = 0.5f;
		} break;

		case ANMode::RAW: {
			tmAnomaly_.anomaly_ 
				= computeRawAnomalyScore(
					activeColumns,
					cellsToColumns( getPredictiveCells())
				);
		} break;

		case ANMode::LIKELIHOOD: {
			const Real raw 
				= computeRawAnomalyScore(
					activeColumns,
					cellsToColumns( getPredictiveCells())
				);
			tmAnomaly_.anomaly_ 
				= tmAnomaly_.anomalyLikelihood_.anomalyProbability(raw);
		} break;

		case ANMode::LOGLIKELIHOOD: {
			const Real raw 
				= computeRawAnomalyScore(
					activeColumns,
					cellsToColumns( getPredictiveCells())
				);
			const Real like
				= tmAnomaly_.anomalyLikelihood_.anomalyProbability(raw);
			const Real log 
				= tmAnomaly_.anomalyLikelihood_.computeLogLikelihood(like);
			tmAnomaly_.anomaly_ = log;
		} break;
	// TODO: Update mean & standard deviation of anomaly here.
	};

	NTA_ASSERT(tmAnomaly_.anomaly_ >= 0.0f and tmAnomaly_.anomaly_ <= 1.0f) << "TM.anomaly is out-of-bounds!";

}

void TemporalMemoryExtension::compute(
	const SDR &activeColumns,
	const bool learn
){
	SDR externalPredictiveInputsActive({ externalPredictiveInputs_ });
	SDR externalPredictiveInputsWinners({ externalPredictiveInputs_ });
	compute( activeColumns, learn, externalPredictiveInputsActive, externalPredictiveInputsWinners );
}

void TemporalMemoryExtension::reset(void) {
	activeCells_.clear();
	winnerCells_.clear();
	activeSegmentsForInner_.clear();
	activeSegmentsForOuter_.clear();
	matchingSegmentsForInner_.clear();
	segmentsValid_ = false;
	tmAnomaly_.anomaly_ = -1.0f; //TODO reset rather to 0.5 as default (undecided) anomaly
}

// ==============================
//  Helper functions
// ==============================
UInt TemporalMemoryExtension::columnForCell(const CellIdx cell) const {
	NTA_ASSERT(cell < numberOfCells());
	return cell / cellsPerColumn_;
}

SDR TemporalMemoryExtension::cellsToColumns(const SDR& cells) const {
	auto correctDims = getColumnDimensions(); //nD column dimensions (eg 10x100)
	correctDims.push_back(static_cast<CellIdx>(getCellsPerColumn())); //add n+1-th dimension for cellsPerColumn (eg. 10x100x8)

	NTA_CHECK(cells.dimensions.size() == correctDims.size()) 
		<< "cells.dimensions must match TM's (column dims x cellsPerColumn) ";

	for(size_t i = 0; i<correctDims.size(); i++) 
		NTA_CHECK(correctDims[i] == cells.dimensions[i]);

	SDR cols(getColumnDimensions());
	auto& dense = cols.getDense();
	for(const auto cell : cells.getSparse()) {
		const auto col = columnForCell(cell);
		dense[col] = static_cast<ElemDense>(1);
	}
	cols.setDense(dense);

	NTA_ASSERT(cols.size == numColumns_); 
	return cols;
}

SDRex<NumCells> TemporalMemoryExtension::countCellsOfColumns(const SDR& cells) const {
	const std::vector<UInt> cellsDimensions = getCellsDimensions();

	NTA_CHECK(cells.dimensions.size() == cellsDimensions.size())
		<< "cells.dimensions must match TM's (column dims x cellsPerColumn) ";
	for(std::size_t i = 0u, size = cellsDimensions.size(); i < size; ++i)
		NTA_CHECK(cellsDimensions.at(i) == cells.dimensions.at(i));

	SDRex<NumCells> cols(getColumnDimensions());

	SDR_dense_t dense(cols.size, static_cast<ElemDense>(0));
	std::vector<NumCells> dataDense(cols.size, static_cast<NumCells>(0));

	for(const ElemSparse& cell : cells.getSparse()){
		const UInt col = columnForCell(cell);

		dense.at(col) = static_cast<ElemDense>(1);
		dataDense.at(col)++;
	}

	cols.setDense(dense);
	cols.setExDataDense(dataDense);

	NTA_ASSERT(cols.size == numColumns_);
	return cols;
}

vector<CellIdx> TemporalMemoryExtension::cellsForColumn(const CellIdx column) const { 
	const CellIdx start = cellsPerColumn_ * column;
	const CellIdx end = start + cellsPerColumn_;

	vector<CellIdx> cellsInColumn;
	cellsInColumn.reserve(cellsPerColumn_);
	for (CellIdx i = start; i < end; i++) {
		cellsInColumn.push_back(i);
	}

	return cellsInColumn;
}

// void TemporalMemoryExtension::identifyActiveColumns(
// 	const SDR& predictiveCells,
// 	const UInt maxActiveColumns,
// 	SDR& activeColumns
// ) const {
// 	activeColumns.initialize(getColumnDimensions());
// 	const auto& predictiveColumns = countCellsOfColumns(predictiveCells);
// 	const auto& numCellsDense = predictiveColumns.getExDataDense();

// 	const auto& connectedCells = connections_.getConnectedCells();

// 	std::set<ElemSparse> activateColumnsMultipleLayers;
// 	std::set<ElemSparse> acitvateColumnsSingleLayers;

// 	for(auto&& cell : predictiveCells.getSparse()){
// 		const SDR_sparse_t& precells = connectedCells.at(static_cast<CellIdx>(cell));

// 		if(	precells.front() < externalPredictiveInputs && externalPredictiveInputs <= precells.back())
// 			activateColumnsMultipleLayers.insert(columnForCell(cell));
// 		else
// 		acitvateColumnsSingleLayers.insert(columnForCell(cell));
// 	}

// 	SDR_sparse_t priorityColumns(activateColumnsMultipleLayers.begin(), activateColumnsMultipleLayers.end());
// 	SDR_sparse_t duplicateColumns(acitvateColumnsSingleLayers.begin(), acitvateColumnsSingleLayers.end());
		
// 	SDR_sparse_t posteriorityColumns;

// 	// Remove duplicate (same) columns from duplicateColumns and save it to posteriorityColumns.
// 	std::set_difference(
// 		duplicateColumns.begin(), duplicateColumns.end(),
// 		priorityColumns.begin(), priorityColumns.end(),
// 		std::back_inserter(posteriorityColumns)
// 	);

// 	const UInt numActiveColumns
// 		= std::min(
// 			maxActiveColumns,
// 			static_cast<UInt>(priorityColumns.size() + posteriorityColumns.size())
// 		);

// 	SDR_sparse_t selectedColumns(numActiveColumns);
// 	std::vector<std::pair<ElemSparse, NumCells>> vec(priorityColumns.size());

// 	for(std::size_t i=0u, size = vec.size(); i < size; ++i)
// 		vec.at(i) = {priorityColumns.at(i), numCellsDense.at(priorityColumns.at(i))};

// 	std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b){return a.second > b.second;});

// 	std::size_t i = 0u;

// 	for(size_t size = vec.size(); i < size && i < numActiveColumns; ++i)
// 		selectedColumns.at(i) = vec.at(i).first;

// 	if(i != numActiveColumns){

// 		vec.clear();
// 		vec.resize(posteriorityColumns.size());

// 		for(std::size_t j = 0u, size = posteriorityColumns.size(); j < size; ++j)
// 			vec.at(j) = {posteriorityColumns.at(j), numCellsDense.at(posteriorityColumns.at(j))};

// 		std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b){return a.second > b.second;});

// 		for(std::size_t j = 0u, size = vec.size(); j < size && j < (numActiveColumns - i); ++j)
// 			selectedColumns.at(i + j) = vec.at(j).first;
// 	}

// 	sort(selectedColumns.begin(), selectedColumns.end());
// 	activeColumns.setSparse(selectedColumns);
// }

const double TemporalMemoryExtension::calAverageNumSynapsesOnActivateSegment() const {
	const SDR_sparse_t& segments = getActiveSegments();

	if(segments.size() == 0u) return 0.0;

	std::size_t sum = 0u;

	for(const ElemSparse seg : segments)
		sum += connections.numSynapses(seg);
	
	return static_cast<double>(sum) / static_cast<double>(segments.size());
}

const double TemporalMemoryExtension::calAverageNumActiveSynapsesOnActiveSegment() const {
	const SDR_sparse_t& segments = getActiveSegments();

	if(segments.size() == 0u) return 0.0;

	std::size_t sum = 0u;

	for(const ElemSparse seg : segments)
		sum += numActiveConnectedSynapsesForSegment_[seg];

	// return static_cast<double>(sum) / static_cast<double>(segments.size());
	return static_cast<double>(sum);
}

vector<CellIdx> TemporalMemoryExtension::getActiveCells() const { 
	return activeCells_; 
}

void TemporalMemoryExtension::getActiveCells(SDR &activeCells) const {
	NTA_CHECK( activeCells.size == numberOfCells() );
	activeCells.setSparse( getActiveCells() );
}

SDR TemporalMemoryExtension::getPredictiveCells() const {
	return getPredictiveCells(activeSegmentsForOuter_);
}

SDR TemporalMemoryExtension::getPredictiveCells(
	const vector<Segment>& activeSegments
) const {
	NTA_CHECK( segmentsValid_ )
		<< "Call TM.activateDendrites() before TM.getPredictiveCells()!";

	auto correctDims = getColumnDimensions();
	correctDims.push_back(static_cast<CellIdx>(getCellsPerColumn()));
	SDR predictive(correctDims);

	std::set<CellIdx> uniqueCells;
	//uniqueCells.reserve(activeSegments_.size());

	for (const auto segment : activeSegments) {
		const CellIdx cell = connections.cellForSegment(segment);
		uniqueCells.insert(cell); //set keeps the cells unique
	}

	vector<CellIdx> predictiveCells(uniqueCells.begin(), uniqueCells.end());
	predictive.setSparse(predictiveCells);
	return predictive;
}

vector<CellIdx> TemporalMemoryExtension::getWinnerCells() const {
	return winnerCells_;
}

void TemporalMemoryExtension::getWinnerCells(SDR &winnerCells) const {
	NTA_CHECK( winnerCells.size == numberOfCells() );
	winnerCells.setSparse( getWinnerCells() );
}

vector<Segment> TemporalMemoryExtension::getActiveSegments() const {
	NTA_CHECK( segmentsValid_ )
		<< "Call TM.activateDendrites() before TM.getActiveSegments()!";

	return activeSegmentsForInner_;
}

vector<Segment> TemporalMemoryExtension::getMatchingSegments() const {
	NTA_CHECK( segmentsValid_ )
		<< "Call TM.activateDendrites() before TM.getActiveSegments()!";

	return matchingSegmentsForInner_;
}

vector<CellIdx> TemporalMemoryExtension::getCellsDimensions() const {
	vector<CellIdx> cellsDimension = getColumnDimensions();
	cellsDimension.emplace_back(cellsPerColumn_);

	return cellsDimension;
}

SynapseIdx TemporalMemoryExtension::getActivationThreshold() const {
	return activationThreshold_;
}

void TemporalMemoryExtension::setActivationThreshold(const SynapseIdx activationThreshold) {
	activationThreshold_ = activationThreshold;
}

Permanence TemporalMemoryExtension::getInitialPermanence() const {
	return initialPermanence_;
}

void TemporalMemoryExtension::setInitialPermanence(const Permanence initialPermanence) {
	initialPermanence_ = initialPermanence;
}

Permanence TemporalMemoryExtension::getConnectedPermanence() const {
	return connectedPermanence_;
}

SynapseIdx TemporalMemoryExtension::getMinThreshold() const { 
	return minThreshold_;
}

void TemporalMemoryExtension::setMinThreshold(const SynapseIdx minThreshold) {
	minThreshold_ = minThreshold;
}

SynapseIdx TemporalMemoryExtension::getMaxNewSynapseCount() const {
	return maxNewSynapseCount_;
}

void TemporalMemoryExtension::setMaxNewSynapseCount(const SynapseIdx maxNewSynapseCount) {
	maxNewSynapseCount_ = maxNewSynapseCount;
}

bool TemporalMemoryExtension::getCheckInputs() const {
	return checkInputs_;
}

void TemporalMemoryExtension::setCheckInputs(bool checkInputs) {
	checkInputs_ = checkInputs;
}

Permanence TemporalMemoryExtension::getPermanenceIncrement() const {
	return permanenceIncrement_;
}

void TemporalMemoryExtension::setPermanenceIncrement(Permanence permanenceIncrement) {
	permanenceIncrement_ = permanenceIncrement;
}

Permanence TemporalMemoryExtension::getPermanenceDecrement() const {
	return permanenceDecrement_;
}

void TemporalMemoryExtension::setPermanenceDecrement(Permanence permanenceDecrement) {
	permanenceDecrement_ = permanenceDecrement;
}

Permanence TemporalMemoryExtension::getPredictedSegmentDecrement() const {
	return predictedSegmentDecrement_;
}

void TemporalMemoryExtension::setPredictedSegmentDecrement(Permanence predictedSegmentDecrement) {
	predictedSegmentDecrement_ = predictedSegmentDecrement;
}

SegmentIdx TemporalMemoryExtension::getMaxSegmentsPerCell() const {
	return maxSegmentsPerCell_;
}

SynapseIdx TemporalMemoryExtension::getMaxSynapsesPerSegment() const {
	return maxSynapsesPerSegment_;
}

bool TemporalMemoryExtension::getExceptionHandling() const {
	return exceptionHandling_;
}

UInt TemporalMemoryExtension::getExternalPredictiveInputs() const {
	return externalPredictiveInputs_;
}

Real TemporalMemoryExtension::getSynapseDestinationWeight() const {
	return synapseDestinationWeight_;
}

TemporalMemoryExtension::ANMode TemporalMemoryExtension::getAnomalyMode() const {
	return tmAnomaly_.mode_;
}

Real TemporalMemoryExtension::getCreateSynWeight() const {
	return createSynWeight_;
}

Real TemporalMemoryExtension::getDestroySynWeight() const {
	return destroySynWeight_;
}

Real TemporalMemoryExtension::getActivateWeight() const {
	return activateWeight_;
}

UInt TemporalMemoryExtension::getActivateSegmentsCapacity() const {
	return activateSegmentsCapacity_;
}

UInt TemporalMemoryExtension::getMatchingSegmentsCapacity() const {
	return matchingSegmentsCapacity_;
}

SSMode TemporalMemoryExtension::getInnerSegmentSelectorMode() const {
	return innerSegmentSelectorMode_;
}

SSMode TemporalMemoryExtension::getOuterSegmentSelectorMode() const {
	return outerSegmentSelectorMode_;
}

UInt TemporalMemoryExtension::version() const {
	return TM_VERSION;
}


static set<pair<CellIdx, SynapseIdx>>getComparableSegmentSet(
	const Connections &connections,
	const vector<Segment> &segments
) {
	set<pair<CellIdx, SynapseIdx>> segmentSet;
	for (Segment segment : segments) {
		segmentSet.emplace(connections.cellForSegment(segment),
						connections.idxOnCellForSegment(segment));
	}
	return segmentSet;
}

bool TemporalMemoryExtension::operator==(const TemporalMemoryExtension &other) const {
	if (numColumns_ != other.numColumns_ ||
		columnDimensions_ != other.columnDimensions_ ||
		cellsPerColumn_ != other.cellsPerColumn_ ||
		activationThreshold_ != other.activationThreshold_ ||
		minThreshold_ != other.minThreshold_ ||
		maxNewSynapseCount_ != other.maxNewSynapseCount_ ||
		initialPermanence_ != other.initialPermanence_ ||
		connectedPermanence_ != other.connectedPermanence_ ||
		permanenceIncrement_ != other.permanenceIncrement_ ||
		permanenceDecrement_ != other.permanenceDecrement_ ||
		predictedSegmentDecrement_ != other.predictedSegmentDecrement_ ||
		activeCells_ != other.activeCells_ ||
		winnerCells_ != other.winnerCells_ ||
		maxSegmentsPerCell_ != other.maxSegmentsPerCell_ ||
		maxSynapsesPerSegment_ != other.maxSynapsesPerSegment_ ||
		tmAnomaly_.anomaly_ != other.tmAnomaly_.anomaly_ ||
		tmAnomaly_.mode_ != other.tmAnomaly_.mode_ || 
		tmAnomaly_.anomalyLikelihood_ != other.tmAnomaly_.anomalyLikelihood_ ) {
		return false;
	}

	if (connections != other.connections) {
		return false;
	}

	if (getComparableSegmentSet(connections, activeSegmentsForInner_) !=
			getComparableSegmentSet(other.connections, other.activeSegmentsForInner_) ||
		getComparableSegmentSet(connections, matchingSegmentsForInner_) !=
			getComparableSegmentSet(other.connections, other.matchingSegmentsForInner_)) {
		return false;
	}

	return true;
}

//----------------------------------------------------------------------
// Debugging helpers
//----------------------------------------------------------------------

namespace htm {
	
std::ostream& operator<< (std::ostream& stream, const TemporalMemoryExtension& self) {
	stream << "Temporal Memory " << self.connections;
	return stream;
}
	
} // namespace htm


// Print the main TM creation parameters
void TemporalMemoryExtension::printParameters(std::ostream& out) const {
	out << "Temporal Memory Parameters\n";
	out << "version                   = " << TM_VERSION << std::endl
		<< "numColumns                = " << numberOfColumns() << std::endl
		<< "cellsPerColumn            = " << getCellsPerColumn() << std::endl
		<< "activationThreshold       = " << getActivationThreshold() << std::endl
		<< "initialPermanence         = " << getInitialPermanence() << std::endl
		<< "connectedPermanence       = " << getConnectedPermanence() << std::endl
		<< "minThreshold              = " << getMinThreshold() << std::endl
		<< "maxNewSynapseCount        = " << getMaxNewSynapseCount() << std::endl
		<< "permanenceIncrement       = " << getPermanenceIncrement() << std::endl
		<< "permanenceDecrement       = " << getPermanenceDecrement() << std::endl
		<< "predictedSegmentDecrement = " << getPredictedSegmentDecrement()
		<< std::endl
		<< "maxSegmentsPerCell        = " << getMaxSegmentsPerCell() << std::endl
		<< "maxSynapsesPerSegment     = " << getMaxSynapsesPerSegment()
		<< std::endl;
}