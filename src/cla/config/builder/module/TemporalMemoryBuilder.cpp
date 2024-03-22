// TemporalMemoryBuilder.cpp

/** 
 * @file
 * Implementation of TemporalMemoryBuilder.cpp
 */

#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"
#include "cla/extension/algorithms/SegmentSelector.hpp"
#include "cla/model/module/tm/HtmTemporalMemory.hpp"

#include "cla/model/generator/TemporalMemoryGenerator.hpp"
#include "cla/config/builder/module/TemporalMemoryBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using TMJLabel = TemporalMemoryJsonLabels;


/************************************************
 * TemporalMemoryJsonBuilder private functions.
 ***********************************************/

PTemporalMemory TemporalMemoryJsonBuilder::buildHtmTemporalMemory_(const json& config) {
	htm::UInt numRegions;
	std::vector<htm::CellIdx> columnDimensions;
	htm::CellIdx cellsPerColumn;
	htm::SynapseIdx activationThreshold;
	htm::Permanence initialPermanence;
	htm::Permanence connectedPermanence;
	htm::SynapseIdx minThreshold;
	htm::SynapseIdx maxNewSynapseCount;
	htm::Permanence permanenceIncrement;
	htm::Permanence permanenceDecrement;
	htm::Permanence predictedSegmentDecrement;
	htm::Int seed;
	htm::SegmentIdx maxSegmentsPerCell;
	htm::SynapseIdx maxSynapsesPerSegment;
	bool checkInputs;
	bool exceptionHandling;
	htm::UInt externalPredictiveInputs;
	htm::Real synapseDestinationWeight;
	htm::Real createSynWeight;
	htm::Real destroySynWeight;
	htm::Real activateWeight;
	htm::UInt capacityOfNbActiveSegments;
	htm::UInt capacityOfNbMatchingSegments;
	std::string innerSegmentSelectorModeStr;
	std::string outerSegmentSelectorModeStr;
	int anomalyIntMode;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, TMJLabel::PARAM_NUM_REGIONS)) {
			value.get_to(numRegions);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_COLUMN_DIMENSIONS)) {
			value.get_to(columnDimensions);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_CELLS_PER_COLUMN)) {
			value.get_to(cellsPerColumn);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_ACTIVATION_THRESHOLD)) {
			value.get_to(activationThreshold);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_INITIAL_PERMANENCE)) {
			value.get_to(initialPermanence);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_CONNECTED_PERMANENCE)) {
			value.get_to(connectedPermanence);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_MIN_THRESHOLD)) {
			value.get_to(minThreshold);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_MAX_NEW_SYNAPSE_COUNT)) {
			value.get_to(maxNewSynapseCount);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_PERMANENCE_INCREMENT)) {
			value.get_to(permanenceIncrement);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_PERMANENCE_DECREMENT)) {
			value.get_to(permanenceDecrement);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_PREDICTED_SEGMENT_DECREMENT)) {
			value.get_to(predictedSegmentDecrement);
			continue;
		}
		
		if(KeyHelper::contain(key, TMJLabel::PARAM_SEED)) {
			value.get_to(seed);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_MAX_SEGMENTS_PER_CELL)) {
			value.get_to(maxSegmentsPerCell);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_MAX_SYNAPSES_PER_SEGMENT)) {
			value.get_to(maxSynapsesPerSegment);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_CHECK_INPUTS)) {
			value.get_to(checkInputs);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_EXCEPTION_HANDLING)) {
			value.get_to(exceptionHandling);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_EXTERNAL_PREDICTIVE_INPUTS)) {
			value.get_to(externalPredictiveInputs);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_SYNAPSE_DESTINATION_WEIGHT)) {
			value.get_to(synapseDestinationWeight);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_CREATE_SYN_WEIGHT)) {
			value.get_to(createSynWeight);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_DESTROY_SYN_WEIGHT)) {
			value.get_to(destroySynWeight);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_ACTIVATE_WEIGHT)) {
			value.get_to(activateWeight);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_CAPACITY_OF_NB_ACTIVE_SEGMENTS)) {
			value.get_to(capacityOfNbActiveSegments);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_CAPACITY_OF_NB_MATCHING_SEGMENTS)) {
			value.get_to(capacityOfNbMatchingSegments);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_INNER_SEGMENT_SELECTOR_MODE)) {
			value.get_to(innerSegmentSelectorModeStr);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_OUTER_SEGMENT_SELECTOR_MODE)) {
			value.get_to(outerSegmentSelectorModeStr);
			continue;
		}

		if(KeyHelper::contain(key, TMJLabel::PARAM_ANOMALY_MODE)) {
			value.get_to(anomalyIntMode);
			continue;
		}
		
		CLA_ALERT("Error: There are parameters that are not assumed.");
	}


	return TemporalMemoryGenerator<HtmTemporalMemory>::generate(
		numRegions, columnDimensions, cellsPerColumn, activationThreshold,
		initialPermanence, connectedPermanence, minThreshold,
		maxNewSynapseCount, permanenceIncrement, permanenceDecrement,
		predictedSegmentDecrement, seed, maxSegmentsPerCell,
		maxSynapsesPerSegment, checkInputs, exceptionHandling,
		externalPredictiveInputs, synapseDestinationWeight,
		createSynWeight, destroySynWeight, activateWeight,
		capacityOfNbActiveSegments, capacityOfNbMatchingSegments,
		htm::SegmentSelectors::getMode(innerSegmentSelectorModeStr), 
		htm::SegmentSelectors::getMode(outerSegmentSelectorModeStr),
		static_cast<htm::TemporalMemoryExtension::ANMode>(anomalyIntMode)
	);
}


/************************************************
 * TemporalMemoryJsonBuilder public functions.
 ***********************************************/

PTemporalMemory TemporalMemoryJsonBuilder::buildTemporalMemory(const json& config) {
	if(KeyHelper::contain(config, TMJLabel::HTM_TEMPORAL_MEMORY_LABEL)) {
		return buildHtmTemporalMemory_(config.at(TMJLabel::HTM_TEMPORAL_MEMORY_LABEL));
	}

	CLA_ALERT("Error: There is no temporal memory that can be built.");
}

PTemporalMemory TemporalMemoryJsonBuilder::buildTemporalMemory(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, TMJLabel::HTM_TEMPORAL_MEMORY_LABEL)) {
		return buildHtmTemporalMemory_(config);
	}

	CLA_ALERT("Error: There is no temporal memory that can be built.");
}


} // namespace cla